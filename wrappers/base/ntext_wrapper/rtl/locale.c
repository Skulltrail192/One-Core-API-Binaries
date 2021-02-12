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
#include <locale.h>

#define MUI_LANGUAGE_ID 1
#define MUI_LANGUAGE_NAME 2

/***********************************************************************
 *           LCIDToLocaleName  (KERNEL32.@)
 */
INT 
WINAPI 
RtlpLCIDToLocaleName( 
	LCID lcid, 
	LPWSTR lpName, 
	INT count, 
	DWORD flags 
)
{
	int i;
	int length = 0;
	for(i=0;i<LOCALE_TABLE_SIZE;i++){
		if(lcid == locale_table[i].lcid){
			length = (wcslen(locale_table[i].localeName)+1);
			if(lpName){
				memcpy(lpName, locale_table[i].localeName, sizeof(WCHAR)*(length));
				lpName[length-1] = 0;
			}			
			return length;
		}
	}
	return length;
}

NTSTATUS 
NTAPI 	
RtlLcidToLocaleName(
	_In_ LCID lcid, 
	_Inout_ PUNICODE_STRING locale, 
	_In_ ULONG Flags, 
	_In_ BOOLEAN AllocateDestinationString
)
{
	LPWSTR lpName = L"";
	
	if(RtlpLCIDToLocaleName(lcid, lpName, 0, 0)>0){
		RtlInitUnicodeString(locale, lpName);
		return STATUS_SUCCESS;
	}else{
		return STATUS_UNSUCCESSFUL;		
	}

}

/*
 * @implemented - new - locale
*/
NTSTATUS 
NTAPI 
RtlLCIDToCultureName(
	IN LCID lcid, 
	OUT PUNICODE_STRING lpName
)
{
	return RtlLcidToLocaleName(lcid, lpName, 0, TRUE);
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
	
	for(i=0;i<LOCALE_TABLE_SIZE;i++){
		if(wcscmp(String->Buffer, locale_table[i].localeName)==0){
			*Lcid = locale_table[i].lcid;
			return TRUE;
		}
	}
	return FALSE;
}

NTSTATUS
NTAPI
RtlLocaleNameToLcid(
    _In_ PWSTR LocaleName,
    _Out_ PLCID lcid,
    _In_ ULONG Flags
)
{
    int i;

	for(i=0;i<LOCALE_TABLE_SIZE;i++){
		if(wcscmp(LocaleName, locale_table[i].localeName)==0){
			*lcid = locale_table[i].lcid;
			return STATUS_SUCCESS;
		}
	}
	
	return STATUS_UNSUCCESSFUL;
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
	WCHAR locale[LOCALE_NAME_MAX_LENGTH];
	UINT length;	
	LCID UILangId;// = ((LCID)(NtCurrentTeb()->CurrentLocale));
	NTSTATUS status;
	
	status = NtQueryDefaultLocale(FALSE, &UILangId);
	
	if(!NT_SUCCESS(status)){
		return status;
	}

	if ( pwszLanguagesBuffer ){
		*pulNumLanguages = 2;	
		length = *pcchLanguagesBuffer;	
		if(dwFlags == MUI_LANGUAGE_ID){
			wcscpy(locale, _itow(UILangId, locale, 16));
			wcscpy(pwszLanguagesBuffer, locale);
			memcpy(pwszLanguagesBuffer+wcslen(pwszLanguagesBuffer), L"\0409\0\0", sizeof(WCHAR)*(5));				
		}else{
			RtlpLCIDToLocaleName(UILangId, locale, LOCALE_NAME_MAX_LENGTH, 0);
			wcscpy(pwszLanguagesBuffer, locale);
			memcpy(pwszLanguagesBuffer+wcslen(pwszLanguagesBuffer), L"\0en-US\0\0", sizeof(WCHAR)*(7));				
		}		
		return STATUS_SUCCESS;
	}else{
		*pulNumLanguages = 2;
		if(dwFlags == MUI_LANGUAGE_ID){
			length = 9;
		}else{
			length = (7 + RtlpLCIDToLocaleName(UILangId, NULL, 0, 0));
		}		
		*pcchLanguagesBuffer = length;
		return STATUS_INVALID_PARAMETER;
	}	
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
	WCHAR locale[LOCALE_NAME_MAX_LENGTH];
	UINT length;	
	LCID lcid = ((LCID)(NtCurrentTeb()->CurrentLocale));

	if ( pwszLanguagesBuffer ){
		*pulNumLanguages = 2;	
		length = *pcchLanguagesBuffer;	
		if(dwFlags == MUI_LANGUAGE_ID){
			wcscpy(locale, _itow(lcid, locale, 16));
			wcscpy(pwszLanguagesBuffer, locale);
			memcpy(pwszLanguagesBuffer+wcslen(pwszLanguagesBuffer), L"\0409\0\0", sizeof(WCHAR)*(5));				
		}else{
			RtlpLCIDToLocaleName(lcid, locale, LOCALE_NAME_MAX_LENGTH, 0);
			wcscpy(pwszLanguagesBuffer, locale);
			memcpy(pwszLanguagesBuffer+wcslen(pwszLanguagesBuffer), L"\0en-US\0\0", sizeof(WCHAR)*(7));				
		}		
		return STATUS_SUCCESS;
	}else{
		*pulNumLanguages = 2;
		if(dwFlags == MUI_LANGUAGE_ID){
			length = 9;
		}else{
			length = (7 + RtlpLCIDToLocaleName(lcid, NULL, 0, 0));
		}		
		*pcchLanguagesBuffer = length;
		return STATUS_INVALID_PARAMETER;
	}		
}

/*
 * @unimplemented - need reimplementation
 */
NTSTATUS 
WINAPI 
RtlGetThreadPreferredUILanguages(
	_In_       DWORD dwFlags,
	_Out_      PULONG pulNumLanguages,
	_Out_opt_  PZZWSTR pwszLanguagesBuffer,
	_Inout_    PULONG pcchLanguagesBuffer
)
{
	WCHAR locale[LOCALE_NAME_MAX_LENGTH];
	UINT length;	
	LCID lcid;
	
	lcid  = ((LCID)(NtCurrentTeb()->CurrentLocale));

	if ( pwszLanguagesBuffer ){
		*pulNumLanguages = 2;	
		length = *pcchLanguagesBuffer;	
		if(dwFlags == MUI_LANGUAGE_ID){
			wcscpy(locale, _itow(lcid, locale, 16));
			wcscpy(pwszLanguagesBuffer, locale);
			memcpy(pwszLanguagesBuffer+wcslen(pwszLanguagesBuffer), L"\0409\0\0", sizeof(WCHAR)*(5));				
		}else{
			RtlpLCIDToLocaleName(lcid, locale, LOCALE_NAME_MAX_LENGTH, 0);
			wcscpy(pwszLanguagesBuffer, locale);
			memcpy(pwszLanguagesBuffer+wcslen(pwszLanguagesBuffer), L"\0en-US\0\0", sizeof(WCHAR)*(7));				
		}		
		return STATUS_SUCCESS;
	}else{
		*pulNumLanguages = 2;
		if(dwFlags == MUI_LANGUAGE_ID){
			length = 9;
		}else{
			length = (7 + RtlpLCIDToLocaleName(lcid, NULL, 0, 0));
		}		
		*pcchLanguagesBuffer = length;
		return STATUS_INVALID_PARAMETER;
	}	
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
NTSTATUS 
WINAPI 
RtlSetThreadPreferredUILanguages(
  _In_       DWORD dwFlags,
  _In_opt_   PCZZWSTR pwszLanguagesBuffer,
  _Out_opt_  PULONG pulNumLanguages
)
{
	//Use Longhorn ntdll
	// LCID langId;
	
	// *pulNumLanguages = 1;
	
	// RtlLocaleNameToLcid(pwszLanguagesBuffer, langId, 0);
	// return NtSetDefaultUILanguage(&langId);
	return STATUS_SUCCESS;
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

NTSTATUS 
NTAPI 
RtlpSetCurrentUserUILanguage(LANGID langId)
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