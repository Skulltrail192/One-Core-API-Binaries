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

#include <wine/config.h>

#include <wine/debug.h>
//#include <ntstatus.h>
#define WIN32_NO_STATUS
#include <windef.h>
#include <winbase.h>
#include <winternl.h>
#include <objbase.h>
#include <rpcproxy.h>
#include <security.h>

HANDLE hRpcHeap;
DWORD LsaFree = HEAP_ZERO_MEMORY;
DWORD gLocalComputerNameLength = 0;
PCNZWCH gLocalComputerName = NULL;

int (__stdcall *pfnCoTaskMemFree)(DWORD);
int (__stdcall *pfnCoTaskMemAlloc)(DWORD);

typedef struct _RPC_BINDING_HANDLE_TEMPLATE {
  unsigned long  Version;
  unsigned long  Flags;
  unsigned long  ProtocolSequence;
  unsigned short *NetworkAddress;
  unsigned short *StringEndpoint;
  union {
    unsigned short *Reserved;
  } u1;
  UUID           ObjectUuid;
} RPC_BINDING_HANDLE_TEMPLATE_V1, RPC_BINDING_HANDLE_TEMPLATE;

typedef struct {
  unsigned long Version;
  unsigned long Flags;
  unsigned long ComTimeout;
  unsigned long CallTimeout;
} RPC_BINDING_HANDLE_OPTIONS_V1, RPC_BINDING_HANDLE_OPTIONS;

typedef struct {
  unsigned long           Version;
  unsigned short          *ServerPrincName;
  unsigned long           AuthnLevel;
  unsigned long           AuthnSvc;
  SEC_WINNT_AUTH_IDENTITY_W *AuthIdentity;
  RPC_SECURITY_QOS        *SecurityQos;
} RPC_BINDING_HANDLE_SECURITY_V1, RPC_BINDING_HANDLE_SECURITY;

typedef enum  { 
  RpcNotificationCallNone          = 0,
  RpcNotificationClientDisconnect  = 1,
  RpcNotificationCallCancel        = 2
} RPC_NOTIFICATIONS;

WINE_DEFAULT_DEBUG_CHANNEL(bcrypt);

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
    TRACE("fdwReason %u\n", fdwReason);

    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            DisableThreadLibraryCalls(hInstDLL);
            break;
    }

    return TRUE;
}

/***********************************************************************
 *              DllRegisterServer (RPCRT4.@)
 */

HRESULT WINAPI DllRegisterServer( void )
{
    FIXME( "(): stub\n" );
    return S_OK;
}

RPC_STATUS WINAPI I_RpcBindingCreateNP(RPC_WSTR ServerName, RPC_WSTR ServiceName, RPC_WSTR NetworkOptions, RPC_BINDING_HANDLE *Binding)
{
  const wchar_t *localServerName; // esi@1
  RPC_WSTR one; // ebx@1
  RPC_WSTR two; // eax@2
  RPC_WSTR v7; // ecx@2
  unsigned __int16 other; // dx@3
  RPC_STATUS resp; // esi@9
  RPC_WSTR v12; // eax@20
  unsigned __int16 v13; // cx@21
  int v14; // edi@22
  LPVOID mem = NULL; // [sp+0h] [bp-234h]@0
  DWORD nSize; // [sp+14h] [bp-220h]@1
  void *string; // [sp+18h] [bp-21Ch]@1
  RPC_WSTR NetworkAddr; // [sp+1Ch] [bp-218h]@1
  RPC_WSTR Str1; // [sp+20h] [bp-214h]@1
  WCHAR Buffer; // [sp+24h] [bp-210h]@36

  localServerName = ServerName;
  one = 0;
  Str1 = ServiceName;
  nSize = 261;
  NetworkAddr = 0;
  string = 0;
  two = Str1;
  *Binding = 0;
  v7 = two + 1;
  do
  {
    other = *two;
    ++two;
  }
  while ( other );
  if ( two - v7 < 6 || _wcsnicmp(Str1, L"\\PIPE\\", 6u) )
  {
    Str1 = L"\\PIPE\\";
  }
  if ( !ServerName )
    goto LABEL_9;
  v12 = ServerName;
  do
  {
    v13 = *v12;
    ++v12;
  }
  while ( v13 );
  v14 = v12 - (ServerName + 1);
  if ( !*ServerName )
  {
    NetworkAddr = 0;
LABEL_9:
    resp = RpcStringBindingComposeW(0, L"ncacn_np", NetworkAddr, Str1, NetworkOptions, &Str1);
    if ( !resp )
    {
      resp = RpcBindingFromStringBindingW(Str1, Binding);
      RpcStringFreeW(&Str1);
    }
    goto LABEL_11;
  }
  if ( *ServerName == 92 && ServerName[1] == 92 )
  {
    NetworkAddr = ServerName;
    localServerName = ServerName + 2;
    v14 -= 2;
    if ( !v14 )
    {
      localServerName = 0;
      NetworkAddr = 0;
    }
    goto LABEL_30;
  }
  one = (RPC_WSTR)HeapAlloc(hRpcHeap, HEAP_ZERO_MEMORY, (SIZE_T)mem);
  if ( one )
  {
    *one = 92;
    one[1] = 92;
    memcpy(one + 2, ServerName, 2 * v14 + 2);
    NetworkAddr = one;
LABEL_30:
    if ( localServerName )
    {
      if ( localServerName[v14 - 1] == 46 )
        --v14;
      if ( v14 == gLocalComputerNameLength - 1 && !_wcsnicmp(gLocalComputerName, localServerName, v14)
        || (unsigned int)v14 <= 0x104
        && GetComputerNameExW(ComputerNameDnsFullyQualified, &Buffer, &nSize)
        && v14 == nSize
        && !_wcsnicmp(&Buffer, localServerName, v14) )
        NetworkAddr = 0;
    }
    goto LABEL_9;
  }
LABEL_11:
  if ( string )
    //FreeWrapper(mem);
	HeapFree(hRpcHeap, LsaFree, mem);
  if ( one )
    HeapFree(hRpcHeap, LsaFree, mem);
  return resp;
}

RPC_STATUS WINAPI I_RpcServerStartService(RPC_WSTR Protseq, RPC_WSTR Endpoint, RPC_IF_HANDLE IfSpec)
{
    return RPC_S_OK;
}

int WINAPI I_RpcVerifierCorruptionExpected()
{
	return 0;
}

BOOL WINAPI NdrGetBaseInterfaceFromStub(int a1, int a2, int a3)
{
  return TRUE;
}

RPC_STATUS WINAPI RpcBindingBind(
  _In_opt_  PRPC_ASYNC_STATE pAsync,
  _In_      RPC_BINDING_HANDLE Binding,
  _In_      RPC_IF_HANDLE IfSpec
)
{
    return RPC_S_OK;
}

RPC_STATUS WINAPI RpcBindingCreateA(
  _In_      RPC_BINDING_HANDLE_TEMPLATE *Template,
  _In_opt_  RPC_BINDING_HANDLE_SECURITY *Security,
  _In_opt_  RPC_BINDING_HANDLE_OPTIONS *Options,
  _Out_     RPC_BINDING_HANDLE *Binding
)
{
    return RPC_S_OK;
}

RPC_STATUS WINAPI RpcBindingCreateW(
  _In_      RPC_BINDING_HANDLE_TEMPLATE *Template,
  _In_opt_  RPC_BINDING_HANDLE_SECURITY *Security,
  _In_opt_  RPC_BINDING_HANDLE_OPTIONS *Options,
  _Out_     RPC_BINDING_HANDLE *Binding
)
{
    return RPC_S_OK;
}

RPC_STATUS WINAPI RpcBindingUnbind(
  _In_  RPC_BINDING_HANDLE Binding
)
{
    return RPC_S_OK;
}

int RPC_ENTRY RpcExceptionFilter(
  _In_  unsigned long ExceptionCode
)
{
	return EXCEPTION_EXECUTE_HANDLER;
}

RPC_STATUS WINAPI RpcServerCompleteSecurityCallback(RPC_BINDING_HANDLE BindingHandle, RPC_STATUS Status)
{
    return RPC_S_OK;
}

RPC_STATUS WINAPI WINAPI RpcServerInqBindingHandle(
  RPC_BINDING_HANDLE *Binding
)
{
    return RPC_S_OK;
}

RPC_STATUS WINAPI RpcServerSubscribeForNotification(
  _In_  RPC_BINDING_HANDLE Binding,
  _In_  DWORD Notification,
  _In_  RPC_NOTIFICATION_TYPES NotificationType,
  _In_  RPC_ASYNC_NOTIFICATION_INFO *NotificationInfo
)
{
    return RPC_S_OK;
}

RPC_STATUS WINAPI RpcServerUnsubscribeForNotification(
  _In_   RPC_BINDING_HANDLE Binding,
  _In_   RPC_NOTIFICATIONS Notification,
  _Out_  unsigned long *NotificationsQueued
)
{
    return RPC_S_OK;
}

RPC_STATUS WINAPI I_RpcGetPortAllocationData(  _In_   RPC_BINDING_HANDLE Binding)
{
    return RPC_S_OK;
}

RPC_STATUS WINAPI I_RpcInitHttpImports(  _In_   RPC_BINDING_HANDLE Binding)
{
    return RPC_S_OK;
}

RPC_STATUS WINAPI I_RpcBindingInqSecurityContextKeyInfo(
    __in_opt RPC_BINDING_HANDLE Binding,
    __inout void *KeyInfo
)
{
    return RPC_S_OK;
}

RPC_STATUS
WINAPI
I_RpcServerInqRemoteConnAddress (
    IN RPC_BINDING_HANDLE Binding,
    IN OUT void *Buffer,
    IN OUT unsigned long *BufferSize,
    OUT unsigned long *AddressFormat
)
{
    return RPC_S_OK;
}

// Test only section
ULONG WINAPI CStdStubBuffer_AddRef(IRpcStubBuffer *This)
{
  InterlockedIncrement((volatile LONG *)&This[1]);
  return (ULONG)This[1].lpVtbl;
}

HRESULT WINAPI CStdStubBuffer_Connect(IRpcStubBuffer *This, IUnknown *pUnkServer)
{
  HRESULT hResult; // edi@3
  LONG exchange; // eax@3
  LONG Value; // [sp+4h] [bp-4h]@1

  Value = 0;
  hResult = pUnkServer->lpVtbl->QueryInterface(
              pUnkServer,
              (const IID *const )*((DWORD *)This->lpVtbl - 4),
              (void **)&Value);
  exchange = InterlockedExchange((volatile LONG *)&This[2], Value);
  Value = exchange;
  if ( exchange )
    (*(void (__stdcall **)(DWORD))(*(DWORD *)exchange + 8))(exchange);
  return hResult;
}

HRESULT WINAPI CStdStubBuffer_DebugServerQueryInterface(IRpcStubBuffer *This, void **ppv)
{
  void *resp; // eax@1

  resp = This[2].lpVtbl;
  *ppv = resp;
  return resp != 0 ? 0 : 0x800401FDu;
}

void WINAPI CStdStubBuffer_DebugServerRelease(IRpcStubBuffer *This, void *pv)
{
  ;
}

ULONG WINAPI CStdStubBuffer_CountRefs(IRpcStubBuffer *This)
{
  ULONG result; // eax@1

  result = 0;
  if ( This[2].lpVtbl )
    result = 1;
  return result;
}

IRpcStubBuffer * WINAPI CStdStubBuffer_IsIIDSupported(IRpcStubBuffer *This, const IID *const riid)
{
  IRpcStubBuffer *buffer; // [sp+Ch] [bp-4h]@1

  buffer = 0;
  if ( !memcmp(riid, *((const void **)This->lpVtbl - 4), 16) && This[2].lpVtbl )
  {
    buffer = This;
    This->lpVtbl->AddRef(This);
  }
  return buffer;
}

void WINAPI NdrOleFree(void *NodeToFree)
{
  pfnCoTaskMemFree((DWORD)NodeToFree);
}

void *WINAPI NdrOleAllocate(size_t Size)
{
  void *result; // eax@1

  result = (void *)pfnCoTaskMemAlloc(Size);
  if ( !result )
    RpcRaiseException(0x8007000Eu);
  return result;
}
/*
ULONG WINAPI IUnknown_AddRef_Proxy(IUnknown *This)
{
  return (*((int (__stdcall **)(DWORD))This[2].lpVtbl->QueryInterface + 1))(This[2].lpVtbl);
}*/