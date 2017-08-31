/*
 * Copyright 2012 Austin English
 * Copyright 2015 Michael Müller
 * Copyright 2015 Sebastian Lackner
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
 */

#include "windows.h"
#include "shlwapi.h"
#include "fdi.h"
#include "wusa.h"
#include "wine/debug.h"
#include "wine/unicode.h"

WINE_DEFAULT_DEBUG_CHANNEL(wusa);

/* from msvcrt/fcntl.h */
#define _O_RDONLY      0
#define _O_WRONLY      1
#define _O_RDWR        2
#define _O_ACCMODE     (_O_RDONLY|_O_WRONLY|_O_RDWR)
#define _O_APPEND      0x0008
#define _O_RANDOM      0x0010
#define _O_SEQUENTIAL  0x0020
#define _O_TEMPORARY   0x0040
#define _O_NOINHERIT   0x0080
#define _O_CREAT       0x0100
#define _O_TRUNC       0x0200
#define _O_EXCL        0x0400
#define _O_SHORT_LIVED 0x1000
#define _O_TEXT        0x4000
#define _O_BINARY      0x8000

struct strbuf
{
    WCHAR *buf;
    DWORD pos;
    DWORD len;
};

struct installer_tempdir
{
    struct list entry;
    WCHAR *path;
};

struct installer_state
{
    BOOL norestart;
    BOOL quiet;
    struct list tempdirs;
    struct list assemblies;
    struct list updates;
};

#if defined(__x86_64__)
static const WCHAR x86W[] = {'x','8','6',0};
#elif defined(__i386__)
static const WCHAR amd64W[] = {'a','m','d','6','4',0};
#endif

static BOOL strbuf_init(struct strbuf *buf)
{
    buf->pos = 0;
    buf->len = 64;
    buf->buf = heap_alloc(buf->len * sizeof(WCHAR));
    return buf->buf != NULL;
}

static void strbuf_free(struct strbuf *buf)
{
    heap_free(buf->buf);
    buf->buf = NULL;
}

static BOOL strbuf_append(struct strbuf *buf, const WCHAR *str, DWORD len)
{
    DWORD new_len;
    WCHAR *new_buf;

    if (!buf->buf) return FALSE;
    if (!str) return TRUE;

    if (len == ~0U) len = strlenW(str);
    if (buf->pos + len + 1 > buf->len)
    {
        new_len = max(buf->pos + len + 1, buf->len * 2);
        new_buf = heap_realloc(buf->buf, new_len * sizeof(WCHAR));
        if (!new_buf)
        {
            strbuf_free(buf);
            return FALSE;
        }
        buf->buf = new_buf;
        buf->len = new_len;
    }

    memcpy(&buf->buf[buf->pos], str, len * sizeof(WCHAR));
    buf->buf[buf->pos + len] = 0;
    buf->pos += len;
    return TRUE;
}

static BOOL str_starts_with(const WCHAR *str, const WCHAR *prefix)
{
    DWORD str_len = strlenW(str), prefix_len = strlenW(prefix);
    if (prefix_len > str_len) return FALSE;
    return !strncmpiW(str, prefix, prefix_len);
}

static BOOL str_ends_with(const WCHAR *str, const WCHAR *suffix)
{
    DWORD str_len = strlenW(str), suffix_len = strlenW(suffix);
    if (suffix_len > str_len) return FALSE;
    return !strcmpiW(str + str_len - suffix_len, suffix);
}

static WCHAR *path_combine(const WCHAR *path, const WCHAR *filename)
{
    static const WCHAR backslashW[] = {'\\',0};
    WCHAR *result;
    DWORD length;

    if (!path || !filename) return NULL;
    length = strlenW(path) + strlenW(filename) + 2;
    if (!(result = heap_alloc(length * sizeof(WCHAR)))) return NULL;

    strcpyW(result, path);
    if (result[0] && result[strlenW(result) - 1] != '\\')
        strcatW(result, backslashW);
    strcatW(result, filename);
    return result;
}

static BOOL is_directory(const WCHAR *path)
{
    DWORD attrs = GetFileAttributesW(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) return FALSE;
    return (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

static BOOL create_directory(const WCHAR *path)
{
    if (is_directory(path)) return TRUE;
    if (CreateDirectoryW(path, NULL)) return TRUE;
    return (GetLastError() == ERROR_ALREADY_EXISTS);
}

static BOOL create_parent_directory(const WCHAR *filename)
{
    WCHAR *p, *path = strdupW(filename);
    BOOL ret = FALSE;

    if (!path) return FALSE;
    if (!PathRemoveFileSpecW(path)) goto done;
    if (is_directory(path))
    {
        ret = TRUE;
        goto done;
    }

    for (p = path; *p; p++)
    {
        if (*p != '\\') continue;
        *p = 0;
        if (!create_directory(path)) goto done;
        *p = '\\';
    }
    ret = create_directory(path);

done:
    heap_free(path);
    return ret;
}

static BOOL delete_directory(const WCHAR *path)
{
    static const WCHAR starW[] = {'*',0};
    static const WCHAR dotW[] = {'.', 0};
    static const WCHAR dotdotW[] = {'.','.', 0};
    WIN32_FIND_DATAW data;
    WCHAR *full_path;
    HANDLE search;

    if (!(full_path = path_combine(path, starW))) return FALSE;
    search = FindFirstFileW(full_path, &data);
    heap_free(full_path);

    if (search != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!strcmpW(data.cFileName, dotW)) continue;
            if (!strcmpW(data.cFileName, dotdotW)) continue;
            if (!(full_path = path_combine(path, data.cFileName))) continue;
            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                delete_directory(full_path);
            else
                DeleteFileW(full_path);
            heap_free(full_path);
        }
        while (FindNextFileW(search, &data));
        FindClose(search);
    }

    return RemoveDirectoryW(path);
}

static WCHAR *get_uncompressed_path(PFDINOTIFICATION pfdin)
{
    WCHAR *file = strdupAtoW(pfdin->psz1);
    WCHAR *path = path_combine(pfdin->pv, file);
    heap_free(file);
    return path;
}

static void * CDECL cabinet_alloc(ULONG cb)
{
    return heap_alloc(cb);
}

static void CDECL cabinet_free(void *pv)
{
    heap_free(pv);
}

static INT_PTR CDECL cabinet_open(char *pszFile, int oflag, int pmode)
{
    DWORD dwAccess = 0;
    DWORD dwShareMode = 0;
    DWORD dwCreateDisposition = OPEN_EXISTING;

    switch (oflag & _O_ACCMODE)
    {
    case _O_RDONLY:
        dwAccess = GENERIC_READ;
        dwShareMode = FILE_SHARE_READ | FILE_SHARE_DELETE;
        break;
    case _O_WRONLY:
        dwAccess = GENERIC_WRITE;
        dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
        break;
    case _O_RDWR:
        dwAccess = GENERIC_READ | GENERIC_WRITE;
        dwShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
        break;
    }

    if ((oflag & (_O_CREAT | _O_EXCL)) == (_O_CREAT | _O_EXCL))
        dwCreateDisposition = CREATE_NEW;
    else if (oflag & _O_CREAT)
        dwCreateDisposition = CREATE_ALWAYS;

    return (INT_PTR)CreateFileA(pszFile, dwAccess, dwShareMode, NULL,
                                dwCreateDisposition, 0, NULL);
}

static UINT CDECL cabinet_read(INT_PTR hf, void *pv, UINT cb)
{
    HANDLE handle = (HANDLE)hf;
    DWORD read;

    if (ReadFile(handle, pv, cb, &read, NULL))
        return read;

    return 0;
}

static UINT CDECL cabinet_write(INT_PTR hf, void *pv, UINT cb)
{
    HANDLE handle = (HANDLE)hf;
    DWORD written;

    if (WriteFile(handle, pv, cb, &written, NULL))
        return written;

    return 0;
}

static int CDECL cabinet_close(INT_PTR hf)
{
    HANDLE handle = (HANDLE)hf;

    return CloseHandle(handle) ? 0 : -1;
}

static LONG CDECL cabinet_seek(INT_PTR hf, LONG dist, int seektype)
{
    HANDLE handle = (HANDLE)hf;
    /* flags are compatible and so are passed straight through */
    return SetFilePointer(handle, dist, NULL, seektype);
}

static INT_PTR cabinet_copy_file(FDINOTIFICATIONTYPE fdint,
                                 PFDINOTIFICATION pfdin)
{
    HANDLE handle = INVALID_HANDLE_VALUE;
    WCHAR *file;
    DWORD attrs;

    if (!(file = get_uncompressed_path(pfdin)))
        return -1;

    TRACE("extracting %s -> %s\n", debugstr_a(pfdin->psz1), debugstr_w(file));

    if (create_parent_directory(file))
    {
        attrs = pfdin->attribs;
        if (!attrs) attrs = FILE_ATTRIBUTE_NORMAL;
        handle = CreateFileW(file, GENERIC_READ | GENERIC_WRITE, 0,
                             NULL, CREATE_ALWAYS, attrs, NULL);
    }

    heap_free(file);
    return (handle != INVALID_HANDLE_VALUE) ? (INT_PTR)handle : -1;
}

static INT_PTR cabinet_close_file_info(FDINOTIFICATIONTYPE fdint,
                                       PFDINOTIFICATION pfdin)
{
    HANDLE handle = (HANDLE)pfdin->hf;
    CloseHandle(handle);
    return 1;
}

static INT_PTR CDECL cabinet_notify(FDINOTIFICATIONTYPE fdint, PFDINOTIFICATION pfdin)
{
    switch (fdint)
    {
    case fdintPARTIAL_FILE:
        FIXME("fdintPARTIAL_FILE not implemented\n");
        return 0;

    case fdintNEXT_CABINET:
        FIXME("fdintNEXT_CABINET not implemented\n");
        return 0;

    case fdintCOPY_FILE:
        return cabinet_copy_file(fdint, pfdin);

    case fdintCLOSE_FILE_INFO:
        return cabinet_close_file_info(fdint, pfdin);

    default:
        return 0;
    }
}

static BOOL extract_cabinet(const WCHAR *filename, const WCHAR *destination)
{
    char *filenameA = NULL;
    BOOL ret = FALSE;
    HFDI hfdi;
    ERF erf;

    hfdi = FDICreate(cabinet_alloc, cabinet_free, cabinet_open, cabinet_read,
                     cabinet_write, cabinet_close, cabinet_seek, 0, &erf);
    if (!hfdi) return FALSE;

    if ((filenameA = strdupWtoA(filename)))
    {
        ret = FDICopy(hfdi, filenameA, NULL, 0, cabinet_notify, NULL, (void *)destination);
        heap_free(filenameA);
    }

    FDIDestroy(hfdi);
    return ret;
}

static WCHAR *lookup_expression(struct assembly_entry *assembly, const WCHAR *key)
{
    static const WCHAR runtime_system32[] = {'r','u','n','t','i','m','e','.','s','y','s','t','e','m','3','2',0};
    static const WCHAR runtime_windows[] = {'r','u','n','t','i','m','e','.','w','i','n','d','o','w','s',0};
    WCHAR path[MAX_PATH];

    if (!strcmpW(key, runtime_system32))
    {
    #if defined(__x86_64__)
        if (!strcmpW(assembly->identity.architecture, x86W))
        {
            GetSystemWow64DirectoryW(path, sizeof(path)/sizeof(path[0]));
            return strdupW(path);
        }
    #endif

        GetSystemDirectoryW(path, sizeof(path)/sizeof(path[0]));
        return strdupW(path);
    }
    if (!strcmpW(key, runtime_windows))
    {
        GetWindowsDirectoryW(path, sizeof(path)/sizeof(path[0]));
        return strdupW(path);
    }

    WINE_FIXME("Unknown expression %s\n", debugstr_w(key));
    return NULL;
}

static WCHAR *expand_expression(struct assembly_entry *assembly, const WCHAR *expression)
{
    static const WCHAR beginW[] = {'$','(',0};
    static const WCHAR endW[] = {')',0};
    const WCHAR *pos, *next;
    WCHAR *key, *value;
    struct strbuf buf;

    if (!expression || !strbuf_init(&buf)) return NULL;

    for (pos = expression; (next = strstrW(pos, beginW)); pos = next + 1)
    {
        strbuf_append(&buf, pos, next - pos);
        pos = next + 2;
        if (!(next = strstrW(pos, endW)))
        {
            strbuf_append(&buf, beginW, 2);
            break;
        }

        if (!(key = strdupWn(pos, next - pos))) goto error;
        value = lookup_expression(assembly, key);
        heap_free(key);
        if (!value) goto error;
        strbuf_append(&buf, value, ~0U);
        heap_free(value);
    }

    strbuf_append(&buf, pos, ~0U);
    return buf.buf;

error:
    WINE_FIXME("Couldn't resolve expression %s\n", debugstr_w(expression));
    strbuf_free(&buf);
    return NULL;
}

static WCHAR *get_assembly_source(struct assembly_entry *assembly)
{
    WCHAR *p, *path = strdupW(assembly->filename);
    if (path && (p = strrchrW(path, '.'))) *p = 0;
    return path;
}

static BOOL install_files_copy(struct assembly_entry *assembly, const WCHAR *source_path, struct fileop_entry *fileop, BOOL dryrun)
{
    WCHAR *target_path, *target, *source = NULL;
    BOOL ret = FALSE;

    if (!(target_path = expand_expression(assembly, fileop->target))) return FALSE;
    if (!(target = path_combine(target_path, fileop->source))) goto error;
    if (!(source = path_combine(source_path, fileop->source))) goto error;

    if (dryrun)
    {
        if (!(ret = PathFileExistsW(source)))
        {
            WINE_ERR("Required file %s not found\n", debugstr_w(source));
            goto error;
        }
    }
    else
    {
        WINE_TRACE("Copying %s -> %s\n", debugstr_w(source), debugstr_w(target));

        if (!create_parent_directory(target))
        {
            WINE_ERR("Failed to create parent directory for %s\n", debugstr_w(target));
            goto error;
        }
        if (!(ret = CopyFileExW(source, target, NULL, NULL, NULL, 0)))
        {
            WINE_ERR("Failed to copy %s to %s\n", debugstr_w(source), debugstr_w(target));
            goto error;
        }
    }

error:
    heap_free(target_path);
    heap_free(target);
    heap_free(source);
    return ret;
}

static BOOL install_files(struct assembly_entry *assembly, BOOL dryrun)
{
    struct fileop_entry *fileop;
    WCHAR *source_path;
    BOOL ret = TRUE;

    if (!(source_path = get_assembly_source(assembly)))
    {
        WINE_ERR("Failed to get assembly source directory\n");
        return FALSE;
    }

    LIST_FOR_EACH_ENTRY(fileop, &assembly->fileops, struct fileop_entry, entry)
    {
        if (!(ret = install_files_copy(assembly, source_path, fileop, dryrun))) break;
    }

    heap_free(source_path);
    return ret;
}

static WCHAR *split_registry_key(WCHAR *key, HKEY *root)
{
    static const WCHAR hkey_classes_rootW[] = {'H','K','E','Y','_','C','L','A','S','S','E','S','_','R','O','O','T',0};
    static const WCHAR hkey_current_configW[] = {'H','K','E','Y','_','C','U','R','R','E','N','T','_','C','O','N','F','I','G',0};
    static const WCHAR hkey_current_userW[] = {'H','K','E','Y','_','C','U','R','R','E','N','T','_','U','S','E','R',0};
    static const WCHAR hkey_local_machineW[] = {'H','K','E','Y','_','L','O','C','A','L','_','M','A','C','H','I','N','E',0};
    static const WCHAR hkey_usersW[] = {'H','K','E','Y','_','U','S','E','R','S',0};

    DWORD size;
    WCHAR *p;

    p = strchrW(key, '\\');
    if (!p) return NULL;

    size = p - key;

    if (strlenW(hkey_classes_rootW) == size && !strncmpW(key, hkey_classes_rootW, size))
        *root = HKEY_CLASSES_ROOT;
    else if (strlenW(hkey_current_configW) == size && !strncmpW(key, hkey_current_configW, size))
        *root = HKEY_CURRENT_CONFIG;
    else if (strlenW(hkey_current_userW) == size && !strncmpW(key, hkey_current_userW, size))
        *root = HKEY_CURRENT_USER;
    else if (strlenW(hkey_local_machineW) == size && !strncmpW(key, hkey_local_machineW, size))
        *root = HKEY_LOCAL_MACHINE;
    else if (strlenW(hkey_usersW) == size && !strncmpW(key, hkey_usersW, size))
        *root = HKEY_USERS;
    else
    {
        WINE_FIXME("Unknown root key %s\n", debugstr_wn(key, size));
        return NULL;
    }

    return p + 1;
}

static BOOL install_registry_string(struct assembly_entry *assembly, HKEY key, struct registrykv_entry *registrykv, DWORD type, BOOL dryrun)
{
    DWORD value_size;
    WCHAR *value = expand_expression(assembly, registrykv->value);
    BOOL ret = TRUE;

    if (registrykv->value && !value)
        return FALSE;

    value_size = value ? (strlenW(value) + 1) * sizeof(WCHAR) : 0;
    if (!dryrun && RegSetValueExW(key, registrykv->name, 0, type, (void *)value, value_size))
    {
        WINE_ERR("Failed to set registry key %s\n", debugstr_w(registrykv->name));
        ret = FALSE;
    }

    heap_free(value);
    return ret;
}

static WCHAR *parse_multisz(const WCHAR *input, DWORD *size)
{
    static const WCHAR quoteW[] = {'"',0};
    static const WCHAR emptyW[] = {0};
    const WCHAR *pos, *next;
    struct strbuf buf;

    *size = 0;
    if (!input || !input[0] || !strbuf_init(&buf)) return NULL;

    for (pos = input; pos[0] == '"'; pos++)
    {
        pos++;
        if (!(next = strstrW(pos, quoteW))) goto error;
        strbuf_append(&buf, pos, next - pos);
        strbuf_append(&buf, emptyW, sizeof(emptyW)/sizeof(emptyW[0]));

        pos = next + 1;
        if (!pos[0]) break;
        if (pos[0] != ',')
        {
            WINE_FIXME("Error while parsing REG_MULTI_SZ string: Expected comma but got '%c'\n", pos[0]);
            goto error;
        }
    }

    if (pos[0])
    {
        WINE_FIXME("Error while parsing REG_MULTI_SZ string: Garbage at end of string\n");
        goto error;
    }

    strbuf_append(&buf, emptyW, sizeof(emptyW)/sizeof(emptyW[0]));
    *size = buf.pos * sizeof(WCHAR);
    return buf.buf;

error:
    strbuf_free(&buf);
    return NULL;
}

static BOOL install_registry_multisz(struct assembly_entry *assembly, HKEY key, struct registrykv_entry *registrykv, BOOL dryrun)
{
    DWORD value_size;
    WCHAR *value = parse_multisz(registrykv->value, &value_size);
    BOOL ret = TRUE;

    if (registrykv->value && registrykv->value[0] && !value)
        return FALSE;

    if (!dryrun && RegSetValueExW(key, registrykv->name, 0, REG_MULTI_SZ, (void *)value, value_size))
    {
        WINE_ERR("Failed to set registry key %s\n", debugstr_w(registrykv->name));
        ret = FALSE;
    }

    heap_free(value);
    return ret;
}

static BOOL install_registry_dword(struct assembly_entry *assembly, HKEY key, struct registrykv_entry *registrykv, BOOL dryrun)
{
    DWORD value = registrykv->value_type ? strtoulW(registrykv->value_type, NULL, 16) : 0;
    BOOL ret = TRUE;

    if (!dryrun && RegSetValueExW(key, registrykv->name, 0, REG_DWORD, (void *)&value, sizeof(value)))
    {
        WINE_ERR("Failed to set registry key %s\n", debugstr_w(registrykv->name));
        ret = FALSE;
    }

    return ret;
}

static BYTE *parse_hex(const WCHAR *input, DWORD *size)
{
    WCHAR number[3] = {0, 0, 0};
    BYTE *output, *p;
    int length;

    *size = 0;
    if (!input) return NULL;
    length = strlenW(input);
    if (length & 1) return NULL;
    length >>= 1;

    if (!(output = heap_alloc(length))) return NULL;
    for (p = output; *input; input += 2)
    {
        number[0] = input[0];
        number[1] = input[1];
        *p++ = strtoulW(number, 0, 16);
    }
    *size = length;
    return output;
}

static BOOL install_registry_binary(struct assembly_entry *assembly, HKEY key, struct registrykv_entry *registrykv, BOOL dryrun)
{
    DWORD value_size;
    BYTE *value = parse_hex(registrykv->value, &value_size);
    BOOL ret = TRUE;

    if (registrykv->value && !value)
        return FALSE;

    if (!dryrun && RegSetValueExW(key, registrykv->name, 0, REG_BINARY, value, value_size))
    {
        WINE_ERR("Failed to set registry key %s\n", debugstr_w(registrykv->name));
        ret = FALSE;
    }

    heap_free(value);
    return ret;
}

static BOOL install_registry_value(struct assembly_entry *assembly, HKEY key, struct registrykv_entry *registrykv, BOOL dryrun)
{
    static const WCHAR reg_szW[] = {'R','E','G','_','S','Z',0};
    static const WCHAR reg_expand_szW[] = {'R','E','G','_','E','X','P','A','N','D','_','S','Z',0};
    static const WCHAR reg_multi_szW[] = {'R','E','G','_','M','U','L','T','I','_','S','Z',0};
    static const WCHAR reg_dwordW[] = {'R','E','G','_','D','W','O','R','D',0};
    static const WCHAR reg_binaryW[] = {'R','E','G','_','B','I','N','A','R','Y',0};

    WINE_TRACE("Setting registry key %s = %s\n", debugstr_w(registrykv->name), debugstr_w(registrykv->value));

    if (!strcmpW(registrykv->value_type, reg_szW))
        return install_registry_string(assembly, key, registrykv, REG_SZ, dryrun);
    if (!strcmpW(registrykv->value_type, reg_expand_szW))
        return install_registry_string(assembly, key, registrykv, REG_EXPAND_SZ, dryrun);
    if (!strcmpW(registrykv->value_type, reg_multi_szW))
        return install_registry_multisz(assembly, key, registrykv, dryrun);
    if (!strcmpW(registrykv->value_type, reg_dwordW))
        return install_registry_dword(assembly, key, registrykv, dryrun);
    if (!strcmpW(registrykv->value_type, reg_binaryW))
        return install_registry_binary(assembly, key, registrykv, dryrun);

    WINE_FIXME("Unsupported registry value type %s\n", debugstr_w(registrykv->value_type));
    return FALSE;
}

static BOOL install_registry(struct assembly_entry *assembly, BOOL dryrun)
{
    struct registryop_entry *registryop;
    struct registrykv_entry *registrykv;
    HKEY root, subkey;
    WCHAR *path;
    REGSAM sam = KEY_ALL_ACCESS;
    BOOL ret = TRUE;

#if defined(__x86_64__)
    if (!strcmpW(assembly->identity.architecture, x86W)) sam |= KEY_WOW64_32KEY;
#endif

    LIST_FOR_EACH_ENTRY(registryop, &assembly->registryops, struct registryop_entry, entry)
    {
        if (!(path = split_registry_key(registryop->key, &root)))
        {
            ret = FALSE;
            break;
        }

        WINE_TRACE("Processing registry key %s\n", debugstr_w(registryop->key));

        if (!dryrun && RegCreateKeyExW(root, path, 0, NULL, REG_OPTION_NON_VOLATILE, sam, NULL, &subkey, NULL))
        {
            WINE_ERR("Failed to open registry key %s\n", debugstr_w(registryop->key));
            ret = FALSE;
            break;
        }

        LIST_FOR_EACH_ENTRY(registrykv, &registryop->keyvalues, struct registrykv_entry, entry)
        {
            if (!(ret = install_registry_value(assembly, subkey, registrykv, dryrun))) break;
        }

        if (!dryrun) RegCloseKey(subkey);
        if (!ret) break;
    }

    return ret;
}

static BOOL compare_assembly_string(const WCHAR *str1, const WCHAR *str2)
{
    static const WCHAR placeholderW[] = {'*',0};
    return !strcmpW(str1, str2) || !strcmpW(str1, placeholderW) || !strcmpW(str2, placeholderW);
}

static struct assembly_entry *lookup_assembly(struct list *manifest_list, struct assembly_identity *identity)
{
    struct assembly_entry *assembly;

    LIST_FOR_EACH_ENTRY(assembly, manifest_list, struct assembly_entry, entry)
    {
        if (strcmpiW(assembly->identity.name, identity->name)) continue;
        if (!compare_assembly_string(assembly->identity.architecture, identity->architecture)) continue;
        if (!compare_assembly_string(assembly->identity.language, identity->language)) continue;
        if (!compare_assembly_string(assembly->identity.pubkey_token, identity->pubkey_token)) continue;
        if (!compare_assembly_string(assembly->identity.version, identity->version))
        {
            WINE_WARN("Ignoring version difference for %s (expected %s, found %s)\n",
                      debugstr_w(identity->name), debugstr_w(identity->version), debugstr_w(assembly->identity.version));
        }
        return assembly;
    }

    return NULL;
}

static BOOL install_assembly(struct list *manifest_list, struct assembly_identity *identity, BOOL dryrun)
{
    struct dependency_entry *dependency;
    struct assembly_entry *assembly;
    const WCHAR *name;

    if (!(assembly = lookup_assembly(manifest_list, identity)))
    {
        WINE_FIXME("Assembly %s not found\n", debugstr_w(identity->name));
        return FALSE;
    }

    name = assembly->identity.name;

    if (assembly->status == ASSEMBLY_STATUS_INSTALLED)
    {
        WINE_TRACE("Assembly %s already installed\n", debugstr_w(name));
        return TRUE;
    }
    if (assembly->status == ASSEMBLY_STATUS_IN_PROGRESS)
    {
        WINE_ERR("Assembly %s caused circular dependency\n", debugstr_w(name));
        return FALSE;
    }

#if defined(__i386__)
    if (!strcmpW(assembly->identity.architecture, amd64W))
    {
        WINE_ERR("Can not install amd64 assembly in 32 bit prefix\n");
        return FALSE;
    }
#endif

    assembly->status = ASSEMBLY_STATUS_IN_PROGRESS;

    LIST_FOR_EACH_ENTRY(dependency, &assembly->dependencies, struct dependency_entry, entry)
    {
        if (!install_assembly(manifest_list, &dependency->identity, dryrun)) return FALSE;
    }

    WINE_TRACE("Installing assembly %s%s\n", debugstr_w(name), dryrun ? " (dryrun)" : "");

    if (!install_files(assembly, dryrun))
    {
        WINE_ERR("Failed to install all files for %s\n", debugstr_w(name));
        return FALSE;
    }

    if (!install_registry(assembly, dryrun))
    {
        WINE_ERR("Failed to install registry keys for %s\n", debugstr_w(name));
        return FALSE;
    }

    WINE_TRACE("Installation of %s finished\n", debugstr_w(name));

    assembly->status = ASSEMBLY_STATUS_INSTALLED;
    return TRUE;
}

static const WCHAR *create_temp_directory(struct installer_state *state)
{
    static const WCHAR msuW[] = {'m','s','u',0};
    static UINT id;
    struct installer_tempdir *entry;
    WCHAR tmp[MAX_PATH];

    if (!GetTempPathW(sizeof(tmp)/sizeof(WCHAR), tmp)) return NULL;
    if (!(entry = heap_alloc(sizeof(*entry)))) return NULL;
    if (!(entry->path = heap_alloc((MAX_PATH + 20) * sizeof(WCHAR))))
    {
        heap_free(entry);
        return NULL;
    }
    for (;;)
    {
        if (!GetTempFileNameW(tmp, msuW, ++id, entry->path))
        {
            heap_free(entry->path);
            heap_free(entry);
            return NULL;
        }
        if (CreateDirectoryW(entry->path, NULL)) break;
    }

    list_add_tail(&state->tempdirs, &entry->entry);
    return entry->path;
}

static void installer_cleanup(struct installer_state *state)
{
    struct installer_tempdir *tempdir, *tempdir2;
    struct assembly_entry *assembly, *assembly2;
    struct dependency_entry *dependency, *dependency2;

    LIST_FOR_EACH_ENTRY_SAFE(tempdir, tempdir2, &state->tempdirs, struct installer_tempdir, entry)
    {
        list_remove(&tempdir->entry);
        delete_directory(tempdir->path);
        heap_free(tempdir->path);
        heap_free(tempdir);
    }
    LIST_FOR_EACH_ENTRY_SAFE(assembly, assembly2, &state->assemblies, struct assembly_entry, entry)
    {
        list_remove(&assembly->entry);
        free_assembly(assembly);
    }
    LIST_FOR_EACH_ENTRY_SAFE(dependency, dependency2, &state->updates, struct dependency_entry, entry)
    {
        list_remove(&dependency->entry);
        free_dependency(dependency);
    }
}

static BOOL load_assemblies_from_cab(const WCHAR *filename, struct installer_state *state)
{
    static const WCHAR manifest_cix_xmlW[] = {'_','m','a','n','i','f','e','s','t','_','.','c','i','x','.','x','m','l',0};
    static const WCHAR manifestW[] = {'.','m','a','n','i','f','e','s','t',0};
    static const WCHAR mumW[] = {'.','m','u','m',0};
    static const WCHAR starW[] = {'*',0};
    struct assembly_entry *assembly;
    const WCHAR *temp_path;
    WIN32_FIND_DATAW data;
    HANDLE search;
    WCHAR *path;

    WINE_TRACE("Processing cab file %s\n", debugstr_w(filename));

    if (!(temp_path = create_temp_directory(state))) return FALSE;
    if (!extract_cabinet(filename, temp_path))
    {
        WINE_ERR("Failed to extract %s\n", debugstr_w(filename));
        return FALSE;
    }

    if (!(path = path_combine(temp_path, manifest_cix_xmlW))) return FALSE;
    if (GetFileAttributesW(path) != INVALID_FILE_ATTRIBUTES)
    {
        WINE_FIXME("Cabinet uses proprietary msdelta file compression which is not (yet) supported.\n");
        WINE_FIXME("Installation of msu file will most likely fail.\n");
    }
    heap_free(path);

    if (!(path = path_combine(temp_path, starW))) return FALSE;
    search = FindFirstFileW(path, &data);
    heap_free(path);

    if (search != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
            if (!str_ends_with(data.cFileName, manifestW) &&
                !str_ends_with(data.cFileName, mumW)) continue;
            if (!(path = path_combine(temp_path, data.cFileName))) continue;
            if ((assembly = load_manifest(path)))
                list_add_tail(&state->assemblies, &assembly->entry);
            heap_free(path);
        }
        while (FindNextFileW(search, &data));
        FindClose(search);
    }

    return TRUE;
}

static BOOL install_updates(struct installer_state *state, BOOL dryrun)
{
    struct dependency_entry *dependency;
    LIST_FOR_EACH_ENTRY(dependency, &state->updates, struct dependency_entry, entry)
    {
        if (!install_assembly(&state->assemblies, &dependency->identity, dryrun))
        {
            WINE_ERR("Failed to install update %s\n", debugstr_w(dependency->identity.name));
            return FALSE;
        }
    }
    return TRUE;
}

static void set_assembly_status(struct list *manifest_list, DWORD status)
{
    struct assembly_entry *assembly;
    LIST_FOR_EACH_ENTRY(assembly, manifest_list, struct assembly_entry, entry)
    {
        assembly->status = status;
    }
}

static BOOL install_msu(WCHAR *filename, struct installer_state *state)
{
    static const WCHAR wsusscanW[] = {'W','S','U','S','S','C','A','N','.','c','a','b',0};
    static const WCHAR cabW[] = {'*','.','c','a','b',0};
    static const WCHAR xmlW[] = {'*','.','x','m','l',0};
    const WCHAR *temp_path;
    WIN32_FIND_DATAW data;
    HANDLE search;
    WCHAR *path;
    BOOL ret = FALSE;

    list_init(&state->tempdirs);
    list_init(&state->assemblies);
    list_init(&state->updates);
    CoInitialize(NULL);

    WINE_TRACE("Processing msu file %s\n", debugstr_w(filename));

    if (!(temp_path = create_temp_directory(state))) return 1;
    if (!extract_cabinet(filename, temp_path))
    {
        WINE_ERR("Failed to extract %s\n", debugstr_w(filename));
        goto done;
    }

    /* load all manifests from contained cabinet archives */
    if (!(path = path_combine(temp_path, cabW))) goto done;
    search = FindFirstFileW(path, &data);
    heap_free(path);

    if (search != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
            if (!strcmpiW(data.cFileName, wsusscanW)) continue;
            if (!(path = path_combine(temp_path, data.cFileName))) continue;
            if (!load_assemblies_from_cab(path, state))
                WINE_ERR("Failed to load all manifests from %s, ignoring\n", debugstr_w(path));
            heap_free(path);
        }
        while (FindNextFileW(search, &data));
        FindClose(search);
    }

    /* load all update descriptions */
    if (!(path = path_combine(temp_path, xmlW))) goto done;
    search = FindFirstFileW(path, &data);
    heap_free(path);

    if (search != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
            if (!(path = path_combine(temp_path, data.cFileName))) continue;
            if (!load_update(path, &state->updates))
                WINE_ERR("Failed to load all updates from %s, ignoring\n", debugstr_w(path));
            heap_free(path);
        }
        while (FindNextFileW(search, &data));
        FindClose(search);
    }

    /* Windows Vista MSU files do not contain an xml file - what is the correct way to get
     * the update list? For now just install all assemblies starting with "Package_for_KB". */
    if (list_empty(&state->updates))
    {
        static const WCHAR package_for_kbW[] = {'P','a','c','k','a','g','e','_','f','o','r','_','K','B',0};
        struct assembly_entry *assembly;
        LIST_FOR_EACH_ENTRY(assembly, &state->assemblies, struct assembly_entry, entry)
        {
            if (!assembly->identity.name) continue;
            if (str_starts_with(assembly->identity.name, package_for_kbW))
                queue_update(assembly, &state->updates);
        }
    }

    /* dump package information (for debugging) */
    if (WINE_TRACE_ON(wusa))
    {
        struct dependency_entry *dependency;
        struct assembly_entry *assembly;

        WINE_TRACE("List of updates:\n");
        LIST_FOR_EACH_ENTRY(dependency, &state->updates, struct dependency_entry, entry)
            WINE_TRACE(" * %s\n", debugstr_w(dependency->identity.name));

        WINE_TRACE("List of manifests (with dependencies):\n");
        LIST_FOR_EACH_ENTRY(assembly, &state->assemblies, struct assembly_entry, entry)
        {
            WINE_TRACE(" * %s\n", debugstr_w(assembly->identity.name));
            LIST_FOR_EACH_ENTRY(dependency, &assembly->dependencies, struct dependency_entry, entry)
                WINE_TRACE("   -> %s\n", debugstr_w(dependency->identity.name));
        }
    }

    if (list_empty(&state->updates))
    {
        WINE_ERR("No updates found, probably incompatible MSU file format?\n");
        goto done;
    }

    /* perform dryrun */
    set_assembly_status(&state->assemblies, ASSEMBLY_STATUS_NONE);
    if (!install_updates(state, TRUE))
    {
        WINE_ERR("Dryrun failed, aborting installation\n");
        goto done;
    }

    /* installation */
    set_assembly_status(&state->assemblies, ASSEMBLY_STATUS_NONE);
    if (!install_updates(state, FALSE))
    {
        WINE_ERR("Installation failed\n");
        goto done;
    }

    WINE_TRACE("Installation finished\n");
    ret = TRUE;

done:
    installer_cleanup(state);
    return ret;
}

static void restart_as_x86_64(void)
{
    WCHAR filename[MAX_PATH];
    PROCESS_INFORMATION pi;
    STARTUPINFOW si;
    DWORD exit_code = 1;
    BOOL is_wow64;
    void *redir;

    if (!IsWow64Process(GetCurrentProcess(), &is_wow64) || !is_wow64)
        return;

    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    GetModuleFileNameW(0, filename, MAX_PATH);

    Wow64DisableWow64FsRedirection(&redir);
    if (CreateProcessW(filename, GetCommandLineW(), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
    {
        WINE_TRACE("Restarting %s\n", wine_dbgstr_w(filename));
        WaitForSingleObject(pi.hProcess, INFINITE);
        GetExitCodeProcess(pi.hProcess, &exit_code);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
    else WINE_ERR("Failed to restart 64-bit %s, err %d\n", wine_dbgstr_w(filename), GetLastError());
    Wow64RevertWow64FsRedirection(redir);

    ExitProcess(exit_code);
}

int wmain(int argc, WCHAR *argv[])
{
    static const WCHAR norestartW[] = {'/','n','o','r','e','s','t','a','r','t',0};
    static const WCHAR quietW[] = {'/','q','u','i','e','t',0};
    struct installer_state state;
    WCHAR *filename = NULL;
    int i;

    restart_as_x86_64();

    state.norestart = FALSE;
    state.quiet = FALSE;

    if (TRACE_ON(wusa))
    {
        WINE_TRACE("Command line:");
        for (i = 0; i < argc; i++)
            WINE_TRACE(" %s", wine_dbgstr_w(argv[i]));
        WINE_TRACE("\n");
    }

    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '/')
        {
            if (!strcmpW(argv[i], norestartW))
                state.norestart = TRUE;
            else if (!strcmpW(argv[i], quietW))
                state.quiet = TRUE;
            else
                WINE_FIXME("Unknown option: %s\n", wine_dbgstr_w(argv[i]));
        }
        else if (!filename)
            filename = argv[i];
        else
            WINE_FIXME("Unknown option: %s\n", wine_dbgstr_w(argv[i]));
    }

    if (!filename)
    {
        WINE_FIXME("Missing filename argument\n");
        return 1;
    }

    return !install_msu(filename, &state);
}
