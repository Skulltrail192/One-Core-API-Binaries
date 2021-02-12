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