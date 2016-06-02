/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         Shorthorn kernel
 * FILE:            dll/ntdll/csr/capture.c
 * PURPOSE:         Routines for probing and capturing CSR API Messages
 * PROGRAMMERS:     Alex Ionescu (alex@relsoft.net), Samuel Marins
 *                  Hermes Belusca-Maito (hermes.belusca@sfr.fr)
 */

/* INCLUDES *******************************************************************/

#include <ntdll.h>
#include "csr.h"

#define NDEBUG
#include <debug.h>

/* GLOBALS ********************************************************************/

extern HANDLE CsrPortHeap;

/* FUNCTIONS ******************************************************************/

/*
 * @implemented - Windows 2000 Source Code
 */
VOID
NTAPI
CsrProbeForRead(
    IN PVOID Address,
    IN ULONG Length,
    IN ULONG Alignment
    )

/*++

Routine Description:

    This function probes a structure for read accessibility.
    If the structure is not accessible, then an exception is raised.

Arguments:

    Address - Supplies a pointer to the structure to be probed.

    Length - Supplies the length of the structure.

    Alignment - Supplies the required alignment of the structure expressed
        as the number of bytes in the primitive datatype (e.g., 1 for char,
        2 for short, 4 for long, and 8 for quad).

Return Value:

    None.

--*/

{
    volatile CHAR *StartAddress;
    volatile CHAR *EndAddress;
    CHAR Temp;

    //
    // If the structure has zero length, then do not probe the structure for
    // read accessibility or alignment.
    //

    if (Length != 0) {

        //
        // If the structure is not properly aligned, then raise a data
        // misalignment exception.
        //

        ASSERT((Alignment == 1) || (Alignment == 2) ||
               (Alignment == 4) || (Alignment == 8));
        StartAddress = (volatile CHAR *)Address;

        if (((ULONG_PTR)StartAddress & (Alignment - 1)) != 0) {
            RtlRaiseStatus(STATUS_DATATYPE_MISALIGNMENT);
        } else {
            Temp = *StartAddress;
            EndAddress = StartAddress + Length - 1;
            Temp = *EndAddress;
        }
    }
}

/*
 * @implemented - Windows 2000 Source Code
 */
VOID
NTAPI
CsrProbeForWrite(
    IN PVOID Address,
    IN ULONG Length,
    IN ULONG Alignment
    )

/*++

Routine Description:

    This function probes a structure for read accessibility.
    If the structure is not accessible, then an exception is raised.

Arguments:

    Address - Supplies a pointer to the structure to be probed.

    Length - Supplies the length of the structure.

    Alignment - Supplies the required alignment of the structure expressed
        as the number of bytes in the primitive datatype (e.g., 1 for char,
        2 for short, 4 for long, and 8 for quad).

Return Value:

    None.

--*/

{
    volatile CHAR *StartAddress;
    volatile CHAR *EndAddress;
    CHAR Temp;

    //
    // If the structure has zero length, then do not probe the structure for
    // write accessibility or alignment.
    //

    if (Length != 0) {

        //
        // If the structure is not properly aligned, then raise a data
        // misalignment exception.
        //

        ASSERT((Alignment == 1) || (Alignment == 2) ||
               (Alignment == 4) || (Alignment == 8));
        StartAddress = (volatile CHAR *)Address;

        if (((ULONG_PTR)StartAddress & (Alignment - 1)) != 0) {
            RtlRaiseStatus(STATUS_DATATYPE_MISALIGNMENT);
        } else {
            //
            // BUG, BUG - this should not be necessary once the 386 kernel
            // makes system space inaccessable to user mode.
            //
            if ((ULONG_PTR)StartAddress > CsrNtSysInfo.MaximumUserModeAddress) {
                RtlRaiseStatus(STATUS_ACCESS_VIOLATION);
            }

            Temp = *StartAddress;
            *StartAddress = Temp;
            EndAddress = StartAddress + Length - 1;
            Temp = *EndAddress;
            *EndAddress = Temp;
        }
    }
}

/*
 * @implemented
 */
PCSR_CAPTURE_HEADER
NTAPI
CsrAllocateCaptureBuffer(IN ULONG CountMessagePointers,
                         IN ULONG Size)
/*++

Routine Description:

    This function allocates a buffer from the Port Memory section for
    use by the client in capture arguments into Port Memory.  In addition to
    specifying the size of the data that needs to be captured, the caller
    needs to specify how many pointers to captured data will be passed.
    Pointers can be located in either the request message itself, and/or
    the capture buffer.

Arguments:

    CountMessagePointers - Number of pointers within the request message
        that will point to locations within the allocated capture buffer.

    Size - Total size of the data that will be captured into the capture
        buffer.

Return Value:

    A pointer to the capture buffer header.

--*/

{
    PCSR_CAPTURE_HEADER CaptureBuffer;
    ULONG CountPointers;

    //
    // Calculate the total number of pointers that will be passed
    //

    CountPointers = CountMessagePointers;

    //
    // Calculate the total size of the capture buffer.  This includes the
    // header, the array of pointer offsets and the data length.  We round
    // the data length to a 32-bit boundary, assuming that each pointer
    // points to data whose length is not aligned on a 32-bit boundary.
    //

    if (Size >= MAXLONG) {
        //
        // Bail early if too big
        //
        return NULL;
        }
    Size += FIELD_OFFSET(CSR_CAPTURE_HEADER, MessagePointerOffsets) + (CountPointers * sizeof( PVOID ));
    Size = (Size + (3 * (CountPointers+1))) & ~3;

    //
    // Allocate the capture buffer from the Port Memory Heap.
    //

    CaptureBuffer = RtlAllocateHeap( CsrPortHeap, /*MAKE_CSRPORT_TAG( CAPTURE_TAG )*/ 0, Size );
    if (CaptureBuffer == NULL) {

        //
        // FIX, FIX - need to attempt the receive lost reply messages to
        // to see if they contain CaptureBuffer pointers that can be freed.
        //

        return( NULL );
    }

    //
    // Initialize the capture buffer header
    //

    CaptureBuffer->Length = Size;
    CaptureBuffer->CountMessagePointers = 0;

    //
    // If there are pointers being passed then initialize the arrays of
    // pointer offsets to zero.  In either case set the free space pointer
    // in the capture buffer header to point to the first 32-bit aligned
    // location after the header, the arrays of pointer offsets are considered
    // part of the header.
    //

    RtlZeroMemory( CaptureBuffer->MessagePointerOffsets,
                   CountPointers * sizeof( ULONG_PTR )
                 );

    CaptureBuffer->FreeSpace = (PCHAR)
        (CaptureBuffer->MessagePointerOffsets + CountPointers);

    //
    // Returned the address of the capture buffer.
    //

    return( CaptureBuffer );
}


/*
 * @implemented
 */
ULONG
NTAPI
CsrAllocateMessagePointer(IN OUT PCSR_CAPTURE_BUFFER CaptureBuffer,
                          IN ULONG MessageLength,
                          OUT PVOID* CapturedData)
{
    if (MessageLength == 0)
    {
        *CapturedData = NULL;
        CapturedData = NULL;
    }
    else
    {
        /* Set the capture data at our current available buffer */
        *CapturedData = CaptureBuffer->BufferEnd;

        /* Validate the size */
        if (MessageLength >= MAXLONG) return 0;

        /* Align it to a 4-byte boundary */
        MessageLength = (MessageLength + 3) & ~3;

        /* Move our available buffer beyond this space */
        CaptureBuffer->BufferEnd = (PVOID)((ULONG_PTR)CaptureBuffer->BufferEnd + MessageLength);
    }

    /* Write down this pointer in the array and increase the count */
    CaptureBuffer->PointerOffsetsArray[CaptureBuffer->PointerCount++] = (ULONG_PTR)CapturedData;

    /* Return the aligned length */
    return MessageLength;
}

/*
 * @implemented
 */
VOID
NTAPI
CsrCaptureMessageBuffer(IN OUT PCSR_CAPTURE_BUFFER CaptureBuffer,
                        IN PVOID MessageBuffer OPTIONAL,
                        IN ULONG MessageLength,
                        OUT PVOID* CapturedData)
{
    /* Simply allocate a message pointer in the buffer */
    CsrAllocateMessagePointer(CaptureBuffer, MessageLength, CapturedData);

    /* Check if there was any data */
    if (!MessageBuffer || !MessageLength) return;

    /* Copy the data into the buffer */
    RtlMoveMemory(*CapturedData, MessageBuffer, MessageLength);
}

/*
 * @implemented
 */
VOID
NTAPI
CsrFreeCaptureBuffer(IN PCSR_CAPTURE_BUFFER CaptureBuffer)
{
    /* Free it from the heap */
    RtlFreeHeap(CsrPortHeap, 0, CaptureBuffer);
}

/*
 * @implemented
 */
VOID
NTAPI
CsrCaptureMessageString(IN OUT PCSR_CAPTURE_BUFFER CaptureBuffer,
                        IN PCSTR String OPTIONAL,
                        IN ULONG StringLength,
                        IN ULONG MaximumLength,
                        OUT PSTRING CapturedString)
{
    ASSERT(CapturedString != NULL);

    /*
     * If we don't have a string, initialize an empty one,
     * otherwise capture the given string.
     */
    if (!String)
    {
        CapturedString->Length = 0;
        CapturedString->MaximumLength = (USHORT)MaximumLength;

        /* Allocate a pointer for it */
        CsrAllocateMessagePointer(CaptureBuffer,
                                  MaximumLength,
                                  (PVOID*)&CapturedString->Buffer);
    }
    else
    {
        /* Cut-off the string length if needed */
        if (StringLength > MaximumLength)
            StringLength = MaximumLength;

        CapturedString->Length = (USHORT)StringLength;

        /* Allocate a buffer and get its size */
        CapturedString->MaximumLength =
            (USHORT)CsrAllocateMessagePointer(CaptureBuffer,
                                              MaximumLength,
                                              (PVOID*)&CapturedString->Buffer);

        /* If the string has data, copy it into the buffer */
        if (StringLength)
            RtlMoveMemory(CapturedString->Buffer, String, StringLength);
    }

    /* Null-terminate the string if we don't take up the whole space */
    if (CapturedString->Length < CapturedString->MaximumLength)
        CapturedString->Buffer[CapturedString->Length] = '\0';
}

static VOID
CsrCaptureMessageUnicodeStringInPlace(IN OUT PCSR_CAPTURE_BUFFER CaptureBuffer,
                                      IN PUNICODE_STRING String)
{
    ASSERT(String != NULL);

    /* This is a way to capture the UNICODE string, since (Maximum)Length are also in bytes */
    CsrCaptureMessageString(CaptureBuffer,
                            (PCSTR)String->Buffer,
                            String->Length,
                            String->MaximumLength,
                            (PSTRING)String);

    /* Null-terminate the string */
    if (String->MaximumLength >= String->Length + sizeof(WCHAR))
    {
        String->Buffer[String->Length / sizeof(WCHAR)] = L'\0';
    }
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
CsrCaptureMessageMultiUnicodeStringsInPlace(OUT PCSR_CAPTURE_HEADER* CaptureBuffer,
                                            IN ULONG StringsCount,
                                            IN PUNICODE_STRING* MessageStrings)
{
    ULONG Count;

    if (!CaptureBuffer) return STATUS_INVALID_PARAMETER;

    /* Allocate a new capture buffer if we don't have one already */
    if (!*CaptureBuffer)
    {
        /* Compute the required size for the capture buffer */
        ULONG Size = 0;

        Count = 0;
        while (Count < StringsCount)
        {
            if (MessageStrings[Count])
                Size += MessageStrings[Count]->MaximumLength;

            ++Count;
        }

        /* Allocate the capture buffer */
        *CaptureBuffer = CsrAllocateCaptureBuffer(StringsCount, Size);
        if (!*CaptureBuffer) return STATUS_NO_MEMORY;
    }

    /* Now capture each UNICODE string */
    Count = 0;
    while (Count < StringsCount)
    {
        if (MessageStrings[Count])
            CsrCaptureMessageUnicodeStringInPlace((PCSR_CAPTURE_BUFFER)*CaptureBuffer, MessageStrings[Count]);

        ++Count;
    }

    return STATUS_SUCCESS;
}

/*
 * @implemented
 */
PLARGE_INTEGER
NTAPI
CsrCaptureTimeout(IN ULONG Milliseconds,
                  OUT PLARGE_INTEGER Timeout)
{
    /* Validate the time */
    if (Milliseconds == -1) return NULL;

    /* Convert to relative ticks */
    Timeout->QuadPart = Milliseconds * -10000LL;
    return Timeout;
}

/* EOF */
