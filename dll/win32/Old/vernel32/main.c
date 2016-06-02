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
 
#define NDEBUG

#include <main.h>
#include <collation.h>
#include <csr.h>

WINE_DEFAULT_DEBUG_CHANNEL(bcrypt);

locale LocaleList[3];

void WINAPI initTable()
{
	LocaleList[0].description = L"English - United States"; LocaleList[0].code = L"en-US"; LocaleList[0].lcidHex = 0x0409; LocaleList[0].lcidDec = 1033;	
	LocaleList[1].description = L"Portuguese - Brazil"; LocaleList[1].code = L"pt-BR"; LocaleList[1].lcidHex = 0x0416; LocaleList[1].lcidDec = 1046;
	LocaleList[2].description = L"Chinese"; LocaleList[2].code = L"zh-CN"; LocaleList[2].lcidHex = 0x0804; LocaleList[2].lcidDec = 2052;	
}

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
	switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
			//initTable();
            DisableThreadLibraryCalls(hInstDLL);
            break;
    }

    return TRUE;
}