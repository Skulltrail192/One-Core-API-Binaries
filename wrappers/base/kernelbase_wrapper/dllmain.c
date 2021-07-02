/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    dllmain.c

Abstract:

    This module initialize Win32 API Base

Author:

    Skulltrail 20-March-2017

Revision History:

--*/


#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(dllmain); 

static BOOL DllInitialized = FALSE;
PPEB Peb;
HMODULE kernel32_handle = NULL;
ULONG BaseDllTag;

void InitializeCriticalForLocaleInfo();
void init_locale(void);

static HRESULT lcid_to_rfc1766(LCID lcid, WCHAR *rfc1766, INT len)
{
    WCHAR buffer[6 /* MAX_RFC1766_NAME */];
    INT n = GetLocaleInfoW(lcid, LOCALE_SISO639LANGNAME, buffer, ARRAY_SIZE(buffer));
    INT i;

    if (n)
    {
        i = PRIMARYLANGID(lcid);
        if ((((i == LANG_ENGLISH) || (i == LANG_CHINESE) || (i == LANG_ARABIC)) &&
            (SUBLANGID(lcid) == SUBLANG_DEFAULT)) ||
            (SUBLANGID(lcid) > SUBLANG_DEFAULT)) {

            buffer[n - 1] = '-';
            i = GetLocaleInfoW(lcid, LOCALE_SISO3166CTRYNAME, buffer + n, ARRAY_SIZE(buffer) - n);
            if (!i)
                buffer[n - 1] = '\0';
        }
        else
            i = 0;

        LCMapStringW(LOCALE_USER_DEFAULT, LCMAP_LOWERCASE, buffer, n + i, rfc1766, len);
        return ((n + i) > len) ? E_INVALIDARG : S_OK;
    }
    return E_FAIL;
}

BOOL
WINAPI
BaseDllInitialize(
	HANDLE hDll,
    DWORD dwReason,
    LPVOID lpReserved)
{
    /* Cache the PEB and Session ID */
    //Peb = NtCurrentPeb();

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            /* Insert more dll attach stuff here! */
			kernel32_handle = GetModuleHandleW(L"kernelex");
			InitializeCriticalForLocaleInfo();
			
			//Initialize Locale
			init_locale();
			
			BaseDllTag = RtlCreateTagHeap( RtlProcessHeap(),
                                       0,
                                       L"BASEDLL!",
                                       L"TMP\0"
                                       L"BACKUP\0"
                                       L"INI\0"
                                       L"FIND\0"
                                       L"GMEM\0"
                                       L"LMEM\0"
                                       L"ENV\0"
                                       L"RES\0"
                                       L"VDM\0"
                                     );			
			
            DllInitialized = TRUE;			
            break;
        }

        case DLL_PROCESS_DETACH:
        {
            if (DllInitialized == TRUE)
            {
				DllInitialized = FALSE;
            }
            break;
        }
		
        case DLL_THREAD_ATTACH:
        {

        }

        default:
            break;
    }

    return TRUE;
}

/***********************************************************************
 *          QuirkIsEnabled3 (KERNELBASE.@)
 */
BOOL WINAPI QuirkIsEnabled3(void *unk1, void *unk2)
{
    static int once;

    if (!once++)
        DbgPrint("(%p, %p) stub!\n", unk1, unk2);
	
    return FALSE;
}

HRESULT WINAPI GetAcceptLanguagesW(WCHAR *langbuf, DWORD *buflen)
{
    DWORD mystrlen, mytype;
    WCHAR *mystr;
    LCID mylcid;
    HKEY mykey;
    LONG lres;
    DWORD len;

    TRACE("%p, %p, *%p: %d\n", langbuf, buflen, buflen, buflen ? *buflen : -1);

    if (!langbuf || !buflen || !*buflen)
        return E_FAIL;

    mystrlen = (*buflen > 20) ? *buflen : 20 ;
    len = mystrlen * sizeof(WCHAR);
    mystr = RtlAllocateHeap(GetProcessHeap(), 0, len);
    mystr[0] = 0;
    RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Internet Explorer\\International",
                  0, KEY_QUERY_VALUE, &mykey);
    lres = RegQueryValueExW(mykey, L"AcceptLanguage", 0, &mytype, (PBYTE)mystr, &len);
    RegCloseKey(mykey);
    len = lstrlenW(mystr);

    if (!lres && (*buflen > len))
    {
        lstrcpyW(langbuf, mystr);
        *buflen = len;
        RtlFreeHeap(GetProcessHeap(), 0, mystr);
        return S_OK;
    }

    /* Did not find a value in the registry or the user buffer is too small */
    mylcid = GetUserDefaultLCID();
    lcid_to_rfc1766(mylcid, mystr, mystrlen);
    len = lstrlenW(mystr);

    memcpy(langbuf, mystr, min(*buflen, len + 1)*sizeof(WCHAR));
    RtlFreeHeap(GetProcessHeap(), 0, mystr);

    if (*buflen > len)
    {
        *buflen = len;
        return S_OK;
    }

    *buflen = 0;
    return E_NOT_SUFFICIENT_BUFFER;
}

HRESULT WINAPI GetAcceptLanguagesA(LPSTR langbuf, DWORD *buflen)
{
    DWORD buflenW, convlen;
    WCHAR *langbufW;
    HRESULT hr;

    TRACE("%p, %p, *%p: %d\n", langbuf, buflen, buflen, buflen ? *buflen : -1);

    if (!langbuf || !buflen || !*buflen)
        return E_FAIL;

    buflenW = *buflen;
    langbufW = RtlAllocateHeap(GetProcessHeap(), 0, sizeof(WCHAR) * buflenW);
    hr = GetAcceptLanguagesW(langbufW, &buflenW);

    if (hr == S_OK)
    {
        convlen = WideCharToMultiByte(CP_ACP, 0, langbufW, -1, langbuf, *buflen, NULL, NULL);
        convlen--;  /* do not count the terminating 0 */
    }
    else  /* copy partial string anyway */
    {
        convlen = WideCharToMultiByte(CP_ACP, 0, langbufW, *buflen, langbuf, *buflen, NULL, NULL);
        if (convlen < *buflen)
        {
            langbuf[convlen] = 0;
            convlen--;  /* do not count the terminating 0 */
        }
        else
        {
            convlen = *buflen;
        }
    }
    *buflen = buflenW ? convlen : 0;

    RtlFreeHeap(GetProcessHeap(), 0, langbufW);
    return hr;
}