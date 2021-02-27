/*++

Copyright (c) 2018 Shorthorn Project

Module Name:

    atom.c

Abstract:

    Implement atom functions

Author:

    Skulltrail 17-March-2018

Revision History:

--*/
 
#define NDEBUG

#include <main.h> 

BOOLEAN
NTAPI
RtlGetIntegerAtom(
    PWSTR Name,
    PRTL_ATOM Atom OPTIONAL
    )
{
    NTSTATUS Status;
    UNICODE_STRING UnicodeString;
    PWSTR s;
    ULONG n;
    RTL_ATOM Temp;

    if (((ULONG_PTR)Name & -0x10000) == 0) {
        Temp = (RTL_ATOM)(USHORT)PtrToUlong(Name);
        if (Temp >= RTL_ATOM_MAXIMUM_INTEGER_ATOM) {
            return FALSE;
            }
        else {
            if (Temp == RTL_ATOM_INVALID_ATOM) {
                Temp = RTL_ATOM_MAXIMUM_INTEGER_ATOM;
                }

            if (ARGUMENT_PRESENT( Atom )) {
                *Atom = Temp;
                }

            return TRUE;
            }
        }
    else
    if (*Name != L'#') {
        return FALSE;
        }

    s = ++Name;
    while (*s != UNICODE_NULL) {
        if (*s < L'0' || *s > L'9') {
            return FALSE;
            }
        else {
            s++;
            }
        }

    n = 0;
    UnicodeString.Buffer = Name;
    UnicodeString.Length = (USHORT)((PCHAR)s - (PCHAR)Name);
    UnicodeString.MaximumLength = UnicodeString.Length;
    Status = RtlUnicodeStringToInteger( &UnicodeString, 10, &n );
    if (NT_SUCCESS( Status )) {
        if (ARGUMENT_PRESENT( Atom )) {
            if (n == 0 || n > RTL_ATOM_MAXIMUM_INTEGER_ATOM) {
                *Atom = RTL_ATOM_MAXIMUM_INTEGER_ATOM;
                }
            else {
                *Atom = (RTL_ATOM)n;
                }
            }

        return TRUE;
        }
    else {
        return FALSE;
        }
}
