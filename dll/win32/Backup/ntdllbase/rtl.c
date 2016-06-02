 /*
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
 
#define NDEBUG

#include <main.h>

DWORD error;

PRTLP_UNHANDLED_EXCEPTION_FILTER RtlpUnhandledExceptionFilter;

VOID 
NTAPI
RtlpFreeMemory( 	
	PVOID  	Mem,
	ULONG  	Tag 
) 		
{
    UNREFERENCED_PARAMETER(Tag);

    RtlFreeHeap(RtlGetProcessHeap(),
                0,
                Mem);
}

PVOID 
NTAPI 
RtlpAllocateMemory( 	
	UINT  	Bytes,
	ULONG  	Tag 
) 		
{
    UNREFERENCED_PARAMETER(Tag);

    return RtlAllocateHeap(RtlGetProcessHeap(),
                           0,
                           Bytes);
}

void DbgUserBreakPoint()
{
  __debugbreak();
}

ULONG NTAPI RtlGetCurrentProcessorNumber()
{
  return __segmentlimit(59u) >> 14;
}

VOID 
NTAPI 
RtlGetCurrentProcessorNumberEx(
  _Out_  PPROCESSOR_NUMBER ProcNumber
)
{
	ProcNumber->Number = RtlGetCurrentProcessorNumber();
}

NTSTATUS 
NTAPI 
RtlCloseDefinitionIdentityHandle(HANDLE handle, int verification)
{
  NTSTATUS result; // eax@2

  /*if ( handle )
    result = RtlCloseBaseIdentityHandle(handle, verification);
  else
    result = RtlRaiseStatus(0xC000000Du);
  return result;*/
  return STATUS_SUCCESS;
}

void 
NTAPI 
RtlReportErrorPropagation(LPSTR a1, int a2, int a3, int a4)
{
  ;
}

NTSTATUS 
NTAPI 
RtlGetDefinitionIdentityAttributeValue(HANDLE handle, int two, int three, int four)
{
  NTSTATUS result; // eax@4
  NTSTATUS otherStatus; // eax@5
  NTSTATUS status; // esi@5

  if ( !handle && four )
  {
    otherStatus = STATUS_SUCCESS;
    status = otherStatus;
    if ( otherStatus >= 0 )
    {
      result = 0;
    }
    else
    {
      RtlReportErrorPropagation("z:\\nt\\public\\internal\\base\\inc\\ntehmacros.h", 0, 92, otherStatus);
      result = status;
    }
  }
  else
  {
    result = 0xC000000Du;
  }
  return result;
}

NTSTATUS 
WINAPI 
RtlParseDefinitionIdentity(int a, int b, BOOL c)
{
	return STATUS_SUCCESS;
}

void 
NTAPI 
RtlReportErrorOrigination(int one, char ArgList, int three, int four)
{
  if ( four == error )
    DbgUserBreakPoint();
  if ( four == 0xC00000E5 )
  {
    DbgPrintEx(99, 0, "\n*** Assertion failed: %s\n***   Source File: %s, line %ld\n\n", ArgList);
    RtlRaiseStatus(0x20u);
  }
}

NTSTATUS 
NTAPI 
RtlInitLUnicodeStringFromNullTerminatedString(PUNICODE_STRING a1, const wchar_t *constantString)
{
  PUNICODE_STRING localString; // esi@1
  SIZE_T localLength; // edi@1
  SIZE_T hum; // eax@3
  NTSTATUS result; // eax@4
  PUNICODE_STRING otherString; // eax@6

  localString = a1;
  localLength = (SIZE_T)constantString;
  if ( a1 )
  {
    a1->Length = 0;
    a1->Buffer = 0;
    a1[1].Length = 0;
    if ( constantString )
    {
      hum = wcslen(constantString);
      if ( hum > 0x7FFFFFFE )
      {
        RtlReportErrorOrigination((int)"z:\\nt\\public\\internal\\base\\inc\\ntehmacros.h", 0, 91, 0xC0000424u);
        return 0xC0000424u;
      }
      otherString = (PUNICODE_STRING)(2 * hum);
      localString->Length = otherString->Length;
      localString->Buffer = &otherString->MaximumLength;
      localString[1].Length = localLength;
    }
    result = 0;
  }
  else
  {
    result = 0xC000000Du;
  }
  return result;
}

/*++
 * RtlGetCriticalSectionRecursionCount
 * @implemented NT5.2 SP1
 *
 *     Retrieves the recursion count of a given critical section.
 *
 * Params:
 *     CriticalSection - Critical section to retrieve its recursion count.
 *
 * Returns:
 *     The recursion count.
 *
 * Remarks:
 *     We return the recursion count of the critical section if it is owned
 *     by the current thread, and otherwise we return zero.
 *
 *--*/
LONG
NTAPI
RtlGetCriticalSectionRecursionCount(PRTL_CRITICAL_SECTION CriticalSection)
{
    if (CriticalSection->OwningThread == NtCurrentTeb()->ClientId.UniqueThread)
    {
        /*
         * The critical section is owned by the current thread,
         * therefore retrieve its actual recursion count.
         */
        return CriticalSection->RecursionCount;
    }
    else
    {
        /*
         * It is not owned by the current thread, so
         * for this thread there is no recursion.
         */
        return 0;
    }
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlImageNtHeaderEx(IN ULONG Flags,
                   IN PVOID Base,
                   IN ULONG64 Size,
                   OUT PIMAGE_NT_HEADERS *OutHeaders)
{
    PIMAGE_NT_HEADERS NtHeaders;
    PIMAGE_DOS_HEADER DosHeader;
    BOOLEAN WantsRangeCheck;

    /* You must want NT Headers, no? */
    if (!OutHeaders) return STATUS_INVALID_PARAMETER;

    /* Assume failure */
    *OutHeaders = NULL;

    /* Validate Flags */
    if (Flags &~ RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK)
    {
        DbgPrint("Invalid flag combination... check for new API flags?\n");
        return STATUS_INVALID_PARAMETER;
    }

    /* Validate base */
    if (!(Base) || (Base == (PVOID)-1)) return STATUS_INVALID_PARAMETER;

    /* Check if the caller wants validation */
    WantsRangeCheck = !(Flags & RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK);
    if (WantsRangeCheck)
    {
        /* Make sure the image size is at least big enough for the DOS header */
        if (Size < sizeof(IMAGE_DOS_HEADER))
        {
            DbgPrint("Size too small\n");
            return STATUS_INVALID_IMAGE_FORMAT;
        }
    }

    /* Check if the DOS Signature matches */
    DosHeader = Base;
    if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE)
    {
        /* Not a valid COFF */
        DbgPrint("Not an MZ file\n");
        return STATUS_INVALID_IMAGE_FORMAT;
    }

    /* Check if the caller wants validation */
    if (WantsRangeCheck)
    {
        /* The offset should fit in the passsed-in size */
        if (DosHeader->e_lfanew >= Size)
        {
            /* Fail */
            DbgPrint("e_lfanew is larger than PE file\n");
            return STATUS_INVALID_IMAGE_FORMAT;
        }

        /* It shouldn't be past 4GB either */
        if (DosHeader->e_lfanew >=
            (MAXULONG - sizeof(IMAGE_DOS_SIGNATURE) - sizeof(IMAGE_FILE_HEADER)))
        {
            /* Fail */
            DbgPrint("e_lfanew is larger than 4GB\n");
            return STATUS_INVALID_IMAGE_FORMAT;
        }

        /* And the whole file shouldn't overflow past 4GB */
        if ((DosHeader->e_lfanew +
            sizeof(IMAGE_DOS_SIGNATURE) - sizeof(IMAGE_FILE_HEADER)) >= Size)
        {
            /* Fail */
            DbgPrint("PE is larger than 4GB\n");
            return STATUS_INVALID_IMAGE_FORMAT;
        }
    }

    /* The offset also can't be larger than 256MB, as a hard-coded check */
    if (DosHeader->e_lfanew >= (256 * 1024 * 1024))
    {
        /* Fail */
        DbgPrint("PE offset is larger than 256MB\n");
        return STATUS_INVALID_IMAGE_FORMAT;
    }

    /* Now it's safe to get the NT Headers */
    NtHeaders = (PIMAGE_NT_HEADERS)((ULONG_PTR)Base + DosHeader->e_lfanew);

    /* Verify the PE Signature */
    if (NtHeaders->Signature != IMAGE_NT_SIGNATURE)
    {
        /* Fail */
        DbgPrint("PE signature missing\n");
        return STATUS_INVALID_IMAGE_FORMAT;
    }

    /* Now return success and the NT header */
    *OutHeaders = NtHeaders;
    return STATUS_SUCCESS;
}

LONGLONG
NTAPI
RtlInterlockedCompareExchange64(LONGLONG volatile *Destination,
                                 LONGLONG Exchange,
                                 LONGLONG Comparand)
{
     /* Just call the intrinsic */
     return _InterlockedCompareExchange64(Destination, Exchange, Comparand);
}

VOID 
NTAPI 
RtlSetUnhandledExceptionFilter(
	IN PRTLP_UNHANDLED_EXCEPTION_FILTER  	TopLevelExceptionFilter	
) 	
{
    /* Set the filter which is used by the CriticalSection package */
    RtlpUnhandledExceptionFilter = RtlEncodePointer(TopLevelExceptionFilter);
}

NTSTATUS NTAPI RtlInitAnsiStringEx(
	IN OUT PANSI_STRING  	DestinationString,
	IN PCSZ  	SourceString 
) 		
{
    SIZE_T Size;

    if (SourceString)
    {
        Size = strlen(SourceString);
        if (Size > (MAXUSHORT - sizeof(CHAR))) return STATUS_NAME_TOO_LONG;
        DestinationString->Length = (USHORT)Size;
        DestinationString->MaximumLength = (USHORT)Size + sizeof(CHAR);
    }
    else
    {
        DestinationString->Length = 0;
        DestinationString->MaximumLength = 0;
    }

    DestinationString->Buffer = (PCHAR)SourceString;
    return STATUS_SUCCESS;

}