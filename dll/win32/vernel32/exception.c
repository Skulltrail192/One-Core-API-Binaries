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

VOID 
WINAPI 
RaiseFailFastException(
  _In_opt_  PEXCEPTION_RECORD pExceptionRecord,
  _In_opt_  PCONTEXT pContextRecord,
  _In_      DWORD dwFlags
)
{
	RaiseException(pExceptionRecord->ExceptionCode, pExceptionRecord->ExceptionFlags, pExceptionRecord->NumberParameters, &pExceptionRecord->ExceptionInformation[0]);
}