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
#include <basemsg.h>

static BOOL DllInitialized = FALSE;
PPEB Peb;
HMODULE hCurrentModule = NULL;
HMODULE kernel32_handle = NULL;
ULONG BaseDllTag;
ULONG SessionId;
UNICODE_STRING BaseDefaultPath;
BOOLEAN BaseRunningInServerProcess = FALSE;

/* Critical section for various kernel32 data structures */
RTL_CRITICAL_SECTION BaseDllDirectoryLock;

#define WIN_OBJ_DIR L"\\Windows"
#define SESSION_DIR L"\\Sessions"

extern BOOL RegInitialize(VOID);
extern BOOL RegCleanup(VOID);
void InitializeCriticalForLocaleInfo();
void init_locale(void);

RTL_HANDLE_TABLE BaseHeapHandleTable;
HANDLE BaseHeap;
ULONG_PTR SystemRangeStart;


UNICODE_STRING BaseWindowsDirectory, BaseWindowsSystemDirectory;

VOID
NTAPI
LdrSetDllManifestProber(
    _In_ PLDR_MANIFEST_PROBER_ROUTINE Routine);
	

NTSTATUS
NTAPI
BaseCreateThreadPoolThread(IN PTHREAD_START_ROUTINE Function,
                           IN PVOID Parameter,
                           OUT PHANDLE ThreadHandle)
{
    NTSTATUS Status;

    /* Create a Win32 thread */
    *ThreadHandle = CreateRemoteThread(NtCurrentProcess(),
                                       NULL,
                                       0,
                                       Function,
                                       Parameter,
                                       CREATE_SUSPENDED,
                                       NULL);
    if (!(*ThreadHandle))
    {
        /* Get the status value if we couldn't get a handle */
        Status = NtCurrentTeb()->LastStatusValue;
        if (NT_SUCCESS(Status)) Status = STATUS_UNSUCCESSFUL;
    }
    else
    {
        /* Set success code */
        Status = STATUS_SUCCESS;
    }

    /* All done */
    return Status;
}

NTSTATUS
NTAPI
BaseExitThreadPoolThread(IN NTSTATUS ExitStatus)
{
    /* Exit the thread */
    ExitThread(ExitStatus);
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
BasepProbeForDllManifest(IN PVOID DllHandle,
                         IN PCWSTR FullDllName,
                         OUT PVOID *ActCtx)
{
    NTSTATUS Status = STATUS_SUCCESS;
    LDR_RESOURCE_INFO Info;
    IMAGE_RESOURCE_DATA_ENTRY *Entry;
    ACTCTXW Context;
    HANDLE Result;

    /* Check if activation context parameter is provided */
    if (!ActCtx)
    {
        ASSERT(FALSE);
        return STATUS_INVALID_PARAMETER;
    }

    /* Zero it out */
    *ActCtx = NULL;

    /* Check whether the image has manifest resource associated with it */
    Info.Type = (ULONG_PTR)RT_MANIFEST;
    Info.Name = (ULONG_PTR)ISOLATIONAWARE_MANIFEST_RESOURCE_ID;
    Info.Language = 0;
    if (!(Status = LdrFindResource_U(DllHandle, &Info, 3, &Entry)))
    {
        /* Create the activation context */
        Context.cbSize = sizeof(Context);
        Context.lpSource = FullDllName;
        Context.dwFlags = ACTCTX_FLAG_RESOURCE_NAME_VALID | ACTCTX_FLAG_HMODULE_VALID;
        Context.hModule = DllHandle;
        Context.lpResourceName = (LPCWSTR)ISOLATIONAWARE_MANIFEST_RESOURCE_ID;

        Status = RtlCreateActivationContext(0, (PVOID)&Context, 0, NULL, NULL, &Result);

        /* Store activation context pointer if it was created successfully */
        if (NT_SUCCESS(Status)) *ActCtx = Result;

        /* CORE-10843: Windows always returns this since we pass the wrong struct */
        if (Status == STATUS_SXS_INVALID_ACTCTXDATA_FORMAT)
        {
            /* Fake "Manifest not found" so the load doesn't fail  */
            static int Once;
            if (Once++)
            {
                DbgPrint("HACK: Passed invalid ACTIVATION_CONTEXT_DATA!\n");
            }
            Status = STATUS_RESOURCE_DATA_NOT_FOUND;
        }
    }

    return Status;
}

VOID
NTAPI
BaseDllInitializeMemoryManager(VOID)
{
    BaseHeap = RtlGetProcessHeap();
    RtlInitializeHandleTable(0xFFFF,
                             sizeof(BASE_HEAP_HANDLE_ENTRY),
                             &BaseHeapHandleTable);
    NtQuerySystemInformation(SystemRangeStartInformation,
                             &SystemRangeStart,
                             sizeof(SystemRangeStart),
                             NULL);
}

BOOL
WINAPI
BaseDllInitialize(
	HANDLE hDll,
    DWORD dwReason,
    LPVOID lpReserved)
{
    NTSTATUS Status;
    BASESRV_API_CONNECTINFO ConnectInfo;
    ULONG ConnectInfoSize = sizeof(ConnectInfo);
    WCHAR SessionDir[256];
//#endif
    //WCHAR szSessionDir[MAX_SESSION_PATH];


    Peb = NtCurrentPeb();

    SessionId = Peb->SessionId;

    //BaseDllHandle = DllHandle;

    /* Cache the PEB and Session ID */
    //Peb = NtCurrentPeb();

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            /* Insert more dll attach stuff here! */
			kernel32_handle = GetModuleHandleW(L"kernelex");
			InitializeCriticalForLocaleInfo();
			//RegInitialize();
			
			//Initialize Locale
			init_locale();
			
            /* Set no filter initially */
            //GlobalTopLevelExceptionFilter = RtlEncodePointer(NULL);
            
            /* Enable the Rtl thread pool and timer queue to use proper Win32 thread */
            RtlSetThreadPoolStartFunc(BaseCreateThreadPoolThread, BaseExitThreadPoolThread);

            /* Register the manifest prober routine */
            LdrSetDllManifestProber(BasepProbeForDllManifest);

            /* Don't bother us for each thread */
            LdrDisableThreadCalloutsForDll((PVOID)hDll);

            /* Initialize default path to NULL */
            RtlInitUnicodeString(&BaseDefaultPath, NULL);

            /* Setup the Object Directory path */
            if (!SessionId)
            {
                /* Use the raw path */
                wcscpy(SessionDir, WIN_OBJ_DIR);
            }
            else
            {
                /* Use the session path */
                swprintf(SessionDir,
                         L"%ws\\%ld%ws",
                         SESSION_DIR,
                         SessionId,
                         WIN_OBJ_DIR);
            }

            /* Connect to the Base Server */
            Status = CsrClientConnectToServer(SessionDir,
                                              BASESRV_SERVERDLL_INDEX,
                                              &ConnectInfo,
                                              &ConnectInfoSize,
                                              &BaseRunningInServerProcess);
            if (!NT_SUCCESS(Status))
            {
                DbgPrint("Failed to connect to CSR (Status %lx)\n", Status);
                NtTerminateProcess(NtCurrentProcess(), Status);
                return FALSE;
            }

            /* Get the server data */
            ASSERT(Peb->ReadOnlyStaticServerData);
            BaseStaticServerData = Peb->ReadOnlyStaticServerData[BASESRV_SERVERDLL_INDEX];
            ASSERT(BaseStaticServerData);

            /* Check if we are running a CSR Server */
            if (!BaseRunningInServerProcess)
            {
                /* Set the termination port for the thread */
                DbgPrint("Creating new thread for CSR\n");
                CsrNewThread();
            }

            /* Initialize heap handle table */
            BaseDllInitializeMemoryManager();

            /* Set HMODULE for our DLL */
            kernel32_handle = hCurrentModule = hDll;

            /* Set the directories */
            BaseWindowsDirectory = BaseStaticServerData->WindowsDirectory;
            BaseWindowsSystemDirectory = BaseStaticServerData->WindowsSystemDirectory;

            /* Construct the default path (using the static buffer) */
            Status = RtlStringCbPrintfW(BaseDefaultPathBuffer,
                                     sizeof(BaseDefaultPathBuffer),
                                     L".;%wZ;%wZ\\system;%wZ;",
                                     &BaseWindowsSystemDirectory,
                                     &BaseWindowsDirectory,
                                     &BaseWindowsDirectory);
            if (!NT_SUCCESS(Status))
            {
                DbgPrint("NLS Init failed\n");
                return FALSE;
            }

            BaseDefaultPath.Buffer = BaseDefaultPathBuffer;
            BaseDefaultPath.Length = (USHORT)wcslen(BaseDefaultPathBuffer) * sizeof(WCHAR);
            BaseDefaultPath.MaximumLength = sizeof(BaseDefaultPathBuffer);

            /* Use remaining part of the default path buffer for the append path */
            BaseDefaultPathAppend.Buffer = (PWSTR)((ULONG_PTR)BaseDefaultPathBuffer + BaseDefaultPath.Length);
            BaseDefaultPathAppend.Length = 0;
            BaseDefaultPathAppend.MaximumLength = BaseDefaultPath.MaximumLength - BaseDefaultPath.Length;

            /* Initialize command line */
            InitCommandLines();

            /* Initialize the DLL critical section */
            RtlInitializeCriticalSection(&BaseDllDirectoryLock);

            /* Initialize the National Language Support routines */
            if (!NlsInit())
            {
                DbgPrint("NLS Init failed\n");
                return FALSE;
            }

            /* Initialize Console Support */
            if (!ConDllInitialize(dwReason, SessionDir))
            {
                DbgPrint("Failed to set up console\n");
                return FALSE;
            }

            /* Initialize application certification globals */
            InitializeListHead(&BasepAppCertDllsList);
            RtlInitializeCriticalSection(&gcsAppCert);

            /* Insert more dll attach stuff here! */
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