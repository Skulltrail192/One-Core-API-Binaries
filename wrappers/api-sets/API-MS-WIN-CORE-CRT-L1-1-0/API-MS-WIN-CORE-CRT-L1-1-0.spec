@ cdecl __isascii(long) msvcrt.__isascii
@ cdecl -ret64 _atoi64(str) msvcrt._atoi64
@ cdecl _errno() msvcrt._errno
@ cdecl -arch=i386 -version=0x600+ _except_handler4_common(ptr ptr ptr ptr ptr ptr) msvcrt._except_handler4_common
@ stub _fltused
@ cdecl -arch=i386 -ret64 _ftol() msvcrt._ftol
@ cdecl -arch=i386 -ret64 _ftol2() msvcrt._ftol2
@ cdecl -arch=i386 -ret64 _ftol2_sse() msvcrt._ftol2_sse
@ cdecl -version=0x600+ _i64tow_s(int64 ptr long long) msvcrt._i64tow_s
@ cdecl -version=0x600+ _itow_s(long ptr long long) msvcrt._itow_s
@ cdecl -version=0x600+ -arch=i386 _local_unwind4(ptr ptr long) msvcrt._local_unwind4
@ cdecl -version=0x600+ _ltow_s(long ptr long long) msvcrt._ltow_s
@ varargs -version=0x600+ _snprintf_s(ptr long long str) msvcrt._snprintf_s
@ varargs -version=0x600+ _snwprintf_s(ptr long long wstr) msvcrt._snwprintf_s
@ cdecl -version=0x600+ _splitpath_s(str ptr long ptr long ptr long ptr long) msvcrt._splitpath_s
@ cdecl _stricmp(str str) msvcrt._stricmp
@ cdecl -version=0x600+ _strlwr_s(ptr long) msvcrt._strlwr_s
@ cdecl _strnicmp(str str long) msvcrt._strnicmp
@ cdecl -version=0x600+ _strupr_s(str long) msvcrt._strupr_s
@ cdecl -version=0x600+ _ui64tow_s(int64 ptr long long) msvcrt._ui64tow_s
@ cdecl _ultow(long ptr long) msvcrt._ultow
@ cdecl -version=0x600+ _ultow_s(long ptr long long) msvcrt._ultow_s
@ cdecl -version=0x600+ _vsnprintf_s(ptr long long str ptr) msvcrt._vsnprintf_s
@ cdecl -version=0x600+ _vsnwprintf_s(ptr long long wstr ptr) msvcrt._vsnwprintf_s
@ cdecl _wcsicmp(wstr wstr) msvcrt._wcsicmp
@ cdecl -version=0x600+ _wcslwr_s(wstr long) msvcrt._wcslwr_s
@ cdecl _wcsnicmp(wstr wstr long) msvcrt._wcsnicmp
@ cdecl -ret64 _wcstoi64(wstr ptr long) msvcrt._wcstoi64
@ cdecl -ret64 _wcstoui64(wstr ptr long) msvcrt._wcstoui64
@ cdecl -version=0x600+ _wcsupr_s(wstr long) msvcrt._wcsupr_s
@ cdecl -version=0x600+ _wsplitpath_s(wstr ptr long ptr long ptr long ptr long) msvcrt._wsplitpath_s
@ cdecl _wtoi(wstr) msvcrt._wtoi
@ cdecl -ret64 _wtoi64(wstr) msvcrt._wtoi64
@ cdecl _wtol(wstr) msvcrt._wtol
@ cdecl atoi(str) msvcrt.atoi
@ cdecl atol(str) msvcrt.atol
@ cdecl isalnum(long) msvcrt.isalnum
@ cdecl isdigit(long) msvcrt.isdigit
@ cdecl isgraph(long) msvcrt.isgraph
@ cdecl islower(long) msvcrt.islower
@ cdecl isprint(long) msvcrt.isprint
@ cdecl isspace(long) msvcrt.isspace
@ cdecl isupper(long) msvcrt.isupper
@ cdecl iswalnum(long) msvcrt.iswalnum
@ cdecl iswascii(long) msvcrt.iswascii
@ cdecl iswctype(long long) msvcrt.iswctype
@ cdecl iswdigit(long) msvcrt.iswdigit
@ cdecl iswgraph(long) msvcrt.iswgraph
@ cdecl iswprint(long) msvcrt.iswprint
@ cdecl iswspace(long) msvcrt.iswspace
@ cdecl memcmp(ptr ptr long) msvcrt.memcmp
@ cdecl memcpy(ptr ptr long) msvcrt.memcpy
@ cdecl -version=0x600+ memcpy_s(ptr long ptr long) msvcrt.memcpy_s
@ cdecl memmove(ptr ptr long) msvcrt.memmove
@ cdecl -version=0x600+ memmove_s(ptr long ptr long) msvcrt.memmove_s
@ cdecl memset(ptr long long) msvcrt.memset
@ cdecl -version=0x600+ qsort_s(ptr long long ptr ptr) msvcrt.qsort_s
@ varargs -version=0x600+ sprintf_s(ptr long str) msvcrt.sprintf_s
@ varargs -version=0x600+ sscanf_s(str str) msvcrt.sscanf_s
@ cdecl -version=0x600+ strcat_s(str long str) msvcrt.strcat_s
@ cdecl strchr(str long) msvcrt.strchr
@ cdecl strcmp(str str) msvcrt.strcmp
@ cdecl -version=0x600+ strcpy_s(ptr long str) msvcrt.strcpy_s
@ cdecl strcspn(str str) msvcrt.strcspn
@ cdecl strlen(str) msvcrt.strlen
@ cdecl -version=0x600+ strncat_s(str long str long) msvcrt.strncat_s
@ cdecl strncmp(str str long) msvcrt.strncmp
@ cdecl -version=0x600+ strncpy_s(ptr long str long) msvcrt.strncpy_s
@ cdecl -version=0x600+ strnlen(str long) msvcrt.strnlen
@ cdecl strpbrk(str str) msvcrt.strpbrk
@ cdecl strrchr(str long) msvcrt.strrchr
@ cdecl strstr(str str) msvcrt.strstr
@ cdecl -version=0x600+ strtok_s(ptr str ptr) msvcrt.strtok_s
@ cdecl strtol(str ptr long) msvcrt.strtol
@ cdecl strtoul(str ptr long) msvcrt.strtoul
@ varargs -version=0x600+ swprintf_s(ptr long wstr) msvcrt.swprintf_s
@ cdecl tolower(long) msvcrt.tolower
@ cdecl toupper(long) msvcrt.toupper
@ cdecl towlower(long) msvcrt.towlower
@ cdecl towupper(long) msvcrt.towupper
@ cdecl -version=0x600+ vsprintf_s(ptr long str ptr) msvcrt.vsprintf_s
@ cdecl -version=0x600+ vswprintf_s(ptr long wstr ptr) msvcrt.vswprintf_s
@ cdecl -version=0x600+ wcscat_s(wstr long wstr) msvcrt.wcscat_s
@ cdecl wcschr(wstr long) msvcrt.wcschr
@ cdecl wcscmp(wstr wstr) msvcrt.wcscmp
@ cdecl -version=0x600+ wcscpy_s(ptr long wstr) msvcrt.wcscpy_s
@ cdecl wcscspn(wstr wstr) msvcrt.wcscspn
@ cdecl wcslen(wstr) msvcrt.wcslen
@ cdecl -version=0x600+ wcsncat_s(wstr long wstr long) msvcrt.wcsncat_s
@ cdecl wcsncmp(wstr wstr long) msvcrt.wcsncmp
@ cdecl -version=0x600+ wcsncpy_s(ptr long wstr long) msvcrt.wcsncpy_s
@ cdecl -version=0x600+ wcsnlen(wstr long) msvcrt.wcsnlen
@ cdecl wcspbrk(wstr wstr) msvcrt.wcspbrk
@ cdecl wcsrchr(wstr long) msvcrt.wcsrchr
@ cdecl wcsstr(wstr wstr) msvcrt.wcsstr
@ cdecl -version=0x600+ wcstok_s(ptr wstr ptr) msvcrt.wcstok_s
@ cdecl wcstol(wstr ptr long) msvcrt.wcstol
@ cdecl wcstoul(wstr ptr long) msvcrt.wcstoul
