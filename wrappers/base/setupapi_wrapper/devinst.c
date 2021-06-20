/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 */


#include "setupapi_private.h"

WINE_DEFAULT_DEBUG_CHANNEL(setupapi);

static struct device **devnode_table;
static unsigned int devnode_table_size;

/* is used to identify if a DeviceInfoSet pointer is
valid or not */
#define SETUP_DEVICE_INFO_SET_MAGIC 0xd00ff056

struct DeviceInfoSet
{
    DWORD magic;        /* if is equal to SETUP_DEVICE_INFO_SET_MAGIC struct is okay */
    GUID ClassGuid;
    HWND hwndParent;
    struct list devices;
};

struct device
{
    struct DeviceInfoSet *set;
    HKEY                  key;
    BOOL                  phantom;
    WCHAR                *instanceId;
    struct list           interfaces;
    GUID                  class;
    DEVINST               devnode;
    struct list           entry;
    BOOL                  removed;
    SP_DEVINSTALL_PARAMS_W params;
    struct driver        *drivers;
    unsigned int          driver_count;
    struct driver        *selected_driver;
};

static void SETUPDI_GuidToString(const GUID *guid, LPWSTR guidStr)
{
    static const WCHAR fmt[] = {'{','%','0','8','X','-','%','0','4','X','-',
        '%','0','4','X','-','%','0','2','X','%','0','2','X','-','%','0','2',
        'X','%','0','2','X','%','0','2','X','%','0','2','X','%','0','2','X','%',
        '0','2','X','}',0};

    swprintf(
	guidStr, 
	fmt, guid->Data1, guid->Data2, guid->Data3,
        guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3],
        guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]);
}

static DEVINST alloc_devnode(struct device *device)
{
    unsigned int i;

    for (i = 0; i < devnode_table_size; ++i)
    {
        if (!devnode_table[i])
            break;
    }

    if (i == devnode_table_size)
    {
        if (devnode_table)
        {
            devnode_table_size *= 2;
            devnode_table = heap_realloc_zero(devnode_table,
                devnode_table_size * sizeof(*devnode_table));
        }
        else
        {
            devnode_table_size = 256;
            devnode_table = heap_alloc_zero(devnode_table_size * sizeof(*devnode_table));
        }
    }

    devnode_table[i] = device;
    return i;
}

static struct device *get_devnode_device(DEVINST devnode)
{
    if (devnode < devnode_table_size)
        return devnode_table[devnode];

    WARN("device node %u not found\n", devnode);
    return NULL;
}

static LSTATUS get_device_property(struct device *device, const DEVPROPKEY *prop_key, DEVPROPTYPE *prop_type,
                BYTE *prop_buff, DWORD prop_buff_size, DWORD *required_size, DWORD flags)
{
    WCHAR key_path[55] = L"Properties\\";
    HKEY hkey;
    DWORD value_type;
    DWORD value_size = 0;
    LSTATUS ls;

    if (!prop_key)
        return ERROR_INVALID_DATA;

    if (!prop_type || (!prop_buff && prop_buff_size))
        return ERROR_INVALID_USER_BUFFER;

    if (flags)
        return ERROR_INVALID_FLAGS;

    SETUPDI_GuidToString(&prop_key->fmtid, key_path + 11);
    swprintf(key_path + 49,L"\\%04X", prop_key->pid);

    ls = RegOpenKeyExW(device->key, key_path, 0, KEY_QUERY_VALUE, &hkey);
    if (!ls)
    {
        value_size = prop_buff_size;
        ls = RegQueryValueExW(hkey, NULL, NULL, &value_type, prop_buff, &value_size);
        RegCloseKey(hkey);
    }

    switch (ls)
    {
    case NO_ERROR:
    case ERROR_MORE_DATA:
        *prop_type = 0xffff & value_type;
        ls = (ls == ERROR_MORE_DATA || !prop_buff) ? ERROR_INSUFFICIENT_BUFFER : NO_ERROR;
        break;
    case ERROR_FILE_NOT_FOUND:
        *prop_type = DEVPROP_TYPE_EMPTY;
        value_size = 0;
        ls = ERROR_NOT_FOUND;
        break;
    default:
        *prop_type = DEVPROP_TYPE_EMPTY;
        value_size = 0;
        FIXME("Unhandled error %#x\n", ls);
        break;
    }

    if (required_size)
        *required_size = value_size;

    return ls;
}

static struct DeviceInfoSet *get_device_set(HDEVINFO devinfo)
{
    struct DeviceInfoSet *set = devinfo;

    if (!devinfo || devinfo == INVALID_HANDLE_VALUE || set->magic != SETUP_DEVICE_INFO_SET_MAGIC)
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return NULL;
    }

    return set;
}

static struct device *get_device(HDEVINFO devinfo, const SP_DEVINFO_DATA *data)
{
    struct DeviceInfoSet *set;
    struct device *device;

    if (!(set = get_device_set(devinfo)))
        return FALSE;

    if (!data || data->cbSize != sizeof(*data) || !data->Reserved)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }

    device = (struct device *)data->Reserved;

    if (device->set != set)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return NULL;
    }

    if (device->removed)
    {
        SetLastError(ERROR_NO_SUCH_DEVINST);
        return NULL;
    }

    return device;
}

static BOOL is_valid_property_type(DEVPROPTYPE prop_type)
{
    DWORD type = prop_type & DEVPROP_MASK_TYPE;
    DWORD typemod = prop_type & DEVPROP_MASK_TYPEMOD;

    if (type > MAX_DEVPROP_TYPE)
        return FALSE;
    if (typemod > MAX_DEVPROP_TYPEMOD)
        return FALSE;

    if (typemod == DEVPROP_TYPEMOD_ARRAY
        && (type == DEVPROP_TYPE_EMPTY || type == DEVPROP_TYPE_NULL || type == DEVPROP_TYPE_STRING
            || type == DEVPROP_TYPE_SECURITY_DESCRIPTOR_STRING))
        return FALSE;

    if (typemod == DEVPROP_TYPEMOD_LIST
        && !(type == DEVPROP_TYPE_STRING || type == DEVPROP_TYPE_SECURITY_DESCRIPTOR_STRING))
        return FALSE;

    return TRUE;
}

/***********************************************************************
 *              CM_Get_DevNode_Property_ExW (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Get_DevNode_Property_ExW(DEVINST devnode, const DEVPROPKEY *prop_key, DEVPROPTYPE *prop_type,
    BYTE *prop_buff, ULONG *prop_buff_size, ULONG flags, HMACHINE machine)
{
    struct device *device = get_devnode_device(devnode);
    LSTATUS ls;

    TRACE("%u, %p, %p, %p, %p, %#x, %p\n", devnode, prop_key, prop_type, prop_buff, prop_buff_size,
          flags, machine);

    if (machine)
        return CR_MACHINE_UNAVAILABLE;

    if (!device)
        return CR_NO_SUCH_DEVINST;

    if (!prop_buff_size)
        return CR_INVALID_POINTER;

    ls = get_device_property(device, prop_key, prop_type, prop_buff, *prop_buff_size, prop_buff_size, flags);
    switch (ls)
    {
    case NO_ERROR:
        return CR_SUCCESS;
    case ERROR_INVALID_DATA:
        return CR_INVALID_DATA;
    case ERROR_INVALID_USER_BUFFER:
        return CR_INVALID_POINTER;
    case ERROR_INVALID_FLAGS:
        return CR_INVALID_FLAG;
    case ERROR_INSUFFICIENT_BUFFER:
        return CR_BUFFER_SMALL;
    case ERROR_NOT_FOUND:
        return CR_NO_SUCH_VALUE;
    }
    return CR_FAILURE;
}

/***********************************************************************
 *              CM_Get_DevNode_PropertyW (SETUPAPI.@)
 */
CONFIGRET WINAPI CM_Get_DevNode_PropertyW(DEVINST dev, const DEVPROPKEY *key, DEVPROPTYPE *type,
    PVOID buf, PULONG len, ULONG flags)
{
    return CM_Get_DevNode_Property_ExW(dev, key, type, buf, len, flags, NULL);
}

BOOL WINAPI SetupDiSetDevicePropertyW(HDEVINFO devinfo, PSP_DEVINFO_DATA device_data, const DEVPROPKEY *key,
                                      DEVPROPTYPE type, const BYTE *buffer, DWORD size, DWORD flags)
{
    static const WCHAR propertiesW[] = {'P', 'r', 'o', 'p', 'e', 'r', 't', 'i', 'e', 's', 0};
    static const WCHAR formatW[] = {'\\', '%', '0', '4', 'X', 0};
    struct device *device;
    HKEY properties_hkey, property_hkey;
    WCHAR property_hkey_path[44];
    LSTATUS ls;

    TRACE("%p %p %p %#x %p %d %#x\n", devinfo, device_data, key, type, buffer, size, flags);

    if (!(device = get_device(devinfo, device_data)))
        return FALSE;

    if (!key || !is_valid_property_type(type)
        || (buffer && !size && !(type == DEVPROP_TYPE_EMPTY || type == DEVPROP_TYPE_NULL))
        || (buffer && size && (type == DEVPROP_TYPE_EMPTY || type == DEVPROP_TYPE_NULL)))
    {
        SetLastError(ERROR_INVALID_DATA);
        return FALSE;
    }

    if (size && !buffer)
    {
        SetLastError(ERROR_INVALID_USER_BUFFER);
        return FALSE;
    }

    if (flags)
    {
        SetLastError(ERROR_INVALID_FLAGS);
        return FALSE;
    }

    ls = RegCreateKeyExW(device->key, propertiesW, 0, NULL, 0, KEY_READ | KEY_WRITE, NULL, &properties_hkey, NULL);
    if (ls)
    {
        SetLastError(ls);
        return FALSE;
    }

    SETUPDI_GuidToString(&key->fmtid, property_hkey_path);
    swprintf(property_hkey_path + 38, formatW, key->pid);

    if (type == DEVPROP_TYPE_EMPTY)
    {
        ls = RegDeleteKeyW(properties_hkey, property_hkey_path);
        RegCloseKey(properties_hkey);
        SetLastError(ls == ERROR_FILE_NOT_FOUND ? ERROR_NOT_FOUND : ls);
        return !ls;
    }
    else if (type == DEVPROP_TYPE_NULL)
    {
        if (!(ls = RegOpenKeyW(properties_hkey, property_hkey_path, &property_hkey)))
        {
            ls = RegDeleteValueW(property_hkey, NULL);
            RegCloseKey(property_hkey);
        }

        RegCloseKey(properties_hkey);
        SetLastError(ls == ERROR_FILE_NOT_FOUND ? ERROR_NOT_FOUND : ls);
        return !ls;
    }
    else
    {
        if (!(ls = RegCreateKeyExW(properties_hkey, property_hkey_path, 0, NULL, 0, KEY_READ | KEY_WRITE, NULL,
                                  &property_hkey, NULL)))
        {
            ls = RegSetValueExW(property_hkey, NULL, 0, 0xffff0000 | (0xffff & type), buffer, size);
            RegCloseKey(property_hkey);
        }

        RegCloseKey(properties_hkey);
        SetLastError(ls);
        return !ls;
    }
}

/***********************************************************************
 *              SetupDiGetDevicePropertyW (SETUPAPI.@)
 */
BOOL WINAPI SetupDiGetDevicePropertyW(HDEVINFO devinfo, PSP_DEVINFO_DATA device_data,
                const DEVPROPKEY *prop_key, DEVPROPTYPE *prop_type, BYTE *prop_buff,
                DWORD prop_buff_size, DWORD *required_size, DWORD flags)
{
    struct device *device;
    LSTATUS ls;

    TRACE("%p, %p, %p, %p, %p, %d, %p, %#x\n", devinfo, device_data, prop_key, prop_type, prop_buff, prop_buff_size,
          required_size, flags);

    if (!(device = get_device(devinfo, device_data)))
        return FALSE;

    ls = get_device_property(device, prop_key, prop_type, prop_buff, prop_buff_size, required_size, flags);

    SetLastError(ls);
    return !ls;
}
