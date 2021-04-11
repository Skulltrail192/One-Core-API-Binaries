/*
 * COPYRIGHT:  See COPYING in the top level directory
 * PROJECT:    ReactOS system libraries
 * FILE:       lib/kernel32/thread/i386/thunk.S
 * PURPOSE:    Thread Start Thunks
 * PROGRAMMER: Alex Ionescu (alex@relsoft.net)
 */

#include <asm.inc>
#include <ks386.inc>

.code

EXTERN _BaseThreadStart@8:PROC
EXTERN _BaseProcessStart@4:PROC

PUBLIC _BaseThreadStartThunk@8
_BaseThreadStartThunk@8:
    
    /* Start out fresh */
    xor ebp, ebp
    
    push ebx                  /* lpParameter */
    push eax                  /* lpStartAddress */
    push 0                    /* Return EIP */
    jmp _BaseThreadStart@8


PUBLIC _BaseProcessStartThunk@8
_BaseProcessStartThunk@8:
    /* Start out fresh */
    xor ebp, ebp
    
    push eax                  /* lpStartAddress */
    push 0                    /* Return EIP */
    jmp _BaseProcessStart@4


END

/* EOF */
