/*++
Copyright (c) 1989-2001  Microsoft Corporation
Module Name:
    werapi.h
Abstract:
    This file contains the function prototypes for Windows Error Reporting (WER)
Notes:
    
--*/

#pragma once

#pragma region Desktop Family

//////////////////////// Defines //////////////////////////////////////////////

typedef HANDLE HREPORT;

//
// Do not add heap dumps for reports for the process
//
#define WER_FAULT_REPORTING_FLAG_NOHEAP     1

//
// Queue critical reports for this process
//
#define WER_FAULT_REPORTING_FLAG_QUEUE      2

//
// Do not suspend the process before error reporting
//
#define WER_FAULT_REPORTING_FLAG_DISABLE_THREAD_SUSPENSION   4

//
// Queue critical reports for this process and upload from the queue
//
#define WER_FAULT_REPORTING_FLAG_QUEUE_UPLOAD      8

//
// Fault reporting UI should always be shown. This is only applicable for interactive processes
//
#define WER_FAULT_REPORTING_ALWAYS_SHOW_UI          16

//
// Fault reporting UI should not be shown.
//
#define WER_FAULT_REPORTING_NO_UI                   32

//
// Do not add heap dumps when queueing reports for the process
//
#define WER_FAULT_REPORTING_FLAG_NO_HEAP_ON_QUEUE   64

//
// Disable snapshots for crash/exception reporting.
//
#define WER_FAULT_REPORTING_DISABLE_SNAPSHOT_CRASH  128

//
// Disable snapshots for hang reporting.
//
#define WER_FAULT_REPORTING_DISABLE_SNAPSHOT_HANG   256

//
// Mark the process as critical.
//
#define WER_FAULT_REPORTING_CRITICAL                512

//
// Mark the process as requiring flushing of its report store.
//
#define WER_FAULT_REPORTING_DURABLE                 1024

// 
// Maximum length of the report event name
//
#define WER_MAX_EVENT_NAME_LENGTH 64

//
// Maximum length of the name of a signature element
//
#define WER_MAX_SIGNATURE_NAME_LENGTH 128

//
// This is the maximum length of any created URL
// NOTE: This constant is obsolete since Win9.
//
#define WER_MAX_TOTAL_PARAM_LENGTH      1720

//
// This is the maximum length of any parameter including the NULL character
//
#define WER_MAX_PARAM_LENGTH            (MAX_PATH)

//
// Number of extra modules that we can select to get extra data in the minidump
// 
#define WER_MAX_PREFERRED_MODULES           128
#define WER_MAX_PREFERRED_MODULES_BUFFER    256

//
// Maximum number of parameters for a report
//
#define WER_MAX_PARAM_COUNT 10

#pragma endregion

#pragma region Application Family

//
// The maximum size of memory block that can be registered
//
#define WER_MAX_MEM_BLOCK_SIZE (64 * 1024)

#pragma endregion

#pragma region Desktop Family
//
// Event Type constants for application crashes
//
#define APPCRASH_EVENT          L"APPCRASH"
#define PACKAGED_APPCRASH_EVENT L"MoAppCrash"

// Indexes for the parameter ids
#define WER_P0  0
#define WER_P1  1
#define WER_P2  2
#define WER_P3  3
#define WER_P4  4
#define WER_P5  5
#define WER_P6  6
#define WER_P7  7
#define WER_P8  8
#define WER_P9  9

#pragma endregion

#pragma region Application Family
//#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)

//
// Custom error HRESULTS
//
#define WER_E_INSUFFICIENT_BUFFER           (HRESULT_FROM_WIN32(ERROR_INSUFFICIENT_BUFFER))
#define WER_E_NOT_FOUND                     (HRESULT_FROM_WIN32(ERROR_NOT_FOUND))
#define WER_E_LENGTH_EXCEEDED               (HRESULT_FROM_WIN32(ERROR_PARAMETER_QUOTA_EXCEEDED))
#define WER_E_INVALID_STATE                 (HRESULT_FROM_WIN32(ERROR_INVALID_STATE))

#pragma endregion

#pragma region Desktop Family
//#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

///////////////////////////////////////////////////////////////////////////////

//
// The enum that describes the indexes of the customizable UI strings
// 
typedef enum _WER_REPORT_UI
{
    WerUIAdditionalDataDlgHeader = 1,
    WerUIIconFilePath = 2,
    WerUIConsentDlgHeader = 3,
    WerUIConsentDlgBody = 4,
    WerUIOnlineSolutionCheckText = 5,
    WerUIOfflineSolutionCheckText = 6,
    WerUICloseText = 7,
    WerUICloseDlgHeader = 8,
    WerUICloseDlgBody = 9,
    WerUICloseDlgButtonText = 10,
    WerUIMax
} WER_REPORT_UI;

//#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */
#pragma endregion

#pragma region Application Family
//#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)

//
// The type of the registered files
//
typedef enum _WER_REGISTER_FILE_TYPE
{   
    WerRegFileTypeUserDocument = 1,
    WerRegFileTypeOther = 2,
    WerRegFileTypeMax
} WER_REGISTER_FILE_TYPE;

////#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP) */
#pragma endregion

#pragma region Desktop Family
//#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

//
// The type of files that can be added to the report
// 
typedef enum _WER_FILE_TYPE
{   
    WerFileTypeMicrodump = 1,
    WerFileTypeMinidump = 2,
    WerFileTypeHeapdump = 3,
    WerFileTypeUserDocument = 4,
    WerFileTypeOther = 5,
    WerFileTypeTriagedump = 6,
    WerFileTypeCustomDump = 7,
    WerFileTypeAuxiliaryDump = 8,
    WerFileTypeEtlTrace = 9,
    WerFileTypeMax
} WER_FILE_TYPE;

typedef enum _WER_SUBMIT_RESULT
{
    WerReportQueued = 1,
    WerReportUploaded = 2,
    WerReportDebug = 3,
    WerReportFailed = 4,
    WerDisabled = 5,
    WerReportCancelled = 6,
    WerDisabledQueue = 7,
    WerReportAsync = 8,
    WerCustomAction = 9,
#if (NTDDI_VERSION >= NTDDI_WIN8)
    WerThrottled = 10,
#endif
#if (NTDDI_VERSION >= NTDDI_WIN10)
    WerReportUploadedCab = 11,
    WerStorageLocationNotFound = 12,
#endif
    WerSubmitResultMax
} WER_SUBMIT_RESULT, *PWER_SUBMIT_RESULT;

//
// The type of the report
//
typedef enum _WER_REPORT_TYPE
{   
    WerReportNonCritical = 0,
    WerReportCritical = 1,
    WerReportApplicationCrash = 2,
    WerReportApplicationHang = 3,
    WerReportKernel = 4,
    WerReportInvalid
} WER_REPORT_TYPE;

//#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */
#pragma endregion

#pragma region Application Family
//#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)

//
// Flags that can be specified when adding a file to the report. 
//
// NOTE: These should always occupy the lower 16 bits of the file flag dword. 
// The upper 16 bits are reserved for internal use and get cleared by WerAddFile.
//
#define WER_FILE_DELETE_WHEN_DONE       1  // Delete the file once WER is done
#define WER_FILE_ANONYMOUS_DATA         2  // This file does not contain any PII

////#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP) */
#pragma endregion

#pragma region Desktop Family
//#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

//
// Report submission flags. 
//                     
#define WER_SUBMIT_HONOR_RECOVERY               1       // show recovery option
#define WER_SUBMIT_HONOR_RESTART                2       // show application restart option
#define WER_SUBMIT_QUEUE                        4       // report directly to queue
#define WER_SUBMIT_SHOW_DEBUG                   8       // show the debug button
#define WER_SUBMIT_ADD_REGISTERED_DATA          16      // Add registered data to the WER report 
#define WER_SUBMIT_OUTOFPROCESS                 32      // Force the report to go out of process
#define WER_SUBMIT_NO_CLOSE_UI                  64      // Do not show the close dialog for the critical report
#define WER_SUBMIT_NO_QUEUE                     128     // Do not queue the report
#define WER_SUBMIT_NO_ARCHIVE                   256     // Do not archive the report
#define WER_SUBMIT_START_MINIMIZED              512     // The initial reporting UI is minimized and will flash
#define WER_SUBMIT_OUTOFPROCESS_ASYNC           1024    // Force the report to go out of process and do not wait for it to finish
#define WER_SUBMIT_BYPASS_DATA_THROTTLING       2048    // Bypass data throttling for the report
#define WER_SUBMIT_ARCHIVE_PARAMETERS_ONLY      4096    // Archive only the parameters; the cab is discarded
#define WER_SUBMIT_REPORT_MACHINE_ID            8192    // Always send the machine ID, regardless of the consent the report was submitted with
#define WER_SUBMIT_BYPASS_POWER_THROTTLING      16384   // Bypass power-related throttling (when on battery)
#define WER_SUBMIT_BYPASS_NETWORK_COST_THROTTLING 32768 // Bypass network-related throttling (when on restricted networks)

#define WER_SUBMIT_DISCARD_IF_QUEUED            WER_SUBMIT_NO_QUEUE

typedef struct _WER_REPORT_INFORMATION
{
    DWORD dwSize;
    HANDLE hProcess;
    WCHAR wzConsentKey[64];
    WCHAR wzFriendlyEventName[128];
    WCHAR wzApplicationName[128];
    WCHAR wzApplicationPath[MAX_PATH];
    WCHAR wzDescription[512];
    HWND hwndParent;    

} WER_REPORT_INFORMATION, *PWER_REPORT_INFORMATION;

#if (NTDDI_VERSION >= NTDDI_WIN8)
typedef struct _WER_REPORT_INFORMATION_V3
{
    DWORD dwSize;
    HANDLE hProcess;
    WCHAR wzConsentKey[64];
    WCHAR wzFriendlyEventName[128];
    WCHAR wzApplicationName[128];
    WCHAR wzApplicationPath[MAX_PATH];
    WCHAR wzDescription[512];
    HWND hwndParent;    
    WCHAR wzNamespacePartner[64];
    WCHAR wzNamespaceGroup[64];
} WER_REPORT_INFORMATION_V3, *PWER_REPORT_INFORMATION_V3;
#endif // NTDDI_VERSION >= NTDDI_WIN8

typedef struct _WER_DUMP_CUSTOM_OPTIONS 
{
    DWORD dwSize;   
    DWORD dwMask;
    DWORD dwDumpFlags;
    BOOL  bOnlyThisThread;
    DWORD dwExceptionThreadFlags;
    DWORD dwOtherThreadFlags;
    DWORD dwExceptionThreadExFlags;
    DWORD dwOtherThreadExFlags;
    DWORD dwPreferredModuleFlags;
    DWORD dwOtherModuleFlags;
    WCHAR wzPreferredModuleList[WER_MAX_PREFERRED_MODULES_BUFFER];
    
} WER_DUMP_CUSTOM_OPTIONS, *PWER_DUMP_CUSTOM_OPTIONS;

typedef struct _WER_DUMP_CUSTOM_OPTIONS_V2
{
    DWORD dwSize;   
    DWORD dwMask;
    DWORD dwDumpFlags;
    BOOL  bOnlyThisThread;
    DWORD dwExceptionThreadFlags;
    DWORD dwOtherThreadFlags;
    DWORD dwExceptionThreadExFlags;
    DWORD dwOtherThreadExFlags;
    DWORD dwPreferredModuleFlags;
    DWORD dwOtherModuleFlags;
    WCHAR wzPreferredModuleList[WER_MAX_PREFERRED_MODULES_BUFFER];
    DWORD dwPreferredModuleResetFlags;
    DWORD dwOtherModuleResetFlags;
} WER_DUMP_CUSTOM_OPTIONS_V2, *PWER_DUMP_CUSTOM_OPTIONS_V2;

#if (NTDDI_VERSION >= NTDDI_WINBLUE)
typedef struct _WER_REPORT_INFORMATION_V4
{
    DWORD dwSize;
    HANDLE hProcess;
    WCHAR wzConsentKey[64];
    WCHAR wzFriendlyEventName[128];
    WCHAR wzApplicationName[128];
    WCHAR wzApplicationPath[MAX_PATH];
    WCHAR wzDescription[512];
    HWND hwndParent;    
    WCHAR wzNamespacePartner[64];
    WCHAR wzNamespaceGroup[64];
    BYTE rgbApplicationIdentity[16];
    HANDLE hSnapshot;
    HANDLE hDeleteFilesImpersonationToken;
} WER_REPORT_INFORMATION_V4, *PWER_REPORT_INFORMATION_V4;
typedef WER_REPORT_INFORMATION_V4 const* PCWER_REPORT_INFORMATION_V4;
#endif // NTDDI_VERSION >= NTDDI_WINBLUE

#if (NTDDI_VERSION >= NTDDI_WIN10)
typedef struct _WER_REPORT_INFORMATION_V5
{
    DWORD dwSize;
    HANDLE hProcess;
    WCHAR wzConsentKey[64];
    WCHAR wzFriendlyEventName[128];
    WCHAR wzApplicationName[128];
    WCHAR wzApplicationPath[MAX_PATH];
    WCHAR wzDescription[512];
    HWND hwndParent;    
    WCHAR wzNamespacePartner[64];
    WCHAR wzNamespaceGroup[64];
    BYTE rgbApplicationIdentity[16];
    HANDLE hSnapshot;
    HANDLE hDeleteFilesImpersonationToken;
    WER_SUBMIT_RESULT submitResultMax;
} WER_REPORT_INFORMATION_V5, *PWER_REPORT_INFORMATION_V5;
typedef WER_REPORT_INFORMATION_V5 const *PCWER_REPORT_INFORMATION_V5;
#endif // NTDDI_VERSION >= NTDDI_WIN10

#if (NTDDI_VERSION >= NTDDI_WINBLUE)
typedef struct _WER_DUMP_CUSTOM_OPTIONS_V3
{
    DWORD dwSize;   
    DWORD dwMask;
    DWORD dwDumpFlags;
    BOOL  bOnlyThisThread;
    DWORD dwExceptionThreadFlags;
    DWORD dwOtherThreadFlags;
    DWORD dwExceptionThreadExFlags;
    DWORD dwOtherThreadExFlags;
    DWORD dwPreferredModuleFlags;
    DWORD dwOtherModuleFlags;
    WCHAR wzPreferredModuleList[WER_MAX_PREFERRED_MODULES_BUFFER];
    DWORD dwPreferredModuleResetFlags;
    DWORD dwOtherModuleResetFlags;

    PVOID pvDumpKey;
    HANDLE hSnapshot;
    DWORD dwThreadID;
} WER_DUMP_CUSTOM_OPTIONS_V3, *PWER_DUMP_CUSTOM_OPTIONS_V3;
typedef WER_DUMP_CUSTOM_OPTIONS_V3 const* PCWER_DUMP_CUSTOM_OPTIONS_V3;
#endif // NTDDI_VERSION >= NTDDI_WINBLUE

typedef struct _WER_EXCEPTION_INFORMATION
{   
    PEXCEPTION_POINTERS pExceptionPointers;
    BOOL bClientPointers;
} WER_EXCEPTION_INFORMATION, *PWER_EXCEPTION_INFORMATION;

typedef enum _WER_CONSENT
{
    WerConsentNotAsked = 1,
    WerConsentApproved = 2,
    WerConsentDenied = 3,
    WerConsentAlwaysPrompt = 4,
    WerConsentMax
}WER_CONSENT;

HRESULT
WINAPI
WerReportCreate(
    _In_ PCWSTR pwzEventType, 
    _In_ WER_REPORT_TYPE repType,
    _In_opt_ PWER_REPORT_INFORMATION pReportInformation,
    _Out_ HREPORT *phReportHandle
    );
    
HRESULT
WINAPI
WerReportSetParameter(
    _In_ HREPORT hReportHandle, 
    _In_ DWORD dwparamID, 
    _In_opt_ PCWSTR pwzName,
    _In_ PCWSTR pwzValue
    );
    
HRESULT
WINAPI
WerReportAddFile(
    _In_ HREPORT hReportHandle, 
    _In_ PCWSTR pwzPath, 
    _In_ WER_FILE_TYPE repFileType,
    _In_ DWORD  dwFileFlags
    );

HRESULT  
WINAPI
WerReportSetUIOption(
    _In_ HREPORT hReportHandle, 
    _In_ WER_REPORT_UI repUITypeID, 
    _In_ PCWSTR pwzValue
    );

HRESULT 
WINAPI
WerReportSubmit(
    _In_ HREPORT hReportHandle,
    _In_ WER_CONSENT consent,
    _In_ DWORD  dwFlags,
    _Out_opt_ PWER_SUBMIT_RESULT pSubmitResult
    );

//
// Masks to be used for customizing the dump
//
#define WER_DUMP_MASK_START 1
#define WER_DUMP_MASK_DUMPTYPE               (WER_DUMP_MASK_START << 0)
#define WER_DUMP_MASK_ONLY_THISTHREAD        (WER_DUMP_MASK_START << 1)
#define WER_DUMP_MASK_THREADFLAGS            (WER_DUMP_MASK_START << 2)
#define WER_DUMP_MASK_THREADFLAGS_EX         (WER_DUMP_MASK_START << 3)
#define WER_DUMP_MASK_OTHERTHREADFLAGS       (WER_DUMP_MASK_START << 4)
#define WER_DUMP_MASK_OTHERTHREADFLAGS_EX    (WER_DUMP_MASK_START << 5)
#define WER_DUMP_MASK_PREFERRED_MODULESFLAGS (WER_DUMP_MASK_START << 6)
#define WER_DUMP_MASK_OTHER_MODULESFLAGS     (WER_DUMP_MASK_START << 7)
#define WER_DUMP_MASK_PREFERRED_MODULE_LIST  (WER_DUMP_MASK_START << 8)

//
// WER dump flags
//
#define WER_DUMP_NOHEAP_ONQUEUE 1
#define WER_DUMP_AUXILIARY      2

//
// WER dump types
//
typedef enum _WER_DUMP_TYPE
{
    WerDumpTypeMicroDump = 1,
    WerDumpTypeMiniDump = 2,
    WerDumpTypeHeapDump = 3,
    WerDumpTypeTriageDump = 4,
    WerDumpTypeMax = 5
} WER_DUMP_TYPE;

HRESULT  
WINAPI
WerReportAddDump(
    _In_ HREPORT hReportHandle, 
    _In_ HANDLE  hProcess,
    _In_opt_ HANDLE hThread,
    _In_ WER_DUMP_TYPE dumpType,
    _In_opt_  PWER_EXCEPTION_INFORMATION pExceptionParam,
    _In_opt_ PWER_DUMP_CUSTOM_OPTIONS pDumpCustomOptions,
    _In_ DWORD dwFlags
    );

HRESULT 
WINAPI
WerReportCloseHandle(
    _In_ HREPORT hReportHandle
    );
    

//
// ++++++++++++++++++++++++++ Registration APIs ++++++++++++++++++++++++++++++++++++++
//

#define WER_MAX_REGISTERED_ENTRIES 512

//#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */
#pragma endregion

#pragma region Application Family
//#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP)

HRESULT 
WINAPI
WerRegisterFile(
    _In_ PCWSTR pwzFile, 
    _In_ WER_REGISTER_FILE_TYPE regFileType,
    _In_ DWORD dwFlags
    );

HRESULT 
WINAPI
WerUnregisterFile(
    _In_ PCWSTR pwzFilePath
    );

HRESULT
WINAPI
WerRegisterMemoryBlock(
    _In_ PVOID pvAddress,
    _In_ DWORD dwSize
    );

HRESULT 
WINAPI
WerUnregisterMemoryBlock(
    _In_ PVOID pvAddress
    );

////#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_APP) */
#pragma endregion

#pragma region Desktop Family
//#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)

    
HRESULT
WINAPI
WerSetFlags(
    _In_ DWORD dwFlags
    );
    
HRESULT
WINAPI
WerGetFlags(
    _In_ HANDLE hProcess,
    _Out_ PDWORD pdwFlags
    );

//
// ++++++++++++++++++++++++++ Application Setup APIs ++++++++++++++++++++++++++++++++++++++
// 
HRESULT
WINAPI
WerAddExcludedApplication(
    _In_ PCWSTR pwzExeName,
    _In_ BOOL bAllUsers
    );
    
HRESULT
WINAPI
WerRemoveExcludedApplication(
    _In_ PCWSTR pwzExeName,
    _In_ BOOL bAllUsers
    );

//
// ++++++++++++++++++++++++++ Run time handler APIs +++++++++++++++++++++++++++
// 

#define WER_MAX_REGISTERED_RUNTIME_EXCEPTION_MODULES 16

HRESULT
WINAPI
WerRegisterRuntimeExceptionModule(
    _In_ PCWSTR pwszOutOfProcessCallbackDll,
    _In_ PVOID pContext
    );

HRESULT
WINAPI
WerUnregisterRuntimeExceptionModule(
    _In_ PCWSTR pwszOutOfProcessCallbackDll,
    _In_ PVOID pContext
    );

#define WER_RUNTIME_EXCEPTION_EVENT_FUNCTION                "OutOfProcessExceptionEventCallback"
#define WER_RUNTIME_EXCEPTION_EVENT_SIGNATURE_FUNCTION      "OutOfProcessExceptionEventSignatureCallback"
#define WER_RUNTIME_EXCEPTION_DEBUGGER_LAUNCH               "OutOfProcessExceptionEventDebuggerLaunchCallback"

typedef struct _WER_RUNTIME_EXCEPTION_INFORMATION
{
    DWORD dwSize;
    HANDLE hProcess;
    HANDLE hThread;
    EXCEPTION_RECORD exceptionRecord;
    CONTEXT context;
    PCWSTR pwszReportId;
} WER_RUNTIME_EXCEPTION_INFORMATION, *PWER_RUNTIME_EXCEPTION_INFORMATION;

typedef
HRESULT
(* PFN_WER_RUNTIME_EXCEPTION_EVENT)(       
    _In_ PVOID pContext,
    _In_ const PWER_RUNTIME_EXCEPTION_INFORMATION pExceptionInformation,
    _Out_ BOOL * pbOwnershipClaimed,
    _Out_writes_(*pchSize) PWSTR pwszEventName,
    _Inout_ PDWORD  pchSize,
    _Out_ PDWORD  pdwSignatureCount
    );

typedef
HRESULT
(* PFN_WER_RUNTIME_EXCEPTION_EVENT_SIGNATURE)(       
    _In_ PVOID pContext,
    _In_ const PWER_RUNTIME_EXCEPTION_INFORMATION pExceptionInformation,
    _In_ DWORD dwIndex,
    _Out_writes_(*pchName) PWSTR pwszName,
    _Inout_ PDWORD  pchName,
    _Out_writes_(*pchValue) PWSTR pwszValue,
    _Inout_ PDWORD  pchValue
    );

typedef
HRESULT
(* PFN_WER_RUNTIME_EXCEPTION_DEBUGGER_LAUNCH)(       
    _In_ PVOID pContext,
    _In_ const PWER_RUNTIME_EXCEPTION_INFORMATION pExceptionInformation,
    _Out_ PBOOL pbIsCustomDebugger,
    _Out_writes_(*pchName) PWSTR pwszDebuggerLaunch,
    _Inout_ PDWORD  pchDebuggerLaunch,
    _Out_ PBOOL pbIsDebuggerAutolaunch
    );

//#endif /* WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP) */
#pragma endregion

#ifdef __cplusplus
}                  
#endif 