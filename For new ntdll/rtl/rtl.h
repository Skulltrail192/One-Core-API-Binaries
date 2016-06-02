/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS System Libraries
 * FILE:            lib/rtl/rtl.h
 * PURPOSE:         Run-Time Libary Header
 * PROGRAMMER:      Alex Ionescu
 */

#ifndef RTL_H
#define RTL_H

/* We're a core NT DLL, we don't import syscalls */
#define _INC_SWPRINTF_INL_
#undef __MSVCRT__

/* C Headers */
#include <stdlib.h>
#include <stdio.h>

/* PSDK/NDK Headers */
#define WIN32_NO_STATUS
#define _INC_WINDOWS
#define COM_NO_WINDOWS_H
#define COBJMACROS
#define CONST_VTABLE
#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#include <objbase.h>
#include <ndk/exfuncs.h>
#include <ndk/iofuncs.h>
#include <ndk/kefuncs.h>
#include <ndk/ldrfuncs.h>
#include <ndk/mmfuncs.h>
#include <ndk/obfuncs.h>
#include <ndk/psfuncs.h>
#include <ndk/rtlfuncs.h>
#include <ndk/setypes.h>
#include <ndk/sefuncs.h>
#include <ndk/umfuncs.h>

/* Internal RTL header */
#include "rtlp.h"

/* Use intrinsics for x86 and x64 */
#if defined(_M_IX86) || defined(_M_AMD64)
#define InterlockedCompareExchange _InterlockedCompareExchange
#define InterlockedIncrement _InterlockedIncrement
#define InterlockedDecrement _InterlockedDecrement
#define InterlockedExchangeAdd _InterlockedExchangeAdd
#define InterlockedExchange _InterlockedExchange
#define InterlockedBitTestAndSet _interlockedbittestandset
#define InterlockedBitTestAndSet64 _interlockedbittestandset64
#define 	RtlProcessHeap()   (NtCurrentPeb()->ProcessHeap)
#define RtlAcquireLockRoutine(L)    RtlEnterCriticalSection((PRTL_CRITICAL_SECTION)(L))
#define RtlReleaseLockRoutine(L)    RtlLeaveCriticalSection((PRTL_CRITICAL_SECTION)(L))

/*From Windows 2000 source code*/
#define RTL_QUERY_PROCESS_MODULES       0x00000001
#define RTL_QUERY_PROCESS_BACKTRACES    0x00000002
#define RTL_QUERY_PROCESS_HEAP_SUMMARY  0x00000004
#define RTL_QUERY_PROCESS_HEAP_TAGS     0x00000008
#define RTL_QUERY_PROCESS_HEAP_ENTRIES  0x00000010
#define RTL_QUERY_PROCESS_LOCKS         0x00000020
#define RTL_QUERY_PROCESS_MODULES32     0x00000040
#define RTL_QUERY_PROCESS_NONINVASIVE   0x80000000	

#define ARGUMENT_PRESENT(ArgumentPointer) \
   ((CHAR*)((ULONG_PTR)(ArgumentPointer)) != (CHAR*)NULL)

#if DBG
#define IS_HEAP_TAGGING_ENABLED() (TRUE)
#else
#define IS_HEAP_TAGGING_ENABLED() (RtlGetNtGlobalFlags() & FLG_HEAP_ENABLE_TAGGING)
#endif // DBG

#define IS_DEBUG_PAGE_HEAP_HANDLE( HeapHandle ) \
            (((PHEAP)(HeapHandle))->ForceFlags & HEAP_FLAG_PAGE_ALLOCS )

#define IF_DEBUG_PAGE_HEAP_THEN_RETURN( Handle, ReturnThis )                \
            {                                                               \
            if ( IS_DEBUG_PAGE_HEAP_HANDLE( Handle ))                       \
                {                                                           \
                return ReturnThis;                                          \
                }                                                           \
            }
#endif

#endif /* RTL_H */
