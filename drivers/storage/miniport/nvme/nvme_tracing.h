/**
*******************************************************************************
** Copyright (c) 2011-2012                                                   **
**                                                                           **
**   Integrated Device Technology, Inc.                                      **
**   Intel Corporation                                                       **
**   LSI Corporation                                                         **
**                                                                           **
** All rights reserved.                                                      **
**                                                                           **
*******************************************************************************
**                                                                           **
** Redistribution and use in source and binary forms, with or without        **
** modification, are permitted provided that the following conditions are    **
** met:                                                                      **
**                                                                           **
**   1. Redistributions of source code must retain the above copyright       **
**      notice, this list of conditions and the following disclaimer.        **
**                                                                           **
**   2. Redistributions in binary form must reproduce the above copyright    **
**      notice, this list of conditions and the following disclaimer in the  **
**      documentation and/or other materials provided with the distribution. **
**                                                                           **
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS   **
** IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, **
** THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR    **
** PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR         **
** CONTRIBUTORS BE LIABLE FOR ANY DIRECT,INDIRECT, INCIDENTAL, SPECIAL,      **
** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       **
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        **
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    **
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      **
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        **
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              **
**                                                                           **
** The views and conclusions contained in the software and documentation     **
** are those of the authors and should not be interpreted as representing    **
** official policies, either expressed or implied, of Intel Corporation,     **
** Integrated Device Technology Inc., or Sandforce Corporation.              **
**                                                                           **
*******************************************************************************
**/
#pragma once
#ifndef NVME_TRACING_H
#define NVME_TRACING_H

#include <evntrace.h>
#include <stortrce.h>

// Declare the WPP Trace Provider, which will be controlled by the WinCIL GUID.
// NVMe control GUID = {0BD0EE0C-BB49-45F4-A798-80D221BE3DE1}
#define WPP_CONTROL_GUIDS \
    WPP_DEFINE_CONTROL_GUID(NVMe, (0F5937E9, FEAB, 40FF, BBAE, 9C2202C54B1F), \
    WPP_DEFINE_BIT(Default) )

// This tracing setup currently uses the following levels (defined in evntrace.h):
//      TRACE_LEVEL_NONE        0   // Tracing is not on
//      TRACE_LEVEL_FATAL       1   // Abnormal exit or termination
//      TRACE_LEVEL_ERROR       2   // Severe errors that need logging
//      TRACE_LEVEL_WARNING     3   // Warnings such as allocation failure
//      TRACE_LEVEL_INFORMATION 4   // Includes non-error cases(for example, Entry-Exit)
//      TRACE_LEVEL_VERBOSE     5   // Detailed traces from intermediate steps
//
// Note: The tracing setup does NOT currently use any flags.  To define level-specific
//      logging macros, we define custom WPP_LEVEL_ENABLED and WPP_LEVEL_LOGGER macros
//      below.  See the 'sources' file as well for declarations of the tracing macros.
//      We are using custom tracing macros explicitly here, rather than the auto-generated
//      ones via WPP_USE_TRACE_LEVELS, so that we can add component flags in the future.
#define WPP_LEVEL_LOGGER(Level) (WPP_CONTROL(WPP_BIT_ ## Default).Logger),
#define WPP_LEVEL_ENABLED(lvl) (WPP_CONTROL(WPP_BIT_ ## Default).Level >= (lvl))

/*
#ifndef DBG
// For FRE / Retail builds, override StorPortDebugPrint to log ETW messages for
// collection in usermode.
// #define StorPortDebugPrint(lvl, Message, ...)   DoTraceMessage((lvl), Message, __VA_ARGS__)
#else
// #define StorPortDebugPrint StorPortDebugPrint
// For CHK / Debug builds, retain normal StorPortDebugPrint behavior.  To view storport
// debug messages, attach a kernel debugger to the target machine, then break in during
// boot and run the following commands (requires correct symbol path from .symfix!)
// > .reload /f ntoskrnl.exe
// > ed nt!kd_STORMINIPORT_mask 0xf
// > g
#endif
*/

#define TraceFatal(Message, ...)          DoTraceMessage(TRACE_LEVEL_FATAL, Message, __VA_ARGS__)
#define TraceError(Message, ...)          DoTraceMessage(TRACE_LEVEL_ERROR, Message, __VA_ARGS__)
#define TraceWarning(Message, ...)        DoTraceMessage(TRACE_LEVEL_WARNING, Message, __VA_ARGS__)
#define TraceInformation(Message, ...)    DoTraceMessage(TRACE_LEVEL_INFORMATION, Message, __VA_ARGS__)
#define TraceVerbose(Message, ...)        DoTraceMessage(TRACE_LEVEL_VERBOSE, Message, __VA_ARGS__)

#endif
