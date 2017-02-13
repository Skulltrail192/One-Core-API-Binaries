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
#include <main.h>

BOOL
GetVersionExA(
    OUT LPOSVERSIONINFOA lpVersionInformation
)
{
        lpVersionInformation->dwMajorVersion = 6;
        lpVersionInformation->dwMinorVersion = 1;
        lpVersionInformation->dwBuildNumber  = 7601;
        lpVersionInformation->dwPlatformId   = VER_PLATFORM_WIN32_NT;
        StringCbCopyExA(lpVersionInformation->szCSDVersion, 
                        ARRAYSIZE(lpVersionInformation->szCSDVersion),                        
                        "Service Pack 1", NULL , NULL, STRSAFE_NULL_ON_FAILURE);


        if( lpVersionInformation->dwOSVersionInfoSize == sizeof(OSVERSIONINFOEXA) ) 
        {
            // We are here as we are passed a OSVERSIONINFOEX structure.
            // Set the major and minor service pack numbers.
            ((LPOSVERSIONINFOEXA)lpVersionInformation)->wServicePackMajor = 1;
            ((LPOSVERSIONINFOEXA)lpVersionInformation)->wServicePackMinor = 0;
        }
    return TRUE;
}

BOOL
GetVersionExW(
    OUT LPOSVERSIONINFOW lpVersionInformation
)
{
        lpVersionInformation->dwMajorVersion = 6;
        lpVersionInformation->dwMinorVersion = 1;
        lpVersionInformation->dwBuildNumber  = 7601;
        lpVersionInformation->dwPlatformId   = VER_PLATFORM_WIN32_NT;
        StringCbCopyExW(lpVersionInformation->szCSDVersion, 
                        ARRAYSIZE(lpVersionInformation->szCSDVersion),                        
                        L"Service Pack 1", NULL , NULL, STRSAFE_NULL_ON_FAILURE);


        if( lpVersionInformation->dwOSVersionInfoSize == sizeof(OSVERSIONINFOEXW) ) 
        {
            // We are here as we are passed a OSVERSIONINFOEX structure.
            // Set the major and minor service pack numbers.
            ((LPOSVERSIONINFOEXW)lpVersionInformation)->wServicePackMajor = 1;
            ((LPOSVERSIONINFOEXW)lpVersionInformation)->wServicePackMinor = 0;
        }
    return TRUE;
}

BOOL
VerifyVersionInfoA(
    OUT LPOSVERSIONINFOEXA lpVersionInformation,
	IN DWORD dwTypeMask,
	IN DWORDLONG dwlConditionMask	
    )
{
    return TRUE;	
}

BOOL
VerifyVersionInfoW(
    OUT LPOSVERSIONINFOEXW lpVersionInformation,
	IN DWORD dwTypeMask,
	IN DWORDLONG dwlConditionMask		
    )
{
    return TRUE;	
}
