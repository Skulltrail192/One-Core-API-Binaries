/*++

Copyright (c) Microsoft Corporation

Module Name:

    baseinit.c

Abstract:

    This module implements Win32 base initialization

Author:

    Mark Lucovsky (markl) 26-Sep-1990

Revision History:

--*/

#include <main.h>

HANDLE BaseNamedObjectDirectory;
UNICODE_STRING Restricted = RTL_CONSTANT_STRING(L"Restricted");

HANDLE 
WINAPI 
BaseGetNamedObjectDirectory(VOID) 	
{
    OBJECT_ATTRIBUTES ObjectAttributes;
    NTSTATUS Status;
    HANDLE DirHandle, BnoHandle, Token, NewToken;

    if (BaseNamedObjectDirectory) return BaseNamedObjectDirectory;

    if (NtCurrentTeb()->IsImpersonating)
    {
        Status = NtOpenThreadToken(NtCurrentThread(),
                                   TOKEN_IMPERSONATE,
                                   TRUE,
                                   &Token);
        if (!NT_SUCCESS(Status)) return BaseNamedObjectDirectory;

        NewToken = NULL;
        Status = NtSetInformationThread(NtCurrentThread(),
                                        ThreadImpersonationToken,
                                        &NewToken,
                                        sizeof(HANDLE));
        if (!NT_SUCCESS (Status))
        {
            NtClose(Token);
            return BaseNamedObjectDirectory;
        }
    }
    else
    {
        Token = NULL;
    }

    RtlAcquirePebLock();
    if (BaseNamedObjectDirectory) goto Quickie;

    InitializeObjectAttributes(&ObjectAttributes,
                               &BaseStaticServerData->NamedObjectDirectory,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);

    Status = NtOpenDirectoryObject(&BnoHandle,
                                   DIRECTORY_QUERY |
                                   DIRECTORY_TRAVERSE |
                                   DIRECTORY_CREATE_OBJECT |
                                   DIRECTORY_CREATE_SUBDIRECTORY,
                                   &ObjectAttributes);
    if (!NT_SUCCESS(Status))
    {
        Status = NtOpenDirectoryObject(&DirHandle,
                                       DIRECTORY_TRAVERSE,
                                       &ObjectAttributes);

        if (NT_SUCCESS(Status))
        {
            InitializeObjectAttributes(&ObjectAttributes,
                                       (PUNICODE_STRING)&Restricted,
                                       OBJ_CASE_INSENSITIVE,
                                       DirHandle,
                                       NULL);

            Status = NtOpenDirectoryObject(&BnoHandle,
                                           DIRECTORY_QUERY |
                                           DIRECTORY_TRAVERSE |
                                           DIRECTORY_CREATE_OBJECT |
                                           DIRECTORY_CREATE_SUBDIRECTORY,
                                           &ObjectAttributes);
            NtClose(DirHandle);

        }
    }

    if (NT_SUCCESS(Status)) BaseNamedObjectDirectory = BnoHandle;

Quickie:

    RtlReleasePebLock();

    if (Token)
    {
        NtSetInformationThread(NtCurrentThread(),
                               ThreadImpersonationToken,
                               &Token,
                               sizeof(Token));

        NtClose(Token);
    }

    return BaseNamedObjectDirectory;
}