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

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

typedef struct _WOW64_CONTEXT
{
     ULONGLONG ContextFlags;
     ULONGLONG Dr0;
     ULONGLONG Dr1;
     ULONGLONG Dr2;
     ULONGLONG Dr3;
     ULONGLONG Dr6;
     ULONGLONG Dr7;
     FLOATING_SAVE_AREA FloatSave;
     ULONGLONG SegGs;
     ULONGLONG SegFs;
     ULONGLONG SegEs;
     ULONGLONG SegDs;
     ULONGLONG Edi;
     ULONGLONG Esi;
     ULONGLONG Ebx;
     ULONGLONG Edx;
     ULONGLONG Ecx;
     ULONGLONG Eax;
     ULONGLONG Ebp;
     ULONGLONG Eip;
     ULONGLONG SegCs;
     ULONGLONG EFlags;
     ULONGLONG Esp;
     ULONGLONG SegSs;
     UCHAR ExtendedRegisters[512];
} WOW64_CONTEXT, *PWOW64_CONTEXT;

typedef struct _WOW64_LDT_ENTRY {
  WORD  LimitLow;
  WORD  BaseLow;
  union {
    struct {
      BYTE BaseMid;
      BYTE Flags1;
      BYTE Flags2;
      BYTE BaseHi;
    } Bytes;
    struct {
      DWORD BaseMid  :8;
      DWORD Type  :5;
      DWORD Dpl  :2;
      DWORD Pres  :1;
      DWORD LimitHi  :4;
      DWORD Sys  :1;
      DWORD Reserved_0  :1;
      DWORD Default_Big  :1;
      DWORD Granularity  :1;
      DWORD BaseHi  :8;
    } Bits;
  } HighWord;
} WOW64_LDT_ENTRY, *PWOW64_LDT_ENTRY;

DWORD WINAPI Wow64SuspendThread(HANDLE hThread)
{ 
  #ifdef _M_IX86
	BaseSetLastNTError(0xC0000002u);
	return -1;
  #elif defined(_M_AMD64)
	return SuspendThread(hThread);
  #endif  
}

BOOL WINAPI Wow64SetThreadContext(
  _In_  HANDLE hThread,
  _In_  const WOW64_CONTEXT *lpContext
)
{ 
  #ifdef _M_IX86
	BaseSetLastNTError(-1073741822);
	return FALSE;
  #elif defined(_M_AMD64)
	return SetThreadContext(hThread, (CONTEXT)lpContext);
  #endif  
}

BOOL WINAPI Wow64GetThreadContext(
  _In_     HANDLE hThread,
  _Inout_  PWOW64_CONTEXT lpContext
)
{ 
  #ifdef _M_IX86
	BaseSetLastNTError(-1073741822);
	return FALSE;
  #elif defined(_M_AMD64)
	return GetThreadContext(hThread, (CONTEXT)lpContext);
  #endif  
}

BOOL WINAPI Wow64GetThreadSelectorEntry(
  _In_   HANDLE hThread,
  _In_   DWORD dwSelector,
  _Out_  PWOW64_LDT_ENTRY lpSelectorEntry
)
{ 
  #ifdef _M_IX86
	BaseSetLastNTError(0xC00000BBu);
	return FALSE;
  #elif defined(_M_AMD64)
	return GetThreadSelectorEntry(hThread, dwSelector, lpSelectorEntry);
  #endif  
}

