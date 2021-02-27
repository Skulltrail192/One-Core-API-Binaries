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

typedef DWORD CONFIGRET;

static CRITICAL_SECTION user_section;
static CRITICAL_SECTION_DEBUG critsect_debug =
{
    0, 0, &user_section,
    { &critsect_debug.ProcessLocksList, &critsect_debug.ProcessLocksList },
      0, 0, { (DWORD_PTR)(__FILE__ ": user_section") }
};
static CRITICAL_SECTION user_section = { &critsect_debug, -1, 0, 0, 0, 0 };

 
BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            break;
    }

    return TRUE;
}

HDEVNOTIFY 
WINAPI 
RegisterDeviceNotificationWorker(HANDLE hRecipient, LPVOID NotificationFilter, DWORD Flags)
{
  CONFIGRET receive; // edi@1
  HDEVNOTIFY resp; // ebx@1
  HMODULE module; // eax@1
  HMODULE otherModule; // esi@1
  FARPROC proc; // eax@2
  DWORD error; // [sp-4h] [bp-14h]@12
  PULONG compose; // [sp+Ch] [bp-4h]@1

  compose = NULL;
  receive = 19;
  resp = 0;
  module = LoadLibraryW(L"SETUPAPI.DLL");
  otherModule = module;
  if ( !module )
    goto LABEL_19;
  proc = GetProcAddress(module, "CMP_RegisterNotification");
  if ( proc )
    receive = ((CONFIGRET (__stdcall *)(HANDLE, LPVOID, DWORD, PULONG))proc)(hRecipient, NotificationFilter, Flags, compose);
  FreeLibrary(otherModule);
  if ( receive )
  {
LABEL_19:
    switch ( receive )
    {
      case 2:
        error = 8;
        break;
      case 3:
        error = 87;
        break;
      case 31:
        error = 13;
        break;
      default:
        error = 1066;
        break;
    }
    SetLastError(error);
  }
  if ( compose && compose != (PULONG)-1 )
    resp = (HDEVNOTIFY)compose;
  return resp;
}

HPOWERNOTIFY 
WINAPI 
RegisterPowerSettingNotification(
  _In_  HANDLE hRecipient,
  _In_  LPCGUID PowerSettingGuid,
  _In_  DWORD Flags
)
{
	return (HPOWERNOTIFY)0xdeadbeef;	
  // LPVOID v4; // [sp+8h] [bp-20h]@1
  // int v5; // [sp+Ch] [bp-1Ch]@1
  // int v6; // [sp+10h] [bp-18h]@1
  // DWORD v7; // [sp+14h] [bp-14h]@1
  // int v8; // [sp+18h] [bp-10h]@1
  // int v9; // [sp+1Ch] [bp-Ch]@1
  // int v10; // [sp+20h] [bp-8h]@1

  // v6 = 0;
  // v4 = (LPVOID)28;
  // v5 = -2147483647;
  // v7 = PowerSettingGuid->Data1;
  // v8 = *(DWORD *)&PowerSettingGuid->Data2;
  // v9 = *(DWORD *)&PowerSettingGuid->Data4[0];
  // v10 = *(DWORD *)&PowerSettingGuid->Data4[4];
  // return RegisterDeviceNotificationWorker(hRecipient, v4, Flags);
}

BOOL WINAPI UnregisterDeviceNotification(HDEVNOTIFY Handle)
{
  int verification; // edi@1
  HMODULE module; // eax@1
  HMODULE receiveModule; // esi@1
  FARPROC proc; // eax@2
  DWORD error; // [sp-4h] [bp-Ch]@8

  verification = 0;
  module = LoadLibraryW(L"SETUPAPI.DLL");
  receiveModule = module;
  if ( module )
  {
    proc = GetProcAddress(module, "CMP_UnregisterNotification");
    if ( proc )
      verification = ((int (__stdcall *)(HDEVNOTIFY))proc)(Handle);
    FreeLibrary(receiveModule);
    if ( verification )
    {
      if ( verification == 3 )
      {
        error = 87;
      }
      else
      {
        if ( verification == 31 )
          error = 13;
        else
          error = 1066;
      }
      SetLastError(error);
    }
  }
  return verification == 0;
}

BOOL WINAPI UnregisterPowerSettingNotification(HDEVNOTIFY Handle)
{
  //return UnregisterDeviceNotification(Handle);
   return TRUE;
}

HDEVNOTIFY WINAPI RegisterDeviceNotificationW(HANDLE hRecipient, LPVOID NotificationFilter, DWORD Flags)
{
  HDEVNOTIFY result; // eax@2

  if ( *((DWORD *)NotificationFilter + 1) == -2147483647 )
  {
    SetLastError(0xDu);
    result = NULL;
  }
  else
  {
    result = (HDEVNOTIFY)RegisterDeviceNotificationWorker(hRecipient, NotificationFilter, Flags);
  }
  return result;
}

BOOL 
WINAPI 
IsThreadDesktopComposited ( ) 
{
	DbgPrint("IsThreadDesktopComposited is UNIMPLEMENTED\n");  
	return FALSE;
}

HWND WINAPI GetSendMessageReceiver(DWORD threadId)
{
	DbgPrint("GetSendMessageReceiver is UNIMPLEMENTED\n");  
  //return (HWND)NtUserCallOneParam(threadId, 35); need implement own NtUserCallOneParam
  return NULL;
}

BOOL WINAPI SwitchDesktopWithFade(
  _In_  HDESK hDesktop,
  _In_	BOOL active
)
{
	return SwitchDesktop(hDesktop);
}

BOOL WINAPI ShowSystemCursor(BOOL fShow)
{
	ShowCursor(fShow);
	return TRUE;
}

BOOL WINAPI SetPhysicalCursorPos(int X, int Y)
{
	return SetCursorPos(X, Y);
}

BOOL WINAPI GetPhysicalCursorPos(
  _Out_  LPPOINT lpPoint
)
{
	return GetCursorPos(lpPoint);
}

BOOL WINAPI RemoveClipboardFormatListener(
  _In_  HWND hwnd
)
{
	DbgPrint("RemoveClipboardFormatListener is UNIMPLEMENTED\n");  
	hwnd = NULL;
	return TRUE;
}

BOOL WINAPI PhysicalToLogicalPoint(
  _In_     HWND hWnd,
  _Inout_  LPPOINT lpPoint
)
{
	DbgPrint("PhysicalToLogicalPoint is UNIMPLEMENTED\n");  
	lpPoint->x = 0;
	lpPoint->y = 0;
	return TRUE;
}

BOOL WINAPI LogicalToPhysicalPoint(
  _In_     HWND hWnd,
  _Inout_  LPPOINT lpPoint
)
{
	DbgPrint("LogicalToPhysicalPoint is UNIMPLEMENTED\n");  
	lpPoint->x = 0;
	lpPoint->y = 0;
	return TRUE;
}

HDESK WINAPI CreateDesktopExA(
  _In_        LPCTSTR lpszDesktop,
  _Reserved_  LPCTSTR lpszDevice,
  _Reserved_  DEVMODEA *pDevmode,
  _In_        DWORD dwFlags,
  _In_        ACCESS_MASK dwDesiredAccess,
  _In_opt_    LPSECURITY_ATTRIBUTES lpsa,
  _In_        ULONG ulHeapSize,
  _Reserved_  PVOID pvoid
)
{
	return CreateDesktopA(lpszDesktop, 
						  lpszDevice, 
						  pDevmode, 
						  dwFlags, 
						  dwDesiredAccess,
						  lpsa);
}

HDESK WINAPI CreateDesktopExW(
  _In_        LPCWSTR lpszDesktop,
  _Reserved_  LPCWSTR lpszDevice,
  _Reserved_  DEVMODEW *pDevmode,
  _In_        DWORD dwFlags,
  _In_        ACCESS_MASK dwDesiredAccess,
  _In_opt_    LPSECURITY_ATTRIBUTES lpsa,
  _In_        ULONG ulHeapSize,
  _Reserved_  PVOID pvoid
)
{
	return CreateDesktopW(lpszDesktop, 
						  lpszDevice, 
						  pDevmode, 
						  dwFlags, 
						  dwDesiredAccess,
						  lpsa);
}

HDESK WINAPI OpenThreadDesktop(DWORD desktop, BOOL verification, DWORD mask, ACCESS_MASK access)
{
  HDESK result; // eax@2
  PCSR_API_MESSAGE message = NULL; // [sp+4h] [bp-50h]@1
  NTSTATUS NtStatus = STATUS_SUCCESS; // [sp+24h] [bp-30h]@1

  CsrClientCallServer(message, 0, 197637, 8);
  if ( NtStatus < 0 )
  {
    SetLastError(NtStatus);
    result = 0;
  }
  else
  {
    result = GetThreadDesktop(desktop);
  }
  return result;
}

int WINAPI VTagOutput()
{
  return 0;
}

int WINAPI VRipOutput()
{
  return 0;
}
/*
BOOL WINAPI SoundSentry()
{
	return _UserSoundSentry();
}*/

BOOL WINAPI GetUpdatedClipboardFormats(
  _Out_  PUINT lpuiFormats,
  _In_   UINT cFormats,
  _Out_  PUINT pcFormatsOut
)
{
	lpuiFormats = 0;
	pcFormatsOut = 0;
	DbgPrint("GetUpdatedClipboardFormats is UNIMPLEMENTED\n");  
	return TRUE;
}

BOOL WINAPI CheckDesktopByThreadId(HANDLE desktop)
{
	DbgPrint("CheckDesktopByThreadId is UNIMPLEMENTED\n");  
	return TRUE;
}

BOOL WINAPI UnregisterSessionPort()
{
	DbgPrint("UnregisterSessionPort is UNIMPLEMENTED\n");  
	return TRUE;
}

BOOL WINAPI SetMirrorRendering(HANDLE h1, HANDLE h2)
{
	DbgPrint("SetMirrorRendering is UNIMPLEMENTED\n");  
	return TRUE;
}

BOOL WINAPI GetInputLocaleInfo(LCID locale, LPCWSTR localName)
{
	localName = L"en-US";
	DbgPrint("GetInputLocaleInfo is UNIMPLEMENTED\n"); 
	return TRUE;
}

BOOL WINAPI AddClipboardFormatListener(
  _In_  HWND hwnd
)
{
	DbgPrint("AddClipboardFormatListener is UNIMPLEMENTED\n"); 
	return TRUE;
}

DWORD WINAPI ConsoleControl(int a1, PVOID a2, int a3)
{
	DbgPrint("ConsoleControl is UNIMPLEMENTED\n"); 
	return 0;
}

BOOL WINAPI DoSoundConnect()
{
	DbgPrint("DoSoundConnect is UNIMPLEMENTED\n"); 
	return TRUE;
}

BOOL WINAPI DoSoundDisconnect()
{
	DbgPrint("DoSoundDisconnect is UNIMPLEMENTED\n"); 
	return TRUE;
}

DWORD WINAPI RegisterErrorReportingDialog(HWND a1, DWORD a2)
{
	DbgPrint("DoSoundDisconnect is UNIMPLEMENTED. Need implement NtUserCallTwoParam");	
	return 0;
}

DWORD WINAPI RegisterSessionPort(HANDLE a1, HANDLE a2)
{
	DbgPrint("RegisterSessionPort is UNIMPLEMENTED\n"); 
	return 0;
}

BOOL WINAPI SkipPointerFrameMessages(
  _In_  UINT32 pointerId
)
{
	DbgPrint("SkipPointerFrameMessages is UNIMPLEMENTED\n"); 
	return TRUE;
}

BOOL WINAPI GetCIMSSM(INPUT_MESSAGE_SOURCE *inputMessageSource)
{
  BOOL resp; // esi@5
  INPUT_MESSAGE_DEVICE_TYPE type; // [sp+10h] [bp-24h]@2
  INPUT_MESSAGE_ORIGIN_ID origin; // [sp+14h] [bp-20h]@4
  CPPEH_RECORD ms_exc; // [sp+1Ch] [bp-18h]@6

  EnterCriticalSection(lpCriticalSection);
  origin = inputMessageSource->originId ;
  if ( !inputMessageSource || !GetCurrentInputMessageSource(&type, 0) || type || origin )
  {
    SetLastError(87);
    resp = 0;
  }
  else
  {
    resp = GetCurrentInputMessageSource(&type, 1);
    if ( resp )
    {
      ms_exc.registration.TryLevel = 0;
      if ( (PVOID)inputMessageSource >= W32UserProbeAddress )
        W32UserProbeAddress = 0;
      inputMessageSource->deviceType = type;
      inputMessageSource->originId = origin;
      ms_exc.registration.TryLevel = -2;
    }
  }
  LeaveCriticalSection(lpCriticalSection);
  return resp;
}

DWORD WINAPI MBToWCSEx(
		WORD  	CodePage,
		LPSTR  	MBString,
		LONG  	MBSize,
		LPWSTR *  	UnicodeString,
		LONG  	UnicodeSize,
		BOOL  	Allocate 
)
{
    DWORD Size;
    if (MBSize == -1)
    {
        MBSize = strlen(MBString)+1;
    }
    if (UnicodeSize == -1)
    {
        if (!Allocate)
        {
            return 0;
        }
        UnicodeSize = MBSize;
    }
    if (Allocate)
    {
        LPWSTR SafeString = RtlAllocateHeap(GetProcessHeap(), 0, UnicodeSize);
        if (SafeString == NULL)
            return 0;
        *UnicodeString = SafeString;
    }
    UnicodeSize *= sizeof(WCHAR);
    if (CodePage == 0)
    {
        RtlMultiByteToUnicodeN(*UnicodeString,UnicodeSize,&Size,MBString,MBSize);
    }
    else
    {
        Size = MultiByteToWideChar(CodePage,0,MBString,MBSize,*UnicodeString,UnicodeSize);
    }
    return Size;
}

/***********************************************************************
 *           USER_Lock
 */
void USER_Lock(void)
{
    EnterCriticalSection( &user_section );
}


/***********************************************************************
 *           USER_Unlock
 */
void USER_Unlock(void)
{
    LeaveCriticalSection( &user_section );
}
