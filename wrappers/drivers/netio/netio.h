/*
 * WSK (Winsock Kernel) driver library.
 *
 * Copyright 2020 Paul Gofman <pgofman@codeweavers.com> for CodeWeavers
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
 */

#define WSKAPI NTAPI

typedef unsigned short u_short;

typedef ULONG KSPIN_LOCK;

typedef LONG NTSTATUS;

typedef UINT_PTR SOCKET;

typedef struct _TP_WAIT TP_WAIT, *PTP_WAIT;

typedef USHORT ADDRESS_FAMILY;

typedef DWORD TP_WAIT_RESULT;

typedef short CSHORT;

typedef struct _TP_CALLBACK_INSTANCE TP_CALLBACK_INSTANCE, *PTP_CALLBACK_INSTANCE;

typedef VOID (*PTP_WAIT_CALLBACK)(PTP_CALLBACK_INSTANCE Instance, PVOID Context, PTP_WAIT Wait, TP_WAIT_RESULT WaitResult);

typedef struct _WSK_REGISTRATION
{
    ULONGLONG ReservedRegistrationState;
    PVOID ReservedRegistrationContext;
    KSPIN_LOCK ReservedRegistrationLock;
} WSK_REGISTRATION, *PWSK_REGISTRATION;

// typedef struct _OVERLAPPED {
     // ULONG_PTR Internal;
     // ULONG_PTR InternalHigh;
     // union {
         // struct {
             // DWORD Offset;
             // DWORD OffsetHigh;
         // } DUMMYSTRUCTNAME;
         // PVOID Pointer;
     // } DUMMYUNIONNAME;
     // HANDLE hEvent;
 // } OVERLAPPED, *POVERLAPPED, *LPOVERLAPPED;

typedef struct _WSK_SOCKET
{
    const VOID *Dispatch;
} WSK_SOCKET, *PWSK_SOCKET;

typedef
NTSTATUS
(WINAPI * PFN_WSK_CLIENT_EVENT)(
    _In_opt_ PVOID ClientContext,
    _In_ ULONG EventType,
    _In_reads_bytes_opt_(InformationLength) PVOID Information,
    _In_ SIZE_T InformationLength);

typedef struct _WSK_CLIENT_DISPATCH
{
    USHORT Version;
    USHORT Reserved;
    PFN_WSK_CLIENT_EVENT WskClientEvent;
} WSK_CLIENT_DISPATCH, *PWSK_CLIENT_DISPATCH;

typedef struct _WSK_CLIENT_NPI
{
    PVOID ClientContext;
    const WSK_CLIENT_DISPATCH *Dispatch;
} WSK_CLIENT_NPI, *PWSK_CLIENT_NPI;

typedef struct sockaddr {
#if (_WIN32_WINNT < 0x0600)
    u_short sa_family;
#else 
    ADDRESS_FAMILY sa_family;
#endif //(_WIN32_WINNT < 0x0600)
    CHAR sa_data[14];
} SOCKADDR, *PSOCKADDR, FAR *LPSOCKADDR;


typedef BOOL
(PASCAL FAR *LPFN_ACCEPTEX)(
  _In_ SOCKET sListenSocket,
  _In_ SOCKET sAcceptSocket,
  _Out_writes_bytes_(dwReceiveDataLength + dwLocalAddressLength + dwRemoteAddressLength) PVOID lpOutputBuffer,
  _In_ DWORD dwReceiveDataLength,
  _In_ DWORD dwLocalAddressLength,
  _In_ DWORD dwRemoteAddressLength,
  _Out_ LPDWORD lpdwBytesReceived,
  _Inout_ LPOVERLAPPED lpOverlapped);
  
  typedef BOOL
(PASCAL FAR *LPFN_CONNECTEX)(
  _In_ SOCKET s,
  _In_reads_bytes_(namelen) const struct sockaddr FAR *name,
  _In_ int namelen,
  _In_reads_bytes_opt_(dwSendDataLength) PVOID lpSendBuffer,
  _In_ DWORD dwSendDataLength,
  _Out_ LPDWORD lpdwBytesSent,
  _Inout_ LPOVERLAPPED lpOverlapped);
  
typedef VOID
(PASCAL FAR *LPFN_GETACCEPTEXSOCKADDRS)(
  _In_reads_bytes_(dwReceiveDataLength + dwLocalAddressLength + dwRemoteAddressLength) PVOID lpOutputBuffer,
  _In_ DWORD dwReceiveDataLength,
  _In_ DWORD dwLocalAddressLength,
  _In_ DWORD dwRemoteAddressLength,
  _Outptr_result_bytebuffer_(*LocalSockaddrLength) struct sockaddr **LocalSockaddr,
  _Out_ LPINT LocalSockaddrLength,
  _Outptr_result_bytebuffer_(*RemoteSockaddrLength) struct sockaddr **RemoteSockaddr,
  _Out_ LPINT RemoteSockaddrLength);  
  
typedef struct _IO_STATUS_BLOCK {
  union {
    NTSTATUS Status;
    PVOID    Pointer;
  };
  ULONG_PTR Information;
} IO_STATUS_BLOCK, *PIO_STATUS_BLOCK;  
  
typedef struct _IRP {
  CSHORT                    Type;
  USHORT                    Size;
  // PMDL                      MdlAddress;
  // ULONG                     Flags;
  // union {
    // struct _IRP     *MasterIrp;
    // __volatile LONG IrpCount;
    // PVOID           SystemBuffer;
  // } AssociatedIrp;
  // LIST_ENTRY                ThreadListEntry;
  IO_STATUS_BLOCK           IoStatus;
  // KPROCESSOR_MODE           RequestorMode;
  // BOOLEAN                   PendingReturned;
  // CHAR                      StackCount;
  CHAR                      CurrentLocation;
  // BOOLEAN                   Cancel;
  // KIRQL                     CancelIrql;
  // CCHAR                     ApcEnvironment;
  // UCHAR                     AllocationFlags;
  // union {
    // PIO_STATUS_BLOCK UserIosb;
    // PVOID            IoRingContext;
  // };
  // PKEVENT                   UserEvent;
  // union {
    // struct {
      // union {
        // PIO_APC_ROUTINE UserApcRoutine;
        // PVOID           IssuingProcess;
      // };
      // union {
        // PVOID                 UserApcContext;
// #if ...
        // _IORING_OBJECT        *IoRing;
// #else
        // struct _IORING_OBJECT *IoRing;
// #endif
      // };
    // } AsynchronousParameters;
    // LARGE_INTEGER AllocationSize;
  // } Overlay;
  // PDRIVER_CANCEL CancelRoutine;
  // PVOID                     UserBuffer;
  // union {
    // struct {
      // union {
        // KDEVICE_QUEUE_ENTRY DeviceQueueEntry;
        // struct {
          // PVOID DriverContext[4];
        // };
      // };
      // PETHREAD     Thread;
      // PCHAR        AuxiliaryBuffer;
      // struct {
        // LIST_ENTRY ListEntry;
        // union {
          // struct _IO_STACK_LOCATION *CurrentStackLocation;
          // ULONG                     PacketType;
        // };
      // };
      // PFILE_OBJECT OriginalFileObject;
    // } Overlay;
    // KAPC  Apc;
    // PVOID CompletionKey;
  // } Tail;
} IRP;  
  