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

/* PSEH for SEH Support */
#include <pseh/pseh2.h>
#include <../kernelfull/kernelfuncs.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

typedef NTSTATUS(NTAPI * PRTL_CONVERT_STRING)(IN PUNICODE_STRING UnicodeString, IN PANSI_STRING AnsiString, IN BOOLEAN AllocateMemory);

HANDLE 	BaseNamedObjectDirectory;

UNICODE_STRING Restricted = RTL_CONSTANT_STRING(L"Restricted");

UNICODE_STRING BaseDotComSuffixName = RTL_CONSTANT_STRING(L".com");

UNICODE_STRING BaseDotPifSuffixName = RTL_CONSTANT_STRING(L".pif");

UNICODE_STRING BaseDotExeSuffixName = RTL_CONSTANT_STRING(L".exe");

RTL_CRITICAL_SECTION gcsAppCert;

BOOLEAN g_HaveAppCerts;

BOOLEAN g_AppCertInitialized;

NTSTATUS g_AppCertStatus;

BOOLEAN BaseRunningInServerProcess = FALSE;

PBASEP_APPCERT_EMBEDDED_FUNC fEmbeddedCertFunc;

PRTL_CONVERT_STRING Basep8BitStringToUnicodeString = RtlAnsiStringToUnicodeString;

void Report32bitAppLaunching ( );

RTL_QUERY_REGISTRY_TABLE BasepAppCertTable[2] =
{
    {
        BasepConfigureAppCertDlls,
        1,
        L"AppCertDlls",
        &BasepAppCertDllsList,
        0,
        NULL,
        0
    }
};

NTSTATUS
NTAPI
BasepAllocateActivationContextActivationBlock(
	IN DWORD Flags,
    IN PVOID CompletionRoutine,
    IN PVOID CompletionContext,
    OUT PBASEP_ACTCTX_BLOCK *ActivationBlock
)
{
    NTSTATUS Status;
    ACTIVATION_CONTEXT_BASIC_INFORMATION ContextInfo;

    /* Clear the info structure */
    ContextInfo.dwFlags = 0;
    ContextInfo.hActCtx = NULL;

    /* Assume failure */
    if (ActivationBlock) *ActivationBlock = NULL;

    /* Only support valid flags */
    if (Flags & ~(1 | 2)) // FIXME: What are they? 2 looks like BASEP_ACTCTX_FORCE_BLOCK
    {
        /* Fail if unknown flags are passed in */
        Status = STATUS_INVALID_PARAMETER_1;
        goto Quickie;
    }

    /* Caller should have passed in an activation block */
    if (!ActivationBlock)
    {
        /* Fail otherwise */
        Status = STATUS_INVALID_PARAMETER_4;
        goto Quickie;
    }

    /* Query RTL for information on the current activation context */
    Status = RtlQueryInformationActivationContext(RTL_QUERY_ACTIVATION_CONTEXT_FLAG_USE_ACTIVE_ACTIVATION_CONTEXT,
                                                  NULL,
                                                  NULL,
                                                  ActivationContextBasicInformation,
                                                  &ContextInfo,
                                                  sizeof(ContextInfo),
                                                  NULL);
    if (!NT_SUCCESS(Status))
    {
        /* Failed -- bail out */
        TRACE("SXS: %s - Failure getting active activation context; ntstatus %08lx\n",
                __FUNCTION__, Status);
        goto Quickie;
    }

    /* Check if the current one should be freed */
    if (ContextInfo.dwFlags & 1)
    {
        /* Release and clear it */
        RtlReleaseActivationContext(ContextInfo.hActCtx);
        ContextInfo.hActCtx = NULL;
    }

    /* Check if there's an active context, or if the caller is forcing one */
    if (!(Flags & 2) || (ContextInfo.hActCtx))
    {
        /* Allocate the block */
        *ActivationBlock = RtlAllocateHeap(RtlGetProcessHeap(),
                                           0,
                                           sizeof(BASEP_ACTCTX_BLOCK));
        if (!(*ActivationBlock))
        {
            /* Ran out of memory, fail */
            Status = STATUS_NO_MEMORY;
            goto Quickie;
        }

        /* Fill it out */
        (*ActivationBlock)->ActivationContext = ContextInfo.hActCtx;
        (*ActivationBlock)->Flags = 0;
        if (Flags & 1) (*ActivationBlock)->Flags |= 1; // Not sure about this flag
        (*ActivationBlock)->CompletionRoutine = CompletionRoutine;
        (*ActivationBlock)->CompletionContext = CompletionContext;

        /* Tell Quickie below not to free anything, since this is success */
        ContextInfo.hActCtx = NULL;
    }

    /* Set success status */
    Status = STATUS_SUCCESS;

Quickie:
    /* Failure or success path, return to caller and free on failure */
    if (ContextInfo.hActCtx) RtlReleaseActivationContext(ContextInfo.hActCtx);
    return Status;
}

VOID 
NTAPI 
BasepFreeActivationContextActivationBlock( 	
	IN PBASEP_ACTCTX_BLOCK  	ActivationBlock	
) 	
{
    /* Exit if there was nothing passed in */
    if (!ActivationBlock) return;

    /* Do we have a context? */
    if (ActivationBlock->ActivationContext)
    {
        /* Release and clear it */
        RtlReleaseActivationContext(ActivationBlock->ActivationContext);
        ActivationBlock->ActivationContext = NULL;
    }

    /* Free the block */
    RtlFreeHeap(RtlGetProcessHeap(), 0, ActivationBlock);
}

BOOL 
WINAPI 
BasepAnsiStringToDynamicUnicodeString(
	PUNICODE_STRING DestinationString, 
	PCSZ SourceStringParameter
)
{
  NTSTATUS status; // eax@2
  STRING SourceString; // [sp+0h] [bp-8h]@1

  if ( RtlInitAnsiStringEx(&SourceString, SourceStringParameter) < 0 )
    goto LABEL_10;
  status = RtlAnsiStringToUnicodeString(DestinationString, &SourceString, 1u);
  if ( status >= 0 )
    return TRUE;
  if ( status == STATUS_BUFFER_OVERFLOW )
LABEL_10:
    SetLastError(206);
  else
    BaseSetLastNTError(status);
  return FALSE;
}

//move to other source file
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



POBJECT_ATTRIBUTES 
WINAPI 
BaseFormatObjectAttributes( 	
	OUT POBJECT_ATTRIBUTES  	ObjectAttributes,
	IN PSECURITY_ATTRIBUTES SecurityAttributes  	OPTIONAL,
	IN PUNICODE_STRING  	ObjectName 
) 		
{
    ULONG Attributes;
    HANDLE RootDirectory;
    PVOID SecurityDescriptor;
    DPRINTF("BaseFormatObjectAttributes. Security: %p, Name: %p\n",
            SecurityAttributes, ObjectName);

    /* Get the attributes if present */
    if (SecurityAttributes)
    {
        Attributes = SecurityAttributes->bInheritHandle ? OBJ_INHERIT : 0;
        SecurityDescriptor = SecurityAttributes->lpSecurityDescriptor;
    }
    else
    {
        if (!ObjectName) return NULL;
        Attributes = 0;
        SecurityDescriptor = NULL;
    }

    if (ObjectName)
    {
        Attributes |= OBJ_OPENIF;
        RootDirectory = BaseGetNamedObjectDirectory();
    }
    else
    {
        RootDirectory = NULL;
    }

    /* Create the Object Attributes */
    InitializeObjectAttributes(ObjectAttributes,
                               ObjectName,
                               Attributes,
                               RootDirectory,
                               SecurityDescriptor);
    DPRINTF("Attributes: %lx, RootDirectory: %p, SecurityDescriptor: %p\n",
            Attributes, RootDirectory, SecurityDescriptor);
    return ObjectAttributes;
}


/*
 * Converts an ANSI or OEM String to the TEB DynamicUnicodeString
 */
BOOLEAN 
WINAPI 
Basep8BitStringToDynamicUnicodeString(
	OUT PUNICODE_STRING UnicodeString, 
	IN LPCSTR String
) 		
{
    ANSI_STRING AnsiString;
    NTSTATUS Status;

    /* Initialize an ANSI String */
    Status = RtlInitAnsiStringEx(&AnsiString, String);
    if (!NT_SUCCESS(Status))
    {
        Status = STATUS_BUFFER_OVERFLOW;
    }
    else
    {
        /* Convert it */
        Status = Basep8BitStringToUnicodeString(UnicodeString, &AnsiString, TRUE);
    }

    if (NT_SUCCESS(Status)) return TRUE;

    if (Status == STATUS_BUFFER_OVERFLOW)
    {
        SetLastError(ERROR_FILENAME_EXCED_RANGE);
    }
    else
    {
        BaseSetLastNTError(Status);
    }

    return FALSE;
}

/*
 * @implemented
*/
VOID
WINAPI
BaseSetLastNTError(
	IN NTSTATUS Status
)
{
    /* Convert from NT to Win32, then set */
    SetLastError(RtlNtStatusToDosError(Status));
}

/*
 * Converts an ANSI or OEM String to the TEB StaticUnicodeString
 */
PUNICODE_STRING
WINAPI
Basep8BitStringToStaticUnicodeString(
	IN LPCSTR String
)
{
    PUNICODE_STRING StaticString = &(NtCurrentTeb()->StaticUnicodeString);
    ANSI_STRING AnsiString;
    NTSTATUS Status;

    /* Initialize an ANSI String */
    Status = RtlInitAnsiStringEx(&AnsiString, String);
    if (!NT_SUCCESS(Status))
    {
        Status = STATUS_BUFFER_OVERFLOW;
    }
    else
    {
        /* Convert it */
        Status = Basep8BitStringToUnicodeString(StaticString, &AnsiString, FALSE);
    }

    if (NT_SUCCESS(Status)) return StaticString;

    if (Status == STATUS_BUFFER_OVERFLOW)
    {
        SetLastError(ERROR_FILENAME_EXCED_RANGE);
    }
    else
    {
        BaseSetLastNTError(Status);
    }

    return NULL;
}

/*
 * @implemented
 */
PLARGE_INTEGER 
WINAPI 
BaseFormatTimeOut(
	OUT PLARGE_INTEGER  	Timeout,
	IN DWORD  	dwMilliseconds 
) 		
{
    /* Check if this is an infinite wait, which means no timeout argument */
    if (dwMilliseconds == INFINITE) return NULL;

    /* Otherwise, convert the time to NT Format */
    Timeout->QuadPart = UInt32x32To64(dwMilliseconds, -10000);
    return Timeout;
}

PVOID 
WINAPI 
BasepMapModuleHandle( 	
	HMODULE  	hModule,
	BOOLEAN  	AsDataFile 
)
{
    /* If no handle is provided - use current image base address */
    if (!hModule) return NtCurrentPeb()->ImageBaseAddress;

    /* Check if it's a normal or a datafile one */
    if (LDR_IS_DATAFILE(hModule) && !AsDataFile)
        return NULL;

    /* It'a a normal DLL, just return its handle */
    return hModule;
}

DWORD 
WINAPI 
BasePrepareReasonContext(
	REASON_CONTEXT *contex, 
	PVOID address
)
{
  size_t size; // ebx@1
  ULONG StringCount; // ecx@1
  DWORD Flags; // eax@2
  DWORD ModuleNamae; // eax@8
  PVOID heap; // eax@11
  REASON_CONTEXT *Localcontext; // esi@11
  DWORD LocalFlgs; // eax@14
  ULONG numner; // ecx@15
  ULONG version; // eax@15
  LPWSTR *string; // ebx@15
  DWORD resp; // edi@16
  WCHAR Src; // [sp+10h] [bp-20Ch]@8

  size = 0;
  StringCount = 28;
  if ( contex )
  {
    Flags = contex->Flags;
    if ( (Flags & 0x80000000u) == 0 )
    {
      if ( contex->Version > 0 || !(Flags & 3) || Flags & 0x7FFFFFFC )
        return STATUS_INVALID_PARAMETER;
      if ( !(Flags & 2) )
        goto LABEL_11;
      if ( contex->Reason.Detailed.LocalizedReasonId > 0xFFFF
        || (ModuleNamae = GetModuleFileNameW(contex->Reason.Detailed.LocalizedReasonModule, &Src, 0x104u)) == 0
        || ModuleNamae == 260 )
        return STATUS_INVALID_PARAMETER;
      size = 2 * ModuleNamae + 2;
      StringCount = ((2 * ModuleNamae + 5) & 0xFFFFFFFC) + 8 * contex->Reason.Detailed.ReasonStringCount + 28;
    }
  }
LABEL_11:

  #ifdef _M_IX86
	 heap = RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, StringCount);
  #elif defined(_M_AMD64)
	 heap = RtlAllocateHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), 0, StringCount);
  #endif
  Localcontext = (REASON_CONTEXT *)heap;
  if ( heap )
  {
    if ( !contex || contex->Flags & 0x80000000 )
    {
      *(DWORD *)heap = 0;
      *((DWORD *)heap + 1) = 0x80000000u;
      goto LABEL_16;
    }
    *(DWORD *)heap = contex->Version;
    LocalFlgs = contex->Flags;
    Localcontext->Flags = LocalFlgs;
    if ( LocalFlgs & 1 )
    {
        goto LABEL_16;
    }
    else
    {
      memcpy(&Localcontext[1].Flags, &Src, size);
      Localcontext->Reason.Detailed.LocalizedReasonId = (ULONG)&Localcontext[1].Flags;
      Localcontext->Reason.Detailed.LocalizedReasonModule = (HMODULE)size;
      Localcontext->Reason.Detailed.LocalizedReasonModule = (HMODULE)size - 2;
      Localcontext->Reason.Detailed.ReasonStringCount = contex->Reason.Detailed.LocalizedReasonId;
      numner = contex->Reason.Detailed.ReasonStringCount;
      version = (unsigned int)((char *)&Localcontext[1].Flags + size + 3) & 0xFFFFFFFC;
      string = 0;
      Localcontext->Reason.Detailed.ReasonStrings = (LPWSTR *)numner;
      Localcontext[1].Version = version;
      if ( !numner )
      {
LABEL_16:
        resp = 0;
        address = Localcontext;
        return resp;
      }
    }
    resp = STATUS_INVALID_PARAMETER;
  }
  else
  {
    resp = STATUS_INSUFFICIENT_RESOURCES;
  }
  if ( Localcontext )
	#ifdef _M_IX86
		RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readfsdword(24) + 48) + 24), 0, Localcontext);
    #elif defined(_M_AMD64)
	    RtlFreeHeap(*(HANDLE *)(*(DWORD *)(__readgsqword(24) + 48) + 24), 0, Localcontext);
    #endif
  return resp;
}

BOOL 
WINAPI 
TermsrvAppInstallMode()
{
  return BaseStaticServerData->NlsUserInfo.iDigits[28] != 0;
}

DWORD 
WINAPI 
BaseDllReadWriteIniFile(
	int a1, 
	BOOLEAN a2, 
	BOOLEAN a3, 
	int a4, 
	int a5, 
	int a6, 
	int a7, 
	int a8
)
{
	return 0;
}

BOOL 
WINAPI 
BaseDestroyVDMEnvironment(
	PANSI_STRING AnsiString, 
	PUNICODE_STRING UnicodeEnv
)
{
  if ( AnsiString->Buffer )
    RtlFreeAnsiString(AnsiString);
  if ( UnicodeEnv->Buffer )
    RtlDestroyEnvironment(UnicodeEnv->Buffer);
  return TRUE;
}

ULONG 
WINAPI 
BaseIsDosApplication( 
	IN PUNICODE_STRING  	PathName,
	IN NTSTATUS  	Status 
) 
{
    UNICODE_STRING String;

    /* Is it a .com? */
    String.Length = BaseDotComSuffixName.Length;
    String.Buffer = &PathName->Buffer[(PathName->Length - String.Length) / sizeof(WCHAR)];
    if (RtlEqualUnicodeString(&String, &BaseDotComSuffixName, TRUE)) return BINARY_TYPE_COM;

    /* Is it a .pif? */
    String.Length = BaseDotPifSuffixName.Length;
    String.Buffer = &PathName->Buffer[(PathName->Length - String.Length) / sizeof(WCHAR)];
    if (RtlEqualUnicodeString(&String, &BaseDotPifSuffixName, TRUE)) return BINARY_TYPE_PIF;

    /* Is it an exe? */
    String.Length = BaseDotExeSuffixName.Length;
    String.Buffer = &PathName->Buffer[(PathName->Length - String.Length) / sizeof(WCHAR)];
    if (RtlEqualUnicodeString(&String, &BaseDotExeSuffixName, TRUE)) return BINARY_TYPE_EXE;

    return 0;
}

BOOL 
WINAPI 
BaseUpdateVDMEntry( 	
	IN ULONG  	UpdateIndex,
	IN OUT PHANDLE  	WaitHandle,
	IN ULONG  	IndexInfo,
	IN ULONG  	BinaryType 
)
{
    NTSTATUS Status;
    BASE_API_MESSAGE ApiMessage;
    PBASE_UPDATE_VDM_ENTRY UpdateVdmEntry = &ApiMessage.Data.UpdateVDMEntryRequest;

    /* Check what update is being sent */
    switch (UpdateIndex)
    {
        /* VDM is being undone */
        case VdmEntryUndo:
        {
            /* Tell the server how far we had gotten along */
            UpdateVdmEntry->iTask = HandleToUlong(*WaitHandle);
            UpdateVdmEntry->VDMCreationState = IndexInfo;
            break;
        }

        /* VDM is ready with a new process handle */
        case VdmEntryUpdateProcess:
        {
            /* Send it the process handle */
            UpdateVdmEntry->VDMProcessHandle = *WaitHandle;
            UpdateVdmEntry->iTask = IndexInfo;
            break;
        }
    }

    /* Also check what kind of binary this is for the console handle */
    if (BinaryType == BINARY_TYPE_WOW)
    {
        /* Magic value for 16-bit apps */
        UpdateVdmEntry->ConsoleHandle = (HANDLE)-1;
    }
    else if (UpdateVdmEntry->iTask)
    {
        /* No handle for true VDM */
        UpdateVdmEntry->ConsoleHandle = NULL;
    }
    else
    {
        /* Otherwise, use the regular console handle */
        UpdateVdmEntry->ConsoleHandle = NtCurrentPeb()->ProcessParameters->ConsoleHandle;
    }

    /* Finally write the index and binary type */
    UpdateVdmEntry->EntryIndex = UpdateIndex;
    UpdateVdmEntry->BinaryType = BinaryType;

    /* Send the message to CSRSS */
    Status = CsrClientCallServer((PCSR_API_MESSAGE)&ApiMessage,
                                 NULL,
                                 CSR_CREATE_API_NUMBER(BASESRV_SERVERDLL_INDEX, BasepUpdateVDMEntry),
                                 sizeof(BASE_UPDATE_VDM_ENTRY));
    if (!NT_SUCCESS(Status))
    {
        /* Handle failure */
        BaseSetLastNTError(Status);
        return FALSE;
    }

    /* If this was an update, CSRSS returns a new wait handle */
    if (UpdateIndex == VdmEntryUpdateProcess)
    {
        /* Return it to the caller */
        *WaitHandle = UpdateVdmEntry->WaitObjectForParent;
    }

    /* We made it */
    return TRUE;
}

BOOLEAN 
WINAPI 
BaseConvertCharFree(
	PVOID Address
)
{
  BOOLEAN result; // al@2

  if ( Address )
    result = RtlFreeHeap(RtlGetProcessHeap(), 0, Address);
  return result;
}

NTSTATUS 
WINAPI 
BaseElevationPostProcessing(
	BOOLEAN verification, 
	ULONG ProcessInformation, 
	HANDLE ProcessHandle
)
{
  NTSTATUS result; // eax@1

  result = STATUS_SUCCESS;
  if ( verification & 2 && (!ProcessInformation || ProcessInformation == 10) )
  {
    ProcessInformation = 1;
    result = NtSetInformationProcess(ProcessHandle, ProcessBreakOnTermination, &ProcessInformation, 4u);
  }
  return result;
}

NTSTATUS
WINAPI
BasepIsProcessAllowed(
	IN LPWSTR ApplicationName
)
{
    NTSTATUS Status, Status1;
    PWCHAR Buffer;
    UINT Length;
    HMODULE TrustLibrary;
    PBASEP_APPCERT_ENTRY Entry;
    ULONG CertFlag;
    PLIST_ENTRY NextEntry;
    HANDLE KeyHandle;
    UNICODE_STRING CertKey = RTL_CONSTANT_STRING(L"\\Registry\\MACHINE\\System\\CurrentControlSet\\Control\\Session Manager\\AppCertDlls");
    OBJECT_ATTRIBUTES KeyAttributes = RTL_CONSTANT_OBJECT_ATTRIBUTES(&CertKey, OBJ_CASE_INSENSITIVE);

    /* Try to initialize the certification subsystem */
    while (!g_AppCertInitialized)
    {
        /* Defaults */
        Status = STATUS_SUCCESS;
        Buffer = NULL;

        /* Acquire the lock while initializing and see if we lost a race */
        RtlEnterCriticalSection(&gcsAppCert);
        if (g_AppCertInitialized) break;

        /* On embedded, there is a special DLL */
        if (SharedUserData->SuiteMask & VER_SUITE_EMBEDDEDNT)
        {
            /* Allocate a buffer for the name */
            Buffer = RtlAllocateHeap(RtlGetProcessHeap(),
                                     0,
                                     MAX_PATH * sizeof(WCHAR) +
                                     sizeof(UNICODE_NULL));
            if (!Buffer)
            {
                /* Fail if no memory */
                Status = STATUS_NO_MEMORY;
            }
            else
            {
                /* Now get the system32 directory in our buffer, make sure it fits */
                Length = GetSystemDirectoryW(Buffer, MAX_PATH - sizeof("EmbdTrst.DLL"));
                if ((Length) && (Length <= MAX_PATH - sizeof("EmbdTrst.DLL")))
                {
                    /* Add a slash if needed, and add the embedded cert DLL name */
                    if (Buffer[Length - 1] != '\\') Buffer[Length++] = '\\';
                    RtlCopyMemory(&Buffer[Length],
                                  L"EmbdTrst.DLL",
                                  sizeof(L"EmbdTrst.DLL"));

                    /* Try to load it */
                    TrustLibrary = LoadLibraryW(Buffer);
                    if (TrustLibrary)
                    {
                        /* And extract the special function out of it */
                        fEmbeddedCertFunc = (PVOID)GetProcAddress(TrustLibrary,
                                                                  "ImageOkToRunOnEmbeddedNT");
                    }
                }

                /* If we didn't get this far, set a failure code */
                if (!fEmbeddedCertFunc) Status = STATUS_UNSUCCESSFUL;
            }
        }
        else
        {
            /* Other systems have a registry entry for this */
            Status1 = NtOpenKey(&KeyHandle, KEY_READ, &KeyAttributes);
            if (NT_SUCCESS(Status1))
            {
                /* Close it, we'll query it through Rtl */
                NtClose(KeyHandle);

                /* Do the query, which will call a special callback */
                Status = RtlQueryRegistryValues(RTL_REGISTRY_CONTROL,
                                                L"Session Manager",
                                                BasepAppCertTable,
                                                NULL,
                                                NULL);
                if (Status == STATUS_OBJECT_NAME_NOT_FOUND)
                {
                    Status = STATUS_SUCCESS;
                }
            }
        }

        /* Free any buffer if we had one */
        if (Buffer) RtlFreeHeap(RtlGetProcessHeap(), 0, Buffer);

        /* Check for errors, or a missing embedded/custom certification DLL */
        if (!NT_SUCCESS(Status) ||
            (!(fEmbeddedCertFunc) && (IsListEmpty(&BasepAppCertDllsList))))
        {
            /* The subsystem is not active on this machine, so give up */
            g_HaveAppCerts = FALSE;
            g_AppCertStatus = Status;
        }
        else
        {
            /* We have certification DLLs active, remember this */
            g_HaveAppCerts = TRUE;
        }

        /* We are done the initialization phase, release the lock */
        g_AppCertInitialized = TRUE;
        RtlLeaveCriticalSection(&gcsAppCert);
    }

    /* If there's no certification DLLs present, return the failure code */
    if (!g_HaveAppCerts) return g_AppCertStatus;

    /* Otherwise, assume success and make sure we have *something* */
    ASSERT(fEmbeddedCertFunc || !IsListEmpty(&BasepAppCertDllsList));
    Status = STATUS_SUCCESS;

    /* If the something is an embedded certification DLL, call it and return */
    if (fEmbeddedCertFunc) return fEmbeddedCertFunc(ApplicationName);

    /* Otherwise we have custom certification DLLs, parse them */
    NextEntry = BasepAppCertDllsList.Flink;
    CertFlag = 2;
    while (NextEntry != &BasepAppCertDllsList)
    {
        /* Make sure the entry has a callback */
        Entry = CONTAINING_RECORD(NextEntry, BASEP_APPCERT_ENTRY, Entry);
        ASSERT(Entry->fPluginCertFunc != NULL);

        /* Call it and check if it failed */
        Status = Entry->fPluginCertFunc(ApplicationName, 1);
        if (!NT_SUCCESS(Status)) CertFlag = 3;

        /* Move on */
        NextEntry = NextEntry->Flink;
    }

    /* Now loop them again */
    NextEntry = BasepAppCertDllsList.Flink;
    while (NextEntry != &BasepAppCertDllsList)
    {
        /* Make sure the entry has a callback */
        Entry = CONTAINING_RECORD(NextEntry, BASEP_APPCERT_ENTRY, Entry);
        ASSERT(Entry->fPluginCertFunc != NULL);

        /* Call it, this time with the flag from the loop above */
        Status = Entry->fPluginCertFunc(ApplicationName, CertFlag);
    }

    /* All done, return the status */
    return Status;
}

NTSTATUS
NTAPI
BasepSaveAppCertRegistryValue(
	IN PLIST_ENTRY List,
    IN PWCHAR ComponentName,
    IN PWCHAR DllName
)
{
    /* Pretty much the only thing this key is used for, is malware */
    UNIMPLEMENTED;
    return STATUS_NOT_IMPLEMENTED;
}

NTSTATUS 
NTAPI 
BasepConfigureAppCertDlls( 	
	IN PWSTR  	ValueName,
	IN ULONG  	ValueType,
	IN PVOID  	ValueData,
	IN ULONG  	ValueLength,
	IN PVOID  	Context,
	IN PVOID  	EntryContext 
)
{
    /* Add this to the certification list */
    return BasepSaveAppCertRegistryValue(Context, ValueName, ValueData);
}

NTSTATUS 
WINAPI 
BasepGetExeArchType(
	int ParameterOne, 
	HANDLE FileHandle, 
	PUNICODE_STRING string)
{
  DWORD one; // ebx@1
  signed __int16 receive; // di@1
  NTSTATUS status; // esi@1
  NTSTATUS error; // eax@20
  NTSTATUS otherStatus; // eax@23
  int SectionInformation; // [sp+Ch] [bp-68h]@1
  char Dst; // [sp+10h] [bp-64h]@1
  __int16 other = 0; // [sp+2Ch] [bp-48h]@3
  BOOLEAN verification = TRUE; // [sp+2Fh] [bp-45h]@4
  struct _SYSTEM_INFO SystemInfo; // [sp+3Ch] [bp-38h]@1
  int v14; // [sp+60h] [bp-14h]@1
  HANDLE handle; // [sp+64h] [bp-10h]@1
  PVOID SystemInformation; // [sp+68h] [bp-Ch]@1
  HANDLE SectionHandle; // [sp+6Ch] [bp-8h]@1

  SectionInformation = 0;
  memset(&Dst, 0, 0x2Cu);
  SystemInfo.dwOemId = 0;
  memset(&SystemInfo.dwPageSize, 0, 0x20u);
  one = 0x11000000u;
  SectionHandle = 0;
  receive = 0;
  SystemInformation = 0;
  handle = 0;
  v14 = 0;
  status = NtCreateSection(&SectionHandle, 1u, 0, 0, 2u, 0x11000000u, FileHandle);
  if ( status >= 0 )
    goto LABEL_31;
  do
  {
    if ( status != STATUS_NOT_SUPPORTED )
      break;
    if ( one != 0x11000000 )
      goto LABEL_6;
    one = 16777216;
    status = NtCreateSection(&SectionHandle, 1u, 0, 0, 2u, 0x1000000u, FileHandle);
  }
  while ( status < 0 );
  if ( status >= 0 )
  {
LABEL_31:
    status = NtQuerySection(SectionHandle, SectionImageInformation, &SectionInformation, 0x30u, 0);
    if ( status >= 0 )
    {
      receive = other;
      if ( other != 332
        || !(verification & 1)
        || (GetSystemInfo(&SystemInfo), SystemInfo.dwOemId != 9) && SystemInfo.dwOemId != 6 )
        goto LABEL_5;
      status = NtQuerySystemInformation(SystemVerifierInformation|SystemProcessorTimes, &SystemInformation, 4u, 0);
      if ( status >= 0 )
      {
        if ( (int)SystemInformation & 1 )
        {
          error = LdrOpenImageFileOptionsKey(string, 0, &handle);
          if ( error >= 0 )
          {
            otherStatus = LdrQueryImageFileKeyOption(handle, L"Wow64", 4, &v14, 4, 0);
            if ( otherStatus == STATUS_OBJECT_NAME_NOT_FOUND || otherStatus >= 0 && v14 )
            {
LABEL_26:
              if ( SystemInfo.dwOemId == 6 )
              {
                receive = 512;
              }
              else
              {
                if ( SystemInfo.dwOemId == 9 )
                  receive = 0x8664u;
              }
              goto LABEL_5;
            }
          }
          else
          {
            if ( error == STATUS_OBJECT_NAME_NOT_FOUND )
              goto LABEL_26;
          }
        }
LABEL_5:
        status = 0;
        goto LABEL_6;
      }
    }
  }
LABEL_6:
  if ( SectionHandle )
    NtClose(SectionHandle);
  ParameterOne = receive;
  return status;
}

NTSTATUS
WINAPI
BasepCheckWebBladeHashes(
	IN HANDLE FileHandle
)
{
    NTSTATUS Status;
    CHAR Hash[16];

    /* Get all the MD5 hashes */
    Status = RtlComputeImportTableHash(FileHandle, Hash, 1);
    if (!NT_SUCCESS(Status)) return Status;

    /* Depending on which suite this is, run a bsearch and block the appropriate ones */
    if (SharedUserData->SuiteMask & VER_SUITE_COMPUTE_SERVER)
    {
        TRACE("Egad! This is a ReactOS Compute Server and we should prevent you from using certain APIs...but we won't.");
    }
    else if (SharedUserData->SuiteMask & VER_SUITE_STORAGE_SERVER)
    {
        TRACE("Gasp! This is a ReactOS Storage Server and we should prevent you from using certain APIs...but we won't.");
    }
    else if (SharedUserData->SuiteMask & VER_SUITE_BLADE)
    {
        TRACE("Golly! This is a ReactOS Web Blade Server and we should prevent you from using certain APIs...but we won't.");
    }

    /* Actually, fuck it, don't block anything, we're open source */
    return STATUS_SUCCESS;
}

ULONG 
WINAPI 
BaseWriteErrorElevationRequiredEvent()
{
  ULONG result; // eax@1
  REGHANDLE handle; // [sp+4h] [bp-8h]@1  

  result = EtwEventRegister(MS_Windows_UAC_Provider, 0, 0, &handle);
  if ( !result )
  {
    EtwEventWrite(handle, UACUnhandledErrorElevationRequired, 0, 0);
    result = EtwEventUnregister(handle);
  }
  return result;
}

DWORD 
WINAPI 
BasepReportFault(
	DWORD parameterOne, 
	DWORD parameterTwo
)
{
  DWORD result; // eax@2

  if ( WerpReportExceptionInProcessContext(parameterOne) )
    result = 0;
  else
    result = 8;
  return result;
}

VOID
WINAPI
BaseThreadStartup(
    IN LPTHREAD_START_ROUTINE lpStartAddress,
    IN LPVOID lpParameter
    )

/*++

Routine Description:

    This function is called to start a Win32 thread. Its purpose
    is to call the thread, and if the thread returns, to terminate
    the thread and delete it's stack.

Arguments:

    lpStartAddress - Supplies the starting address of the new thread.  The
        address is logically a procedure that never returns and that
        accepts a single 32-bit pointer argument.

    lpParameter - Supplies a single parameter value passed to the thread.

Return Value:

    None.

--*/

{
    _SEH2_TRY {

        //
        // test for fiber start or new thread
        //

        //
        // WARNING WARNING DO NOT CHANGE INIT OF NtTib.Version. There is
        // external code depending on this initialization !
        //
		CsrNewThread();
        if ( NtCurrentTeb()->NtTib.Version == 7680 ) {
            if ( !BaseRunningInServerProcess ) {
                CsrNewThread();
                }
        }
        ExitThread((lpStartAddress)(lpParameter));
        }
    _SEH2_EXCEPT(UnhandledExceptionFilter( GetExceptionInformation() )) {
        if ( !BaseRunningInServerProcess ) {
            ExitProcess(GetExceptionCode());
            }
        else {
            ExitThread(GetExceptionCode());
            }
        }
	_SEH2_END;
}

LPTOP_LEVEL_EXCEPTION_FILTER GlobalTopLevelExceptionFilter;

static
LONG BaseExceptionFilter(EXCEPTION_POINTERS *ExceptionInfo)
{
    LONG ExceptionDisposition = EXCEPTION_EXECUTE_HANDLER;
    LPTOP_LEVEL_EXCEPTION_FILTER RealFilter;
    RealFilter = RtlDecodePointer(GlobalTopLevelExceptionFilter);

    if (RealFilter != NULL)
    {
        _SEH2_TRY
        {
            ExceptionDisposition = RealFilter(ExceptionInfo);
        }
        _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
        {
        }
        _SEH2_END;
    }
    if ((ExceptionDisposition == EXCEPTION_CONTINUE_SEARCH || ExceptionDisposition == EXCEPTION_EXECUTE_HANDLER) &&
        RealFilter != UnhandledExceptionFilter)
    {
       ExceptionDisposition = UnhandledExceptionFilter(ExceptionInfo);
    }

    return ExceptionDisposition;
}

VOID
WINAPI
BaseProcessStartup(PPROCESS_START_ROUTINE lpStartAddress)
{
    DbgPrint("BaseProcessStartup(..) - setting up exception frame.\n");

    _SEH2_TRY
    {
        /* Set our Start Address */
        NtSetInformationThread(NtCurrentThread(),
                               ThreadQuerySetWin32StartAddress,
                               &lpStartAddress,
                               sizeof(PPROCESS_START_ROUTINE));

        /* Call the Start Routine */
        ExitThread(lpStartAddress());
    }
    _SEH2_EXCEPT(BaseExceptionFilter(_SEH2_GetExceptionInformation()))
    {
        /* Get the Exit code from the SEH Handler */
        if (!BaseRunningInServerProcess)
        {
            /* Kill the whole process, usually */
            ExitProcess(_SEH2_GetExceptionCode());
        }
        else
        {
            /* If running inside CSRSS, kill just this thread */
            ExitThread(_SEH2_GetExceptionCode());
        }
    }
    _SEH2_END;
}

static
LONG BaseThreadExceptionFilter(EXCEPTION_POINTERS * ExceptionInfo)
{
   LONG ExceptionDisposition = EXCEPTION_EXECUTE_HANDLER;
   LPTOP_LEVEL_EXCEPTION_FILTER RealFilter;
   RealFilter = RtlDecodePointer(GlobalTopLevelExceptionFilter);

   if (RealFilter != NULL)
   {
      _SEH2_TRY
      {
         ExceptionDisposition = RealFilter(ExceptionInfo);
      }
      _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
      {
         ExceptionDisposition = UnhandledExceptionFilter(ExceptionInfo);
      }
      _SEH2_END;
   }

   return ExceptionDisposition;
}

__declspec(noreturn)
VOID
WINAPI
BasepThreadStartup(IN LPTHREAD_START_ROUTINE lpStartAddress,
                  IN LPVOID lpParameter)
{
    /* Attempt to call the Thread Start Address */
    _SEH2_TRY
    {
        /* Legacy check which is still used today for Win32 threads */
        if (NtCurrentTeb()->NtTib.Version == (30 << 8)) // OS/2 V3.0 ("Cruiser")
        {
            /* This registers the termination port with CSRSS */
            if (!BaseRunningInServerProcess) CsrNewThread();
        }

        /* Get the exit code from the Thread Start */
        ExitThread((lpStartAddress)((PVOID)lpParameter));
    }
    _SEH2_EXCEPT(BaseThreadExceptionFilter(_SEH2_GetExceptionInformation()))
    {
        /* Get the Exit code from the SEH Handler */
        if (!BaseRunningInServerProcess)
        {
            /* Kill the whole process, usually */
            ExitProcess(_SEH2_GetExceptionCode());
        }
        else
        {
            /* If running inside CSRSS, kill just this thread */
            ExitThread(_SEH2_GetExceptionCode());
        }
    }
    _SEH2_END;
}

void
Report32bitAppLaunching ()
/*++

Routine Description:

      This routine is called whenever a 32-bit is launched on win64 (inside Wow64). It will
      check to see if application loggin is enabled, and if so, will log an event to the 
      application log.
      
Arguments:

    None.

Return Value:

    None.
--*/

{

    
    const static UNICODE_STRING KeyName = RTL_CONSTANT_STRING  (L"\\REGISTRY\\MACHINE\\SOFTWARE\\Microsoft\\wow64\\config");
    static UNICODE_STRING ValueName = RTL_CONSTANT_STRING  (L"LogAppLaunchingEvent");
    static OBJECT_ATTRIBUTES ObjA = RTL_CONSTANT_OBJECT_ATTRIBUTES (&KeyName, OBJ_CASE_INSENSITIVE);
    
    WCHAR Buffer [MAX_PATH];
    const PWCHAR Msg [2] = {Buffer, NULL};
    PWCHAR pAppName;
    HKEY  hKey;
    PUNICODE_STRING ImageName;
    PKEY_VALUE_PARTIAL_INFORMATION KeyValueInformation;
    NTSTATUS Status;
    PPEB Peb;
    PWSTR PtrToCopy;
    DWORD CopyLength;

    
    Status = NtOpenKey (&hKey, KEY_READ | KEY_WOW64_64KEY, &ObjA);

    if (NT_SUCCESS(Status)) {
        
        KeyValueInformation = (PKEY_VALUE_PARTIAL_INFORMATION)Buffer;
  
        Status = NtQueryValueKey(
                                hKey,
                                &ValueName,
                                KeyValuePartialInformation,
                                KeyValueInformation,
                                sizeof (Buffer),
                                &CopyLength
                                );
        NtClose (hKey);

        if ((NT_SUCCESS (Status)) && 
            (KeyValueInformation->Type == REG_DWORD) &&
            (KeyValueInformation->DataLength == sizeof (DWORD))) {

            if (*(LONG *)KeyValueInformation->Data == 0x1) {

                Peb = NtCurrentPeb ();
                PtrToCopy = NULL;

                if (Peb->ProcessParameters != NULL) {

                    ImageName = &Peb->ProcessParameters->ImagePathName;

                    ASSERT (ImageName->Buffer != NULL);
                    if (ImageName->Length > (sizeof (Buffer) - sizeof (UNICODE_NULL))) {
                        
                        CopyLength = (sizeof (Buffer) - sizeof (UNICODE_NULL));
                        PtrToCopy = (PWSTR)((PSTR)ImageName->Buffer + (ImageName->Length - sizeof (Buffer)) + sizeof (UNICODE_NULL));
                    } else {

                        CopyLength = ImageName->Length;
                        PtrToCopy = ImageName->Buffer;
                    }

                    if ( CopyLength){
                        RtlCopyMemory (Buffer, PtrToCopy, CopyLength);
                        Buffer [(CopyLength >> 1)] = UNICODE_NULL;
                    } else swprintf (Buffer, L"PID=%d",NtCurrentTeb()->ClientId.UniqueProcess);  
                }
                //Wow64LogMessageInEventLogger ((PWCHAR *)Msg);
            }
        }
    }
}

// VOID
// BaseThreadStart(
    // IN LPTHREAD_START_ROUTINE lpStartAddress,
    // IN LPVOID lpParameter
    // )

// /*++

// Routine Description:

    // This function is called to start a Win32 thread. Its purpose
    // is to call the thread, and if the thread returns, to terminate
    // the thread and delete its stack.

// Arguments:

    // lpStartAddress - Supplies the starting address of the new thread.  The
        // address is logically a procedure that never returns and that
        // accepts a single 32-bit pointer argument.

    // lpParameter - Supplies a single parameter value passed to the thread.

// Return Value:

    // None.

// --*/

// {
    // _SEH2_TRY {

        
        // // test for fiber start or new thread
        

        
        // // WARNING WARNING DO NOT CHANGE INIT OF NtTib.Version. There is
        // // external code depending on this initialization !
        
        // if ( NtCurrentTeb()->NtTib.Version == 7680) {//OS2_VERSION ) {
            // if ( !BaseRunningInServerProcess ) {
                // CsrNewThread();
                // }
            // }
        // ExitThread((lpStartAddress)(lpParameter));
        // }
    // _SEH2_EXCEPT(UnhandledExceptionFilter( GetExceptionInformation() )) {
        // if ( !BaseRunningInServerProcess ) {
            // ExitProcess(GetExceptionCode());
            // }
        // else {
            // ExitThread(GetExceptionCode());
            // }
        // }
	// _SEH2_END
// }

// VOID
// BaseProcessStart(
    // PPROCESS_START_ROUTINE lpStartAddress
    // )

// /*++

// Routine Description:

    // This function is called to start a Win32 process.  Its purpose is to
    // call the initial thread of the process, and if the thread returns,
    // to terminate the thread and delete its stack.

// Arguments:

    // lpStartAddress - Supplies the starting address of the new thread.  The
        // address is logically a procedure that never returns.

// Return Value:

    // None.

// --*/

// {
    // _SEH2_TRY {
#if defined(BUILD_WOW6432)        
        // Report32bitAppLaunching ();
#endif 
        // NtSetInformationThread( NtCurrentThread(),
                                // ThreadQuerySetWin32StartAddress,
                                // &lpStartAddress,
                                // sizeof( lpStartAddress )
                              // );
        // ExitThread((lpStartAddress)());
        // }
    // _SEH2_EXCEPT(UnhandledExceptionFilter( GetExceptionInformation() )) {
        // if ( !BaseRunningInServerProcess ) {
            // ExitProcess(GetExceptionCode());
            // }
        // else {
            // ExitThread(GetExceptionCode());
            // }
        // }
// }


VOID
BasepFreeAppCompatData(
    PVOID  pAppCompatData,
    SIZE_T cbAppCompatData,
    PVOID  pSxsData,
    SIZE_T cbSxsData
    )
{
    if (pAppCompatData) {
        NtFreeVirtualMemory(NtCurrentProcess(),
                            &pAppCompatData,
                            &cbAppCompatData,
                            MEM_RELEASE);
    }

    if (pSxsData) {
        NtFreeVirtualMemory(NtCurrentProcess(),
                            &pSxsData,
                            &cbSxsData,
                            MEM_RELEASE);

    }
}