/*
 * COPYRIGHT:  See COPYING in the top level directory
 * PROJECT:    ReactOS system libraries
 * FILE:       dll/win32/kernel32/client/i386/thread.S
 * PURPOSE:    Thread Start Thunks
 * PROGRAMMER: Alex Ionescu (alex@relsoft.net)
 */

#include <asm.inc>
#include <ks386.inc>

.code

EXTERN _BaseThreadStartup@8:PROC
EXTERN _BaseProcessStartup@4:PROC

PUBLIC _BaseThreadStartupThunk@8
_BaseThreadStartupThunk@8:
    
    /* Start out fresh */
    xor ebp, ebp
    
    push ebx                  /* lpParameter */
    push eax                  /* lpStartAddress */
    push 0                    /* Return EIP */
    jmp _BaseThreadStartup@8


PUBLIC _BaseProcessStartThunk@4
_BaseProcessStartThunk@4:
    /* Start out fresh */
    xor ebp, ebp
    
    push eax                  /* lpStartAddress */
    push 0                    /* Return EIP */
    jmp _BaseProcessStartup@4


END

/* EOF */
