/*++

Copyright (C) Shorthorn Project, 2019

Module Name:

    util.h

Abstract:

    Internal support routines used in STORAHCI

Notes:

Revision History:

--*/

__inline
UCHAR
NumberOfSetBits (
    __in ULONG Value
    )
/*++
    This routine emulates the __popcnt intrinsic function.

Return Value:
    Count of '1's in the ULONG value
--*/
{
    //
    // Partition into groups of bit pairs. Compute population count for each
    // bit pair.
    //
    Value -= (Value >> 1) & 0x55555555;

    //
    // Sum population count of adjacent pairs into quads.
    //
    Value = (Value & 0x33333333) + ((Value >> 2) & 0x33333333);

    //
    // Sum population count of adjacent quads into octets. Lower quad in each
    // octet has desired sum and upper quad is garbage.
    //
    Value = (Value + (Value >> 4)) & 0x0F0F0F0F;

    //
    // The lower quads in each octet must now be accumulated by multiplying with
    // a magic multiplier:
    //
    //   0p0q0r0s * 0x01010101 =         :0p0q0r0s
    //                                 0p:0q0r0s
    //                               0p0q:0r0s
    //                             0p0q0r:0s
    //                           000pxxww:vvuutt0s
    //
    // The octet vv contains the final interesting result.
    //
    Value *= 0x01010101;

    return (UCHAR)(Value >> 24);
}

FORCEINLINE UCHAR
SrbGetPathId(
    __in PVOID Srb
    )
{
    return ((PSCSI_REQUEST_BLOCK)Srb)->PathId;;
}


FORCEINLINE ULONG
SrbGetSrbFunction(
    __in PVOID Srb
    )
{
	return (ULONG)((PSCSI_REQUEST_BLOCK)Srb)->Function;
}

FORCEINLINE PVOID
SrbGetDataBuffer(
    __in PVOID Srb
    )
{
    return  ((PSCSI_REQUEST_BLOCK)Srb)->DataBuffer;
}

FORCEINLINE ULONG
SrbGetDataTransferLength(
    __in PVOID Srb
    )
{
    return ((PSCSI_REQUEST_BLOCK)Srb)->DataTransferLength;
}
