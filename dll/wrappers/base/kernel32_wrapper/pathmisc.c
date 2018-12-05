/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    pathmisc.c

Abstract:

    Win32 misceleneous path functions

Author:

    Skulltrail 25-June-2017

Revision History:

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(kernel32file);

HMODULE hshlwapi;

static LPWSTR (WINAPI *pPathCombineW)(LPWSTR ,LPCWSTR, LPCWSTR);

static HRESULT (WINAPI *pPathCreateFromUrlW)(PCWSTR, PWSTR, DWORD, DWORD);

#define FS_VOLUME_BUFFER_SIZE (MAX_PATH * sizeof(WCHAR) + sizeof(FILE_FS_VOLUME_INFORMATION))

#define FS_ATTRIBUTE_BUFFER_SIZE (MAX_PATH * sizeof(WCHAR) + sizeof(FILE_FS_ATTRIBUTE_INFORMATION))

/*
 * @implemented
 */
BOOL
WINAPI
GetVolumeInformationByHandleW(
	IN HANDLE hFile,
    IN LPWSTR lpVolumeNameBuffer,
    IN DWORD nVolumeNameSize,
    OUT LPDWORD lpVolumeSerialNumber OPTIONAL,
    OUT LPDWORD lpMaximumComponentLength OPTIONAL,
    OUT LPDWORD lpFileSystemFlags OPTIONAL,
    OUT LPWSTR lpFileSystemNameBuffer OPTIONAL,
    IN DWORD nFileSystemNameSize)
{
  PFILE_FS_VOLUME_INFORMATION FileFsVolume;
  PFILE_FS_ATTRIBUTE_INFORMATION FileFsAttribute;
  IO_STATUS_BLOCK IoStatusBlock;
  UCHAR Buffer[max(FS_VOLUME_BUFFER_SIZE, FS_ATTRIBUTE_BUFFER_SIZE)];

  NTSTATUS errCode;

  FileFsVolume = (PFILE_FS_VOLUME_INFORMATION)Buffer;
  FileFsAttribute = (PFILE_FS_ATTRIBUTE_INFORMATION)Buffer;

  TRACE("FileFsVolume %p\n", FileFsVolume);
  TRACE("FileFsAttribute %p\n", FileFsAttribute);

  if (hFile == INVALID_HANDLE_VALUE)
    {
      return FALSE;
    }

  TRACE("hFile: %p\n", hFile);
  errCode = NtQueryVolumeInformationFile(hFile,
                                         &IoStatusBlock,
                                         FileFsVolume,
                                         FS_VOLUME_BUFFER_SIZE,
                                         FileFsVolumeInformation);
  if ( !NT_SUCCESS(errCode) )
    {
      WARN("Status: %x\n", errCode);
      CloseHandle(hFile);
      BaseSetLastNTError (errCode);
      return FALSE;
    }

  if (lpVolumeSerialNumber)
    *lpVolumeSerialNumber = FileFsVolume->VolumeSerialNumber;

  if (lpVolumeNameBuffer)
    {
      if (nVolumeNameSize * sizeof(WCHAR) >= FileFsVolume->VolumeLabelLength + sizeof(WCHAR))
        {
	  memcpy(lpVolumeNameBuffer,
		 FileFsVolume->VolumeLabel,
		 FileFsVolume->VolumeLabelLength);
	  lpVolumeNameBuffer[FileFsVolume->VolumeLabelLength / sizeof(WCHAR)] = 0;
	}
      else
        {
	  CloseHandle(hFile);
	  SetLastError(ERROR_MORE_DATA);
	  return FALSE;
	}
    }

  errCode = NtQueryVolumeInformationFile (hFile,
	                                  &IoStatusBlock,
	                                  FileFsAttribute,
	                                  FS_ATTRIBUTE_BUFFER_SIZE,
	                                  FileFsAttributeInformation);
  CloseHandle(hFile);
  if (!NT_SUCCESS(errCode))
    {
      WARN("Status: %x\n", errCode);
      BaseSetLastNTError (errCode);
      return FALSE;
    }

  if (lpFileSystemFlags)
    *lpFileSystemFlags = FileFsAttribute->FileSystemAttributes;
  if (lpMaximumComponentLength)
    *lpMaximumComponentLength = FileFsAttribute->MaximumComponentNameLength;
  if (lpFileSystemNameBuffer)
    {
      if (nFileSystemNameSize * sizeof(WCHAR) >= FileFsAttribute->FileSystemNameLength + sizeof(WCHAR))
        {
	  memcpy(lpFileSystemNameBuffer,
		 FileFsAttribute->FileSystemName,
		 FileFsAttribute->FileSystemNameLength);
	  lpFileSystemNameBuffer[FileFsAttribute->FileSystemNameLength / sizeof(WCHAR)] = 0;
	}
      else
        {
	  SetLastError(ERROR_MORE_DATA);
	  return FALSE;
	}
    }
  return TRUE;
}

HRESULT WINAPI PathCchAddBackslash(WCHAR *path, SIZE_T size)
{
    return PathCchAddBackslashEx(path, size, NULL, NULL);
}

HRESULT WINAPI PathCchAddBackslashEx(WCHAR *path, SIZE_T size, WCHAR **endptr, SIZE_T *remaining)
{
    BOOL needs_termination;
    SIZE_T length;

    TRACE("%s, %lu, %p, %p\n", debugstr_w(path), size, endptr, remaining);

    length = strlenW(path);
    needs_termination = size && length && path[length - 1] != '\\';

    if (length >= (needs_termination ? size - 1 : size))
    {
        if (endptr) *endptr = NULL;
        if (remaining) *remaining = 0;
        return STRSAFE_E_INSUFFICIENT_BUFFER;
    }

    if (!needs_termination)
    {
        if (endptr) *endptr = path + length;
        if (remaining) *remaining = size - length;
        return S_FALSE;
    }

    path[length++] = '\\';
    path[length] = 0;

    if (endptr) *endptr = path + length;
    if (remaining) *remaining = size - length;

    return S_OK;
}

/***********************************************************************
 *          PathCchCombineEx (KERNELBASE.@)
 */
HRESULT WINAPI PathCchCombineEx(WCHAR *out, SIZE_T size, const WCHAR *path1, const WCHAR *path2, DWORD flags)
{
    WCHAR result[MAX_PATH];
	hshlwapi = GetModuleHandleA("shlwapi.dll");
	
    FIXME("(%p, %lu, %s, %s, %x): semi-stub\n", out, size, wine_dbgstr_w(path1), wine_dbgstr_w(path2), flags);

    if (!out || !size) return E_INVALIDARG;
    if (flags) FIXME("Flags %x not supported\n", flags);
	
	
	pPathCombineW = (void *)GetProcAddress(hshlwapi, "PathCombineW");
	if(pPathCombineW){
		if(!pPathCombineW(result, path1, path2))
			return E_INVALIDARG;
	}else{
		SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
		return FALSE;
	}	

    if (strlenW(result) + 1 > size)
    {
        out[0] = 0;
        return STRSAFE_E_INSUFFICIENT_BUFFER;
    }

    strcpyW(out, result);
    return S_OK;
}

/***********************************************************************
 *          PathCchCombineEx (KERNELBASE.@)
 */
HRESULT WINAPI PathCchCombine(WCHAR *out, SIZE_T size, const WCHAR *path1, const WCHAR *path2){
	return PathCchCombineEx(out, size, path1, path2, 0);
}

HRESULT WINAPI PathCchFindExtension(const WCHAR *path, SIZE_T size, const WCHAR **extension)
{
    const WCHAR *lastpoint = NULL;
    SIZE_T counter = 0;

    TRACE("%s %lu %p\n", wine_dbgstr_w(path), size, extension);

    if (!path || !size || size > PATHCCH_MAX_CCH)
    {
        *extension = NULL;
        return E_INVALIDARG;
    }

    while (*path)
    {
        if (*path == '\\' || *path == ' ')
            lastpoint = NULL;
        else if (*path == '.')
            lastpoint = path;

        path++;
        counter++;
        if (counter == size || counter == PATHCCH_MAX_CCH)
        {
            *extension = NULL;
            return E_INVALIDARG;
        }
    }

    *extension = lastpoint ? lastpoint : path;
    return S_OK;
} 

HRESULT WINAPI PathCchAddExtension(WCHAR *path, SIZE_T size, const WCHAR *extension)
{
    const WCHAR *existing_extension, *next;
    SIZE_T path_length, extension_length, dot_length;
    BOOL has_dot;
    HRESULT hr;

    TRACE("%s %lu %s\n", wine_dbgstr_w(path), size, wine_dbgstr_w(extension));

    if (!path || !size || size > PATHCCH_MAX_CCH || !extension) return E_INVALIDARG;

    next = extension;
    while (*next)
    {
        if ((*next == '.' && next > extension) || *next == ' ' || *next == '\\') return E_INVALIDARG;
        next++;
    }

    has_dot = extension[0] == '.' ? TRUE : FALSE;

    hr = PathCchFindExtension(path, size, &existing_extension);
    if (FAILED(hr)) return hr;
    if (*existing_extension) return S_FALSE;

    //path_length = wcsnlen(path, size);
    path_length = wcslen(path);
    dot_length = has_dot ? 0 : 1;
    extension_length = strlenW(extension);

    if (path_length + dot_length + extension_length + 1 > size) return STRSAFE_E_INSUFFICIENT_BUFFER;

    /* If extension is empty or only dot, return S_OK with path unchanged */
    if (!extension[0] || (extension[0] == '.' && !extension[1])) return S_OK;

    if (!has_dot)
    {
        path[path_length] = '.';
        path_length++;
    }

    strcpyW(path + path_length, extension);
    return S_OK;
} 

HRESULT WINAPI PathCchRemoveExtension(WCHAR *path, SIZE_T size)
{
    const WCHAR *extension;
    WCHAR *next;
    HRESULT hr;

    TRACE("%s %lu\n", wine_dbgstr_w(path), size);

    if (!path || !size || size > PATHCCH_MAX_CCH) return E_INVALIDARG;

    hr = PathCchFindExtension(path, size, &extension);
    if (FAILED(hr)) return hr;

    next = path + (extension - path);
    while (next - path < size && *next) *next++ = 0;

    return next == extension ? S_FALSE : S_OK;
}

HRESULT WINAPI PathCchRenameExtension(WCHAR *path, SIZE_T size, const WCHAR *extension)
{
    HRESULT hr;

    TRACE("%s %lu %s\n", wine_dbgstr_w(path), size, wine_dbgstr_w(extension));

    hr = PathCchRemoveExtension(path, size);
    if (FAILED(hr)) return hr;

    hr = PathCchAddExtension(path, size, extension);
    return FAILED(hr) ? hr : S_OK;
}  

HRESULT WINAPI PathCchRemoveBackslashEx(WCHAR *path, SIZE_T size, WCHAR **endptr, SIZE_T *remaining)
{
    BOOL needs_trim;
    SIZE_T length;

    TRACE("%s, %lu, %p, %p\n", debugstr_w(path), size, endptr, remaining);

    if (!path) return E_INVALIDARG;
    length = strlenW(path);
    needs_trim = size && length && path[length - 1] == '\\';

    if (needs_trim && (length > 1) && path[length - 2] == ':')
        needs_trim = 0;

    if (needs_trim)
    {
        path[length - 1] = 0;
        --length;
    }

    if (endptr) *endptr = path + length;
    if (remaining) *remaining = size - length;

    return needs_trim ? S_OK : S_FALSE;
}

HRESULT WINAPI PathCchRemoveBackslash(WCHAR *path, SIZE_T size)
{
    return PathCchRemoveBackslashEx(path, size, NULL, NULL);
}

HRESULT WINAPI PathCchAppendEx(WCHAR *path1, SIZE_T size, const WCHAR *path2, DWORD flags)
{
    HRESULT hr;
    WCHAR *result;
    TRACE("%s %lu %s %#x\n", wine_dbgstr_w(path1), size, wine_dbgstr_w(path2), flags);
    if (!path1 || !size) return E_INVALIDARG;
    /* Create a temporary buffer for result because we need to keep path1 unchanged if error occurs.
     * And PathCchCombineEx writes empty result if there is error so we can't just use path1 as output
     * buffer for PathCchCombineEx */
    result = HeapAlloc(GetProcessHeap(), 0, size * sizeof(WCHAR));
    if (!result) return E_OUTOFMEMORY;
    /* Avoid the single backslash behavior with PathCchCombineEx when appending */
    if (path2 && path2[0] == '\\' && path2[1] != '\\') path2++;
    hr = PathCchCombineEx(result, size, path1, path2, flags);
    if (SUCCEEDED(hr)) memcpy(path1, result, size * sizeof(WCHAR));
    HeapFree(GetProcessHeap(), 0, result);
    return hr;
}

HRESULT WINAPI PathCchAppend(WCHAR *path1, SIZE_T size, const WCHAR *path2)
{
    TRACE("%s %lu %s\n", wine_dbgstr_w(path1), size, wine_dbgstr_w(path2));
    return PathCchAppendEx(path1, size, path2, PATHCCH_NONE);
}

// HRESULT WINAPI PathAllocCanonicalize(const WCHAR *path_in, DWORD flags, WCHAR **path_out)
// {
    // WCHAR *buffer, *dst;
    // const WCHAR *src;
    // const WCHAR *root_end;
    // SIZE_T buffer_size, length;

    // TRACE("%s %#x %p\n", debugstr_w(path_in), flags, path_out);

    // if (!path_in || !path_out
        // || ((flags & PATHCCH_FORCE_ENABLE_LONG_NAME_PROCESS) && (flags & PATHCCH_FORCE_DISABLE_LONG_NAME_PROCESS))
        // || (flags & (PATHCCH_FORCE_ENABLE_LONG_NAME_PROCESS | PATHCCH_FORCE_DISABLE_LONG_NAME_PROCESS)
            // && !(flags & PATHCCH_ALLOW_LONG_PATHS))
        // || ((flags & PATHCCH_ENSURE_IS_EXTENDED_LENGTH_PATH) && (flags & PATHCCH_ALLOW_LONG_PATHS)))
    // {
        // if (path_out) *path_out = NULL;
        // return E_INVALIDARG;
    // }

    // length = strlenW(path_in);
    // if ((length + 1 > MAX_PATH && !(flags & (PATHCCH_ALLOW_LONG_PATHS | PATHCCH_ENSURE_IS_EXTENDED_LENGTH_PATH)))
        // || (length + 1 > PATHCCH_MAX_CCH))
    // {
        // *path_out = NULL;
        // return HRESULT_FROM_WIN32(ERROR_FILENAME_EXCED_RANGE);
    // }

    // /* PATHCCH_ENSURE_IS_EXTENDED_LENGTH_PATH implies PATHCCH_DO_NOT_NORMALIZE_SEGMENTS */
    // if (flags & PATHCCH_ENSURE_IS_EXTENDED_LENGTH_PATH) flags |= PATHCCH_DO_NOT_NORMALIZE_SEGMENTS;

    // /* path length + possible \\?\ addition + possible \ addition + NUL */
    // buffer_size = (length + 6) * sizeof(WCHAR);
    // buffer = LocalAlloc(LMEM_ZEROINIT, buffer_size);
    // if (!buffer)
    // {
        // *path_out = NULL;
        // return E_OUTOFMEMORY;
    // }

    // src = path_in;
    // dst = buffer;

    // root_end = get_root_end(path_in);
    // if (root_end) root_end = buffer + (root_end - path_in);

    // /* Copy path root */
    // if (root_end)
    // {
        // memcpy(dst, src, (root_end - buffer + 1) * sizeof(WCHAR));
        // src += root_end - buffer + 1;
        // if(PathCchStripPrefix(dst, length + 6) == S_OK)
        // {
            // /* Fill in \ in X:\ if the \ is missing */
            // if(isalphaW(dst[0]) && dst[1] == ':' && dst[2]!= '\\')
            // {
                // dst[2] = '\\';
                // dst[3] = 0;
            // }
            // dst = buffer + strlenW(buffer);
            // root_end = dst;
        // }
        // else
            // dst += root_end - buffer + 1;
    // }

    // while (*src)
    // {
        // if (src[0] == '.')
        // {
            // if (src[1] == '.')
            // {
                // /* Keep one . after * */
                // if (dst > buffer && dst[-1] == '*')
                // {
                    // *dst++ = *src++;
                    // continue;
                // }

                // /* Keep the . if one of the following is true:
                 // * 1. PATHCCH_DO_NOT_NORMALIZE_SEGMENTS
                 // * 2. in form of a..b
                 // */
                // if (dst > buffer
                    // && (((flags & PATHCCH_DO_NOT_NORMALIZE_SEGMENTS) && dst[-1] != '\\')
                        // || (dst[-1] != '\\' && src[2] != '\\' && src[2])))
                // {
                    // *dst++ = *src++;
                    // *dst++ = *src++;
                    // continue;
                // }

                // /* Remove the \ before .. if the \ is not part of root */
                // if (dst > buffer && dst[-1] == '\\' && (!root_end || dst - 1 > root_end))
                // {
                    // *--dst = '\0';
                    // /* Remove characters until a \ is encountered */
                    // while (dst > buffer)
                    // {
                        // if (dst[-1] == '\\')
                        // {
                            // *--dst = 0;
                            // break;
                        // }
                        // else
                            // *--dst = 0;
                    // }
                // }
                // /* Remove the extra \ after .. if the \ before .. wasn't deleted */
                // else if (src[2] == '\\')
                    // src++;

                // src += 2;
            // }
            // else
            // {
                // /* Keep the . if one of the following is true:
                 // * 1. PATHCCH_DO_NOT_NORMALIZE_SEGMENTS
                 // * 2. in form of a.b, which is used in domain names
                 // * 3. *.
                 // */
                // if (dst > buffer
                    // && ((flags & PATHCCH_DO_NOT_NORMALIZE_SEGMENTS && dst[-1] != '\\')
                        // || (dst[-1] != '\\' && src[1] != '\\' && src[1]) || (dst[-1] == '*')))
                // {
                    // *dst++ = *src++;
                    // continue;
                // }

                // /* Remove the \ before . if the \ is not part of root */
                // if (dst > buffer && dst[-1] == '\\' && (!root_end || dst - 1 > root_end)) dst--;
                // /* Remove the extra \ after . if the \ before . wasn't deleted */
                // else if (src[1] == '\\')
                    // src++;

                // src++;
            // }

            // /* If X:\ is not complete, then complete it */
            // if (isalphaW(buffer[0]) && buffer[1] == ':' && buffer[2] != '\\')
            // {
                // root_end = buffer + 2;
                // dst = buffer + 3;
                // buffer[2] = '\\';
                // /* If next character is \, use the \ to fill in */
                // if (src[0] == '\\') src++;
            // }
        // }
        // /* Copy over */
        // else
            // *dst++ = *src++;
    // }
    // /* End the path */
    // *dst = 0;

    // /* If result path is empty, fill in \ */
    // if (!*buffer)
    // {
        // buffer[0] = '\\';
        // buffer[1] = 0;
    // }

    // /* Extend the path if needed */
    // length = strlenW(buffer);
    // if (((length + 1 > MAX_PATH && isalphaW(buffer[0]) && buffer[1] == ':')
         // || (isalphaW(buffer[0]) && buffer[1] == ':' && flags & PATHCCH_ENSURE_IS_EXTENDED_LENGTH_PATH))
        // && !(flags & PATHCCH_FORCE_ENABLE_LONG_NAME_PROCESS))
    // {
        // memmove(buffer + 4, buffer, (length + 1) * sizeof(WCHAR));
        // buffer[0] = '\\';
        // buffer[1] = '\\';
        // buffer[2] = '?';
        // buffer[3] = '\\';
    // }

    // /* Add a trailing backslash to the path if needed */
    // if (flags & PATHCCH_ENSURE_TRAILING_SLASH)
        // PathCchAddBackslash(buffer, buffer_size);

    // *path_out = buffer;
    // return S_OK;
// } 

// HRESULT WINAPI PathCchCanonicalizeEx(WCHAR *out, SIZE_T size, const WCHAR *in, DWORD flags)
// {
    // WCHAR *buffer;
    // SIZE_T length;
    // HRESULT hr;

    // TRACE("%p %lu %s %#x\n", out, size, wine_dbgstr_w(in), flags);

    // if (!size) return E_INVALIDARG;

    // hr = PathAllocCanonicalize(in, flags, &buffer);
    // if (FAILED(hr)) return hr;

    // length = strlenW(buffer);
    // if (size < length + 1)
    // {
        // /* No root and path > MAX_PATH - 4, return HRESULT_FROM_WIN32(ERROR_FILENAME_EXCED_RANGE) */
        // if (length > MAX_PATH - 4 && !(in[0] == '\\' || (isalphaW(in[0]) && in[1] == ':' && in[2] == '\\')))
            // hr = HRESULT_FROM_WIN32(ERROR_FILENAME_EXCED_RANGE);
        // else
            // hr = STRSAFE_E_INSUFFICIENT_BUFFER;
    // }

    // if (SUCCEEDED(hr))
    // {
        // memcpy(out, buffer, (length + 1) * sizeof(WCHAR));

        // /* Fill a backslash at the end of X: */
        // if (isalphaW(out[0]) && out[1] == ':' && !out[2] && size > 3)
        // {
            // out[2] = '\\';
            // out[3] = 0;
        // }
    // }

    // LocalFree(buffer);
    // return hr;
// } 
// BOOL WINAPI PathCchIsRoot(const WCHAR *path)
// {
    // const WCHAR *root_end;
    // const WCHAR *next;
    // BOOL is_unc;

    // TRACE("%s\n", wine_dbgstr_w(path));

    // if (!path || !*path) return FALSE;

    // root_end = get_root_end(path);
    // if (!root_end) return FALSE;

    // if ((is_unc = is_prefixed_unc(path)) || (path[0] == '\\' && path[1] == '\\' && path[2] != '?'))
    // {
        // next = root_end + 1;
        // /* No extra segments */
        // if ((is_unc && !*next) || (!is_unc && !*next)) return TRUE;

        // /* Has first segment with an ending backslash but no remaining characters */
        // if (get_next_segment(next, &next) && !*next) return FALSE;
        // /* Has first segment with no ending backslash */
        // else if (!*next)
            // return TRUE;
        // /* Has first segment with an ending backslash and has remaining characters*/
        // else
        // {
            // next++;
            // /* Second segment must have no backslash and no remaining characters */
            // return !get_next_segment(next, &next) && !*next;
        // }
    // }
    // else if (*root_end == '\\' && !root_end[1])
        // return TRUE;
    // else
        // return FALSE;
// }

// HRESULT WINAPI PathCchStripToRoot(WCHAR *path, SIZE_T size)
// {
    // const WCHAR *root_end;
    // WCHAR *segment_end;
    // BOOL is_unc;

    // TRACE("%s %lu\n", wine_dbgstr_w(path), size);

    // if (!path || !*path || !size || size > PATHCCH_MAX_CCH) return E_INVALIDARG;

    // /* \\\\?\\UNC\\* and \\\\* have to have at least two extra segments to be striped,
     // * e.g. \\\\?\\UNC\\a\\b\\c -> \\\\?\\UNC\\a\\b
     // *      \\\\a\\b\\c         -> \\\\a\\b         */
    // if ((is_unc = is_prefixed_unc(path)) || (path[0] == '\\' && path[1] == '\\' && path[2] != '?'))
    // {
        // root_end = is_unc ? path + 8 : path + 3;
        // if (!get_next_segment(root_end, &root_end)) return S_FALSE;
        // if (!get_next_segment(root_end, &root_end)) return S_FALSE;

        // if (root_end - path >= size) return E_INVALIDARG;

        // segment_end = path + (root_end - path) - 1;
        // *segment_end = 0;
        // return S_OK;
    // }
    // else if (PathCchSkipRoot(path, &root_end) == S_OK)
    // {
        // if (root_end - path >= size) return E_INVALIDARG;

        // segment_end = path + (root_end - path);
        // if (!*segment_end) return S_FALSE;

        // *segment_end = 0;
        // return S_OK;
    // }
    // else
        // return E_INVALIDARG;
// }

// HRESULT WINAPI PathCchSkipRoot(const WCHAR *path, const WCHAR **root_end)
// {
    // static const WCHAR unc_prefix[] = {'\\', '\\', '?'};

    // TRACE("%s %p\n", debugstr_w(path), root_end);

    // if (!path || !path[0] || !root_end
        // || (!memicmpW(unc_prefix, path, ARRAY_SIZE(unc_prefix)) && !is_prefixed_volume(path) && !is_prefixed_unc(path)
            // && !is_prefixed_disk(path)))
        // return E_INVALIDARG;

    // *root_end = get_root_end(path);
    // if (*root_end)
    // {
        // (*root_end)++;
        // if (is_prefixed_unc(path))
        // {
            // get_next_segment(*root_end, root_end);
            // get_next_segment(*root_end, root_end);
        // }
        // else if (path[0] == '\\' && path[1] == '\\' && path[2] != '?')
        // {
            // /* Skip share server */
            // get_next_segment(*root_end, root_end);
            // /* If mount point is empty, don't skip over mount point */
            // if (**root_end != '\\') get_next_segment(*root_end, root_end);
        // }
    // }

    // return *root_end ? S_OK : E_INVALIDARG;
// }

// HRESULT WINAPI PathCchStripPrefix(WCHAR *path, SIZE_T size)
// {
    // TRACE("%s %lu\n", wine_dbgstr_w(path), size);

    // if (!path || !size || size > PATHCCH_MAX_CCH) return E_INVALIDARG;

    // if (is_prefixed_unc(path))
    // {
        // /* \\?\UNC\a -> \\a */
        // if (size < strlenW(path + 8) + 3) return E_INVALIDARG;
        // strcpyW(path + 2, path + 8);
        // return S_OK;
    // }
    // else if (is_prefixed_disk(path))
    // {
        // /* \\?\C:\ -> C:\ */
        // if (size < strlenW(path + 4) + 1) return E_INVALIDARG;
        // strcpyW(path, path + 4);
        // return S_OK;
    // }
    // else
        // return S_FALSE;
// }

// BOOL WINAPI PathIsUNCEx(const WCHAR *path, const WCHAR **server)
// {
    // const WCHAR *result = NULL;

    // TRACE("%s %p\n", wine_dbgstr_w(path), server);

    // if (is_prefixed_unc(path))
        // result = path + 8;
    // else if (path[0] == '\\' && path[1] == '\\' && path[2] != '?')
        // result = path + 2;

    // if (server) *server = result;
    // return result ? TRUE : FALSE;
// }

/*************************************************************************
 * StrCpyNW	[SHLWAPI.@]
 *
 * Copy a string to another string, up to a maximum number of characters.
 *
 * PARAMS
 *  dst    [O] Destination string
 *  src    [I] Source string
 *  count  [I] Maximum number of chars to copy
 *
 * RETURNS
 *  dst.
 */
LPWSTR WINAPI StrCpyNW(LPWSTR dst, LPCWSTR src, int count)
{
  LPWSTR d = dst;
  LPCWSTR s = src;

  TRACE("(%p,%s,%i)\n", dst, debugstr_w(src), count);

  if (s)
  {
    while ((count > 1) && *s)
    {
      count--;
      *d++ = *s++;
    }
  }
  if (count) *d = 0;

  return dst;
}

/*************************************************************************
 * StrCpyW	[SHLWAPI.@]
 *
 * Copy a string to another string.
 *
 * PARAMS
 *  lpszStr [O] Destination string
 *  lpszSrc [I] Source string
 *
 * RETURNS
 *  lpszStr.
 */
LPWSTR WINAPI StrCpyW(LPWSTR lpszStr, LPCWSTR lpszSrc)
{
  TRACE("(%p,%s)\n", lpszStr, debugstr_w(lpszSrc));

  if (lpszStr && lpszSrc)
    strcpyW(lpszStr, lpszSrc);
  return lpszStr;
}


/*************************************************************************
 *      UrlUnescapeW	[SHLWAPI.@]
 *
 * See UrlUnescapeA.
 */
HRESULT WINAPI UrlUnescapeW(
	LPWSTR pszUrl,
	LPWSTR pszUnescaped,
	LPDWORD pcchUnescaped,
	DWORD dwFlags)
{
    WCHAR *dst, next;
    LPCWSTR src;
    HRESULT ret;
    DWORD needed;
    BOOL stop_unescaping = FALSE;

    TRACE("(%s, %p, %p, 0x%08x)\n", debugstr_w(pszUrl), pszUnescaped,
	  pcchUnescaped, dwFlags);

    if(!pszUrl) return E_INVALIDARG;

    if(dwFlags & URL_UNESCAPE_INPLACE)
        dst = pszUrl;
    else
    {
        if (!pszUnescaped || !pcchUnescaped) return E_INVALIDARG;
        dst = pszUnescaped;
    }

    for(src = pszUrl, needed = 0; *src; src++, needed++) {
        if(dwFlags & URL_DONT_UNESCAPE_EXTRA_INFO &&
           (*src == '#' || *src == '?')) {
	    stop_unescaping = TRUE;
	    next = *src;
        } else if(*src == '%' && isxdigitW(*(src + 1)) && isxdigitW(*(src + 2))
		  && stop_unescaping == FALSE) {
	    INT ih;
	    WCHAR buf[5] = {'0','x',0};
	    memcpy(buf + 2, src + 1, 2*sizeof(WCHAR));
	    buf[4] = 0;
	    StrToIntExW(buf, STIF_SUPPORT_HEX, &ih);
	    next = (WCHAR) ih;
	    src += 2; /* Advance to end of escape */
	} else
	    next = *src;

	if(dwFlags & URL_UNESCAPE_INPLACE || needed < *pcchUnescaped)
	    *dst++ = next;
    }

    if(dwFlags & URL_UNESCAPE_INPLACE || needed < *pcchUnescaped) {
        *dst = '\0';
	ret = S_OK;
    } else {
        needed++; /* add one for the '\0' */
	ret = E_POINTER;
    }
    if(!(dwFlags & URL_UNESCAPE_INPLACE))
        *pcchUnescaped = needed;

    if (ret == S_OK) {
	TRACE("result %s\n", (dwFlags & URL_UNESCAPE_INPLACE) ?
	      debugstr_w(pszUrl) : debugstr_w(pszUnescaped));
    }

    return ret;
}

/*************************************************************************
 * PathCreateFromUrlW   [SHLWAPI.@]
 *
 * Create a path from a URL
 *
 * PARAMS
 *  lpszUrl  [I] URL to convert into a path
 *  lpszPath [O] Output buffer for the resulting Path
 *  pcchPath [I] Length of lpszPath
 *  dwFlags  [I] Flags controlling the conversion
 *
 * RETURNS
 *  Success: S_OK. lpszPath contains the URL in path format,
 *  Failure: An HRESULT error code such as E_INVALIDARG.
 */
HRESULT WINAPI PathCreateFromUrlW(LPCWSTR pszUrl, LPWSTR pszPath,
                                  LPDWORD pcchPath, DWORD dwReserved)
{
    static const WCHAR file_colon[] = { 'f','i','l','e',':',0 };
    static const WCHAR localhost[] = { 'l','o','c','a','l','h','o','s','t',0 };
    DWORD nslashes, unescape, len;
    const WCHAR *src;
    WCHAR *tpath, *dst;
    HRESULT ret;

    TRACE("(%s,%p,%p,0x%08x)\n", debugstr_w(pszUrl), pszPath, pcchPath, dwReserved);

    if (!pszUrl || !pszPath || !pcchPath || !*pcchPath)
        return E_INVALIDARG;

    if (lstrlenW(pszUrl) < 5)
        return E_INVALIDARG;

    if (CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE, pszUrl, 5,
                       file_colon, 5) != CSTR_EQUAL)
        return E_INVALIDARG;
    pszUrl += 5;
    ret = S_OK;

    src = pszUrl;
    nslashes = 0;
    while (*src == '/' || *src == '\\') {
        nslashes++;
        src++;
    }

    /* We need a temporary buffer so we can compute what size to ask for.
     * We know that the final string won't be longer than the current pszUrl
     * plus at most two backslashes. All the other transformations make it
     * shorter.
     */
    len = 2 + lstrlenW(pszUrl) + 1;
    if (*pcchPath < len)
        tpath = HeapAlloc(GetProcessHeap(), 0, len * sizeof(WCHAR));
    else
        tpath = pszPath;

    len = 0;
    dst = tpath;
    unescape = 1;
    switch (nslashes)
    {
    case 0:
        /* 'file:' + escaped DOS path */
        break;
    case 1:
        /* 'file:/' + escaped DOS path */
        /* fall through */
    case 3:
        /* 'file:///' (implied localhost) + escaped DOS path */
        if (!isalphaW(*src) || (src[1] != ':' && src[1] != '|'))
            src -= 1;
        break;
    case 2:
        if (lstrlenW(src) >= 10 && CompareStringW(LOCALE_INVARIANT, NORM_IGNORECASE,
            src, 9, localhost, 9) == CSTR_EQUAL && (src[9] == '/' || src[9] == '\\'))
        {
            /* 'file://localhost/' + escaped DOS path */
            src += 10;
        }
        else if (isalphaW(*src) && (src[1] == ':' || src[1] == '|'))
        {
            /* 'file://' + unescaped DOS path */
            unescape = 0;
        }
        else
        {
            /*    'file://hostname:port/path' (where path is escaped)
             * or 'file:' + escaped UNC path (\\server\share\path)
             * The second form is clearly specific to Windows and it might
             * even be doing a network lookup to try to figure it out.
             */
            while (*src && *src != '/' && *src != '\\')
                src++;
            len = src - pszUrl;
            StrCpyNW(dst, pszUrl, len + 1);
            dst += len;
            if (*src && isalphaW(src[1]) && (src[2] == ':' || src[2] == '|'))
            {
                /* 'Forget' to add a trailing '/', just like Windows */
                src++;
            }
        }
        break;
    case 4:
        /* 'file://' + unescaped UNC path (\\server\share\path) */
        unescape = 0;
        if (isalphaW(*src) && (src[1] == ':' || src[1] == '|'))
            break;
        /* fall through */
    default:
        /* 'file:/...' + escaped UNC path (\\server\share\path) */
        src -= 2;
    }

    /* Copy the remainder of the path */
    len += lstrlenW(src);
    StrCpyW(dst, src);

     /* First do the Windows-specific path conversions */
    for (dst = tpath; *dst; dst++)
        if (*dst == '/') *dst = '\\';
    if (isalphaW(*tpath) && tpath[1] == '|')
        tpath[1] = ':'; /* c| -> c: */

    /* And only then unescape the path (i.e. escaped slashes are left as is) */
    if (unescape)
    {
        ret = UrlUnescapeW(tpath, NULL, &len, URL_UNESCAPE_INPLACE);
        if (ret == S_OK)
        {
            /* When working in-place UrlUnescapeW() does not set len */
            len = lstrlenW(tpath);
        }
    }

    if (*pcchPath < len + 1)
    {
        ret = E_POINTER;
        *pcchPath = len + 1;
    }
    else
    {
        *pcchPath = len;
        if (tpath != pszPath)
            StrCpyW(pszPath, tpath);
    }
    if (tpath != pszPath)
      HeapFree(GetProcessHeap(), 0, tpath);

    TRACE("Returning (%u) %s\n", *pcchPath, debugstr_w(pszPath));
    return ret;
}

/*************************************************************************
 * PathCreateFromUrlAlloc   [SHLWAPI.@]
 */
HRESULT WINAPI PathCreateFromUrlAlloc(LPCWSTR pszUrl, LPWSTR *pszPath,
                                      DWORD dwReserved)
{
    WCHAR pathW[MAX_PATH];
    DWORD size;
    HRESULT hr;

    size = MAX_PATH;
    hr = PathCreateFromUrlW(pszUrl, pathW, &size, dwReserved);
    if (SUCCEEDED(hr))
    {
        /* Yes, this is supposed to crash if pszPath is NULL */
        *pszPath = StrDupW(pathW);
    }
    return hr;
}
