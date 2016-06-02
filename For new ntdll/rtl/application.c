/*
 * COPYRIGHT:         See COPYING in the top level directory
 * PROJECT:           ReactOS system libraries
 * FILE:              lib/rtl/application.c
 * PURPOSE:           Apllication verifier stop
 * PROGRAMMER:        Rex Jolliff (rex@lvcablemodem.com)
 */

/* INCLUDES *****************************************************************/

#include <rtl.h>

#define NDEBUG
#include <debug.h>

NTSTATUS WINAPI RtlApplicationVerifierStop(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10)
{
	return STATUS_SUCCESS;
}