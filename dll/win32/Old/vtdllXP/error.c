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
#include <evntprov.h>
#include <ndk/cmfuncs.h>

#define SEM_FAILCRITICALERRORS   0x0001
#define SEM_NOGPFAULTERRORBOX   0x0002
#define RTL_SEM_FAILCRITICALERRORS   (SEM_FAILCRITICALERRORS << 4)
#define RTL_SEM_NOGPFAULTERRORBOX   (SEM_NOGPFAULTERRORBOX << 4)
#define RTL_SEM_NOALIGNMENTFAULTEXCEPT   (SEM_NOALIGNMENTFAULTEXCEPT << 4)


NTSTATUS NTAPI RtlSetThreadErrorMode( 	
	IN ULONG  NewMode,
	OUT PULONG OldMode  OPTIONAL 
) 		
{
    PTEB Teb = NtCurrentTeb();

    /* Ignore invalid error modes */
    if (NewMode & ~(RTL_SEM_FAILCRITICALERRORS |
          /**made by samuel*/          RTL_SEM_NOGPFAULTERRORBOX |
                    RTL_SEM_NOALIGNMENTFAULTEXCEPT))
    {
        //DPRINT1("Invalid error mode\n");
        return STATUS_INVALID_PARAMETER_1;
    }

    /* Return old mode */
    if (OldMode) *OldMode = Teb->HardErrorMode;
    
    /* Set new one and return success */
    Teb->HardErrorMode = NewMode;
    return STATUS_SUCCESS;
}

ULONG NTAPI RtlGetThreadErrorMode() 	
{
    /* Return it from the TEB */
    return NtCurrentTeb()->HardErrorMode;
}
