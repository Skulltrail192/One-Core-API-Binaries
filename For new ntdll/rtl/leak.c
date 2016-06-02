/*
 * COPYRIGHT:         See COPYING in the top level directory
 * PROJECT:           ReactOS system libraries
 * FILE:              lib/rtl/ppb.c
 * PURPOSE:           Process parameters functions
 * PROGRAMMER:        Ariadne ( ariadne@xs4all.nl)
 */

/* INCLUDES ****************************************************************/

#include <rtl.h>

#define NDEBUG
#include <debug.h>

//Windows 2000 Source Code Section
NTSTATUS
NTAPI
RtlCheckProcessParameters(PVOID p1,
                          PWSTR p2,
                          PULONG p3,
                          ULONG v1)
{
    while (*p2) {
        p3[2] = p3[1];
        p3[1] = p3[0];
        p3[0] = *p2;
        p2++;
    }

    v1 = * (volatile WCHAR *) p2;
    v1 *= ((PULONG) p1)[0];
    p3[v1] += ((PULONG) p1)[2];
    v1 += 2;
    p3[v1] = v1 * 3;
    return (NTSTATUS) 0xc0000578;
}