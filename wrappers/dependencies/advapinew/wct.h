/*++

Copyright (c) 2004 Microsoft Corporation

Module Name:

    wct.h

Abstract:

    Private interfaces for thread wait chain traversal.

History:

    12/01/2004   sdoll     Created.

--*/


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum _WCT_OBJECT_TYPE
{
    WctCriticalSectionType = 1,
    WctSendMessageType,
    WctMutexType,
    WctAlpcType,
    WctComType,
    WctThreadWaitType,
    WctProcessWaitType,
    WctThreadType,
    WctComActivationType,
    WctUnknownType,
    WctMaxType
} WCT_OBJECT_TYPE;


typedef enum _WCT_OBJECT_STATUS
{
    WctStatusNoAccess = 1,            // ACCESS_DENIED for this object
    WctStatusRunning,                 // Thread status
    WctStatusBlocked,                 // Thread status
    WctStatusPidOnly,                 // Thread status
    WctStatusPidOnlyRpcss,            // Thread status
    WctStatusOwned,                   // Dispatcher object status
    WctStatusNotOwned,                // Dispatcher object status
    WctStatusAbandoned,               // Dispatcher object status
    WctStatusUnknown,                 // All objects
    WctStatusError,                   // All objects
    WctStatusMax
} WCT_OBJECT_STATUS;

// Max. number of nodes in the wait chain
#define WCT_MAX_NODE_COUNT 16

#define WCT_OBJNAME_LENGTH 128

typedef struct _WAITCHAIN_NODE_INFO
{
    WCT_OBJECT_TYPE ObjectType;
    WCT_OBJECT_STATUS ObjectStatus;

    union {
        struct {
            WCHAR ObjectName[WCT_OBJNAME_LENGTH];
            LARGE_INTEGER Timeout;    // Not implemented in v1
            BOOL Alertable;           // Not implemented in v1
        } LockObject;

        struct {
            DWORD ProcessId;
            DWORD ThreadId;
            DWORD WaitTime;
            DWORD ContextSwitches;
        } ThreadObject;
    };

} WAITCHAIN_NODE_INFO, *PWAITCHAIN_NODE_INFO;


typedef LPVOID HWCT;

typedef VOID (CALLBACK *PWAITCHAINCALLBACK) (
    HWCT WctHandle,
    DWORD_PTR Context,
    DWORD CallbackStatus,
    LPDWORD NodeCount,
    PWAITCHAIN_NODE_INFO NodeInfoArray,
    LPBOOL IsCycle
    );

#define WCT_ASYNC_OPEN_FLAG 0x1
#define WCTP_OPEN_ALL_FLAGS (WCT_ASYNC_OPEN_FLAG)

HWCT WINAPI
OpenThreadWaitChainSession (
    __in DWORD Flags,
    __in_opt PWAITCHAINCALLBACK callback
    );

VOID WINAPI CloseThreadWaitChainSession(
  _In_  HWCT WctHandle
);

#define WCT_OUT_OF_PROC_FLAG     0x1
#define WCT_OUT_OF_PROC_COM_FLAG 0x2
#define WCT_OUT_OF_PROC_CS_FLAG  0x4
#define WCTP_GETINFO_ALL_FLAGS (WCT_OUT_OF_PROC_FLAG|WCT_OUT_OF_PROC_COM_FLAG|WCT_OUT_OF_PROC_CS_FLAG)

BOOL WINAPI GetThreadWaitChain(
  _In_      HWCT WctHandle,
  _In_opt_  DWORD_PTR Context,
  _In_      DWORD Flags,
  _In_      DWORD ThreadId,
  _Inout_   LPDWORD NodeCount,
  _Out_     PWAITCHAIN_NODE_INFO NodeInfoArray,
  _Out_     LPBOOL IsCycle
);

typedef HRESULT (*PCOGETCALLSTATE)(int, PULONG);
typedef HRESULT (*PCOGETACTIVATIONSTATE)(GUID, DWORD, DWORD*);

VOID WINAPI RegisterWaitChainCOMCallback(
  _In_  PCOGETCALLSTATE CallStateCallback,
  _In_  PCOGETACTIVATIONSTATE ActivationStateCallback
);

#ifdef __cplusplus
}
#endif

