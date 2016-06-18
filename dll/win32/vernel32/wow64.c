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

DWORD 
WINAPI 
Wow64SuspendThread(
	HANDLE hThread
)
{ 
  #ifdef _M_IX86
	BaseSetLastNTError(STATUS_NOT_IMPLEMENTED);
	return -1;
  #elif defined(_M_AMD64)
	return SuspendThread(hThread);
  #endif  
}

BOOL 
WINAPI 
Wow64SetThreadContext(
  _In_  HANDLE hThread,
  _In_  const WOW64_CONTEXT *lpContext
)
{ 
  #ifdef _M_IX86
	BaseSetLastNTError(-1073741822);
	return FALSE;
  #elif defined(_M_AMD64)
	return SetThreadContext(hThread, lpContext);
  #endif  
}

BOOL 
WINAPI 
Wow64GetThreadContext(
  _In_     HANDLE hThread,
  _Inout_  PWOW64_CONTEXT lpContext
)
{ 
  #ifdef _M_IX86
	BaseSetLastNTError(-1073741822);
	return FALSE;
  #elif defined(_M_AMD64)
	return GetThreadContext(hThread, lpContext);
  #endif  
}

BOOL 
WINAPI 
Wow64GetThreadSelectorEntry(
  _In_   HANDLE hThread,
  _In_   DWORD dwSelector,
  _Out_  PWOW64_LDT_ENTRY lpSelectorEntry
)
{ 
  #ifdef _M_IX86
	BaseSetLastNTError(STATUS_NOT_SUPPORTED);
	return FALSE;
  #elif defined(_M_AMD64)
	return GetThreadSelectorEntry(hThread, dwSelector, lpSelectorEntry);
  #endif  
}

BOOL 
WINAPI 
Wow64DisableWow64FsRedirection(
	PVOID *OldValue
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = RtlWow64EnableFsRedirectionEx((PVOID)1, OldValue);
  if ( status >= 0 )
  {
    result = TRUE;
  }
  else
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  return result;
}

BOOL 
WINAPI 
Wow64RevertWow64FsRedirection(
	PVOID OlValue
)
{
  NTSTATUS status; // eax@1
  BOOL result; // eax@2

  status = RtlWow64EnableFsRedirectionEx(OlValue, &OlValue);
  if ( status >= 0 )
  {
    result = TRUE;
  }
  else
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  return result;
}

BOOLEAN 
WINAPI 
Wow64EnableWow64FsRedirection(
	BOOLEAN Wow64FsEnableRedirection
)
{
  NTSTATUS status; // eax@1
  BOOLEAN result; // al@2

  status = RtlWow64EnableFsRedirection(Wow64FsEnableRedirection);
  if ( status >= 0 )
  {
    result = TRUE;
  }
  else
  {
    BaseSetLastNTError(status);
    result = FALSE;
  }
  return result;
}