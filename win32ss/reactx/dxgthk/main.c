
/*
 * COPYRIGHT:        See COPYING in the top level directory
 * PROJECT:          ReactOS kernel
 * PURPOSE:          Native driver for dxg implementation
 * FILE:             drivers/directx/dxg/main.c
 * PROGRAMER:        Magnus olsen (magnus@greatlord.com)
 * REVISION HISTORY:
 *       15/10-2007   Magnus Olsen
 */

/* INCLUDES *****************************************************************/

#include <stdio.h>
#include <ntddk.h>
#include <wmilib.h>

#define NDEBUG
#include <debug.h>
#include <ntimage.h>
#include <inbvfuncs.h>
#include <ldrfuncs.h>
#include <ntifs.h>
#include <exfuncs.h>
#include <lpctypes.h>


/* GLOBALS ******************************************************************/

typedef int BOOL;
typedef unsigned long DWORD;

PVOID gptiCurrent;
PVOID gDwmApiPort;
PERESOURCE gpresUser;
PEPROCESS gpepDwm;
//POBJECT_TYPE LpcPortObjectType;
BOOL gfCompositing;
HANDLE ghDwmApiPort;

typedef struct _RECT {
  LONG left;
  LONG top;
  LONG right;
  LONG bottom;
} RECT, *PRECT;

typedef struct _RGNDATAHEADER {
  DWORD dwSize;
  DWORD iType;
  DWORD nCount;
  DWORD nRgnSize;
  RECT  rcBound;
} RGNDATAHEADER, *PRGNDATAHEADER;

typedef struct _RGNDATA {
  RGNDATAHEADER rdh;
  char          Buffer[1];
} RGNDATA, *PRGNDATA;

/* FUNCTIONS ****************************************************************/
/*
PVOID NTAPI EnterCrit()
{
  PVOID result; // eax@1

  result = ExEnterCriticalRegionAndAcquireResourceExclusive(gpresUser);
  gptiCurrent = result;
  return result;
}

void NTAPI LeaveCrit()
{
  ExReleaseResourceAndLeaveCriticalRegion(gpresUser);
}

NTSTATUS NTAPI xxxDwmStartup()
{
  NTSTATUS status; // edi@1

  status = 0;
  if ( gfCompositing )
  {
    status = 0xC0000001u;
  }
  else
  {
    DwmHwndValidationBeginStartup();
    if ( dword_BF9CDB08 )
      StopFade();
    DwmNotifyChildrenAddRemove(1);
    bSetDevDragRect(*(_DWORD *)gpDispInfo, 0, 0);
    if ( GreDwmStartup(*(_DWORD *)gpDispInfo) )
    {
      gfCompositing = 1;
      *(_DWORD *)(gpsi + 1836) = 1;
      sub_BF87F242();
      xxxComposeDesktop(grpdeskRitInput, 1);
    }
    else
    {
      DwmNotifyChildrenAddRemove(0);
      status = 0xC0000001u;
    }
  }
  return status;
}

BOOL NTAPI NtUserDwmStartup(HANDLE Object)
{
  ULONG error; // eax@14
  BOOL resp; // esi@1
  NTSTATUS status; // ebx@2

  EnterCrit();
  resp = 0;
  if ( gpepDwm )
  {
    //UserSetLastError(5);
    goto LABEL_6;
  }
  status = ObReferenceObjectByHandle(Object, 0, (POBJECT_TYPE)LpcPortObjectType, 1, &Object, 0);
  gDwmApiPort = Object;
  if ( status >= 0 )
  {
    status = ObOpenObjectByPointer(Object, 512, 0, 2031619, 0, 0, &ghDwmApiPort);
    if ( status < 0 )
    {
      ObfDereferenceObject(gDwmApiPort);
    }
    else
    {
      gpepDwm = PsGetCurrentProcess();
      status = xxxDwmStartup();
      if ( status >= 0 )
      {
        resp = 1;
        goto LABEL_6;
      }
      if ( gDwmApiPort )
        ObfDereferenceObject(gDwmApiPort);
      gpepDwm = 0;
    }
    gDwmApiPort = 0;
  }
  if ( RtlNtStatusToDosError(status) )
  {
    error = RtlNtStatusToDosError(status);
    //UserSetLastError(error);
  }
LABEL_6:
  LeaveCrit();
  return resp;
}
*/

INT
NTAPI
EngCombineRgn(
    _In_ HANDLE hrgnTrg,
    _In_ HANDLE hrgnSrc1,
    _In_ HANDLE hrgnSrc2,
    _In_ INT iMode)
{
	return 0;
}

HANDLE
EngCreateRectRgn(
    _In_ INT left,
    _In_ INT top,
    _In_ INT right,
    _In_ INT bottom)
{
	return NULL;
}

VOID
EngDeleteRgn(
    _In_ HANDLE hrgn)
{
	;
}

DWORD
EngGetRgnData(
    _In_ HANDLE hrgn,
    _In_ DWORD nCount,
    _Out_cap_(nCount) PRGNDATA lpRgnData)
{
	return 0;
}

INT
EngOffsetRgn(
    _In_ HANDLE hrgn,
    _In_ INT x,
    _In_ INT y)
{
	return 0;
}

BOOL
EngSetRectRgn(
    _In_ HANDLE hrgn,
    _In_ INT left,
    _In_ INT top,
    _In_ INT right,
    _In_ INT bottom)
{
	return TRUE;
}

NTSTATUS 
NTAPI
EngQueryW32kCddInterface(HANDLE a1, int a2, int a3, int a4, int a5)
{
	return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
DriverEntry(IN PDRIVER_OBJECT DriverObject,
            IN PUNICODE_STRING RegistryPath)
{
    /* 
     * NOTE this driver will never be load, it only contain export list
     * to win32k eng functions
     */
    return STATUS_SUCCESS;
}

