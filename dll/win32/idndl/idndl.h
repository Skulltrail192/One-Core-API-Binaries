// ==++==
//
// Copyright (c) Microsoft Corporation.  All Rights Reserved
//
// ==++==

// IdnDl.h
//
// WARNING: This .DLL is downlevel only.
//
// This file contains the downlevel versions of the scripts APIs
//
// 06 Jun 2005     Shawn Steele    Initial Implementation

#ifndef IDNDL_H_
#define IDNDL_H_

#include <crtdbg.h>
#include <windows.h>

#define VS_ALLOW_LATIN              0x0001
#define GSS_ALLOW_INHERITED_COMMON  0x0001

int WINAPI DownlevelGetLocaleScripts(
    __in                            LPCWSTR lpLocaleName,   // Locale Name
    __out_ecount_opt(cchScripts)    LPWSTR  lpScripts,      // Output buffer for scripts
    __in                            int     cchScripts      // size of output buffer
    );

int WINAPI DownlevelGetStringScripts(
        __in                         DWORD   dwFlags,        // optional behavior flags
        __in                         LPCWSTR lpString,       // Unicode character input string
        __in                         int     cchString,      // size of input string
        __out_ecount_opt(cchScripts) LPWSTR  lpScripts,      // Script list output string
        __in                         int     cchScripts      // size of output string
    );

BOOL WINAPI DownlevelVerifyScripts(
     __in    DWORD   dwFlags,            // optional behavior flags
     __in    LPCWSTR lpLocaleScripts,    // Locale list of scripts string
     __in    int     cchLocaleScripts,   // size of locale script list string
     __in    LPCWSTR lpTestScripts,      // test scripts string
     __in    int     cchTestScripts      // size of test list string
     );

#endif  // IDNDL_H_
