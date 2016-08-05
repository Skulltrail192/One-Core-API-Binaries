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

#define MUI_LANGUAGE_ID 1
#define MUI_LANGUAGE_NAME 2

LOCALE LocaleList[3];

void InitTable()
{
	LocaleList[0].description = L"English - United States"; LocaleList[0].cultureName = L"en-US"; LocaleList[0].lcidHex = 0x0409; LocaleList[0].lcidDec = 1033;	
	LocaleList[1].description = L"Portuguese - Brazil"; LocaleList[1].cultureName = L"pt-BR"; LocaleList[1].lcidHex = 0x0416; LocaleList[1].lcidDec = 1046;
	LocaleList[2].description = L"Chinese"; LocaleList[2].cultureName = L"zh-CN"; LocaleList[2].lcidHex = 0x0804; LocaleList[2].lcidDec = 2052;	
}

/*
 * @unimplemented - need reimplementation
 */
BOOL 
WINAPI 
RtlGetThreadPreferredUILanguages(
	_In_       DWORD dwFlags,
	_Out_      PULONG pulNumLanguages,
	_Out_opt_  PZZWSTR pwszLanguagesBuffer,
	_Inout_    PULONG pcchLanguagesBuffer
)
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

NTSTATUS 
WINAPI 
RtlGetUserPreferredUILanguages(
	DWORD dwFlags, 
	BOOL verification,
	PULONG pulNumLanguages, 
	PZZWSTR pwszLanguagesBuffer, 
	PULONG pcchLanguagesBuffer
)
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

BOOL 
WINAPI 
RtlGetSystemPreferredUILanguages(
  _In_       DWORD dwFlags,
  _Out_      PULONG pulNumLanguages,
  _Out_opt_  PZZWSTR pwszLanguagesBuffer,
  _Inout_    PULONG pcchLanguagesBuffer
)
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

NTSTATUS 
WINAPI 
RtlGetUILanguageInfo(
	DWORD dwFlags, 
	PCZZWSTR pwmszLanguage, 
	PZZWSTR pwszFallbackLanguages,
	PDWORD pcchFallbackLanguages,
	PDWORD pdwAttributes
)
{
	pwmszLanguage = L"en-US";
	pwszFallbackLanguages = L"en-US";
	*pcchFallbackLanguages = 1;
	return STATUS_SUCCESS;
}

/*
 * @unimplemented - need reimplementation
 */
BOOL 
WINAPI 
RtlSetThreadPreferredUILanguages(
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
*/
BOOLEAN 
NTAPI 
RtlCultureNameToLCID(
	__in PUNICODE_STRING String, 
	__out PLCID Lcid
)
{
	int i;	
	InitTable();
	
	for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(strcmp(String->Buffer,LocaleList[i].cultureName) == 0)
		{
			*Lcid = LocaleList[i].lcidHex;
			return TRUE;
		}
	}
	return FALSE;
}

/*
 * @implemented - new - locale
*/
BOOLEAN 
WINAPI 
RtlLCIDToCultureName(
	IN LCID lcid, 
	OUT PUNICODE_STRING locale
)
{
	int i;
	InitTable();
	
	for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(lcid == LocaleList[i].lcidHex)
		{
			RtlInitUnicodeString(locale, LocaleList[i].cultureName);
			return TRUE;
		}
	} 
	return FALSE;
}

NTSTATUS 
NTAPI 	
RtlLcidToLocaleName(
	_In_ LCID lcid, 
	_Inout_ PUNICODE_STRING LocaleName, 
	_In_ ULONG Flags, 
	_In_ BOOLEAN AllocateDestinationString
)
{
	int i;
	InitTable();
	
	for(i=0;i<sizeof(LocaleList)/sizeof(LOCALE);i++)
	{
		if(lcid == LocaleList[i].lcidHex)
		{
			RtlInitUnicodeString(LocaleName, LocaleList[i].cultureName);
			return STATUS_SUCCESS;
		}
	}	
	return STATUS_UNSUCCESSFUL;
}

NTSTATUS 
NTAPI 
RtlpQueryDefaultUILanguage(
	LANGID langid, 
	BOOLEAN verification
)
{
	NTSTATUS Status; // eax@1

	Status = NtQueryDefaultUILanguage(&langid);
	if ( (Status & 0xC0000000) == 0xC0000000 )
		Status = NtQueryInstallUILanguage(&langid);
	return Status;
}

NTSTATUS 
NTAPI 
RtlpSetDefaultUILanguage(LANGID langId)
{
	return NtSetDefaultUILanguage(langId);
}

BOOL 
NTAPI 
RtlGetFileMUIPath(
  _In_         DWORD dwFlags,
  _In_         PCWSTR pcwszFilePath,
  _Inout_opt_  PWSTR pwszLanguage,
  _Inout_      PULONG pcchLanguage,
  _Out_opt_    PWSTR pwszFileMUIPath,
  _Inout_      PULONG pcchFileMUIPath,
  _Inout_      PULONGLONG pululEnumerator
)
{
	return FALSE;
}