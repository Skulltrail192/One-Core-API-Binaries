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

#include "config.h"
#include <assert.h>
#include <rtl.h>

/*
 * @implemented - new

HANDLE WINAPI RtlGetCurrentTransaction()
{
  #ifdef _M_IX86
	return *(HANDLE *)(__readfsdword(24) + 4012);
  #elif defined(_M_AMD64)
	return *(HANDLE *)(__readgsqword(24) + 4012);
  #endif
}
 */
/*
 * @implemented - need test

BOOL WINAPI RtlSetCurrentTransaction(HANDLE new_transaction)
{
  BOOL result; // eax@2

  if ( new_transaction == (HANDLE)-1 )
  {
    result = 0;
  }
  else
  {
    #ifdef _M_IX86
	*(HANDLE *)(__readfsdword(24) + 4012) = new_transaction;
	#elif defined(_M_AMD64)
	*(HANDLE *)(__readgsqword(24) + 4012) = new_transaction;
	#endif
    result = 1;
  }
  return result;
}
 */

DWORD WINAPI NtRollbackTransaction(int a, int b)
{
	return 0;
}

/*need implementation*/
NTSTATUS WINAPI NtCommitTransaction(HANDLE hTransaction, DWORD options)
{
	return 0;
}

DWORD WINAPI NtCreateTransaction(int a, int b)
{
	return 0;
}
