/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    scm.c

Abstract:

    Service control manager functions

Author:

    Skulltrail 19-July-2021

Revision History:

--*/

#include "main.h"

typedef BOOL (WINAPI *I_QUERY_TA_INFORMATION)(
    PVOID,
	TAG_INFO_LEVEL,
	PTAG_INFO_NAME_FROM_TAG);

/**********************************************************************
 *  I_QueryTagInformation
 *
 * @implemented
 */
DWORD WINAPI
I_QueryTagInformation(PVOID Unused,
                      TAG_INFO_LEVEL dwInfoLevel,
                      PTAG_INFO_NAME_FROM_TAG InOutParams)
{
	I_QUERY_TA_INFORMATION i_QueryTagInformation;
    i_QueryTagInformation = (I_QUERY_TA_INFORMATION) GetProcAddress(
                            GetModuleHandle(TEXT("advapibase.dll")),
                            "I_QueryTagInformation");	
	if(i_QueryTagInformation == NULL){
		return ERROR_CALL_NOT_IMPLEMENTED;
	}else{
		return i_QueryTagInformation(Unused,
									 dwInfoLevel,
									 InOutParams);
	}    
}
