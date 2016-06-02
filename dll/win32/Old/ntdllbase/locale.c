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

#include "config.h"
#include <assert.h>
#include <rtl.h>
#include <localelcid.h>
#include <winnls.h>

#define MUI_LANGUAGE_ID 1
#define MUI_LANGUAGE_NAME 2

void initTable()
{
	LocaleList[0].description = L"English - United States"; LocaleList[0].code = L"en-US"; LocaleList[0].lcidHex = 0x0409; LocaleList[0].lcidDec = 1033;	
	LocaleList[1].description = L"Portuguese - Brazil"; LocaleList[1].code = L"pt-BR"; LocaleList[1].lcidHex = 0x0416; LocaleList[1].lcidDec = 1046;
	LocaleList[2].description = L"Chinese"; LocaleList[2].code = L"zh-CN"; LocaleList[2].lcidHex = 0x0804; LocaleList[2].lcidDec = 2052;	
}

/*
 * @unimplemented - need reimplementation
 */
BOOL WINAPI RtlGetThreadPreferredUILanguages(_In_       DWORD dwFlags,
  _Out_      PULONG pulNumLanguages,
  _Out_opt_  PZZWSTR pwszLanguagesBuffer,
  _Inout_    PULONG pcchLanguagesBuffer)
{
	*pulNumLanguages = 1;
	if(dwFlags == MUI_LANGUAGE_ID)
	{
		pwszLanguagesBuffer = L"0x0409";
	}
	if(dwFlags == MUI_LANGUAGE_NAME)
	{
		pwszLanguagesBuffer = L"en-US";
	}
	return TRUE;
}

/*
 * @unimplemented - need reimplementation
 */
BOOL WINAPI RtlSetThreadPreferredUILanguages(
  _In_       DWORD dwFlags,
  _In_opt_   PCZZWSTR pwszLanguagesBuffer,
  _Out_opt_  PULONG pulNumLanguages
)
{
	*pulNumLanguages = 1;
	if(dwFlags == MUI_LANGUAGE_ID)
	{
		pwszLanguagesBuffer = L"0409";
	}
	if(dwFlags == MUI_LANGUAGE_NAME)
	{
		pwszLanguagesBuffer = L"en-US";
	}
	return TRUE;
}

/*
 * @implemented - need test

BOOLEAN NTAPI RtlCultureNameToLCID(__in PUNICODE_STRING String, __out PLCID Lcid)
{
	LCID result = 0;
	int i;
	for(i=0;i<2;i++)
	{
		if(lstrcmpiW(lpName,LocaleList[i].code) == 0)
		{
			result = LocaleList[i].lcidHex;
		}
	}
	//sreturn result;
	return TRUE;
}
 */
/*
 * @implemented - new - locale

BOOLEAN WINAPI RtlLCIDToCultureName(LCID lcid, LPCWSTR locale)
{
  /*int result; // eax@2
  if ( flags )
  {
    SetLastError(87u);
    result = 0;
  }
  else
  {
	result = GetLocaleInfoW( lcid, LOCALE_SNAME | LOCALE_NOUSEROVERRIDE, name, count );
  }
  return result;
	int i;
	for(i=0;i<2;i++)
	{
		if(lcid == LocaleList[i].lcidHex)
		{
			locale = LocaleList[i].code;
		}
	}
  return TRUE;
}
*/