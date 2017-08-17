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
Copyright (c) 2015  Microsoft Corporation 
 
Module Name:

    cnvint.c
 
 */
 
#define NDEBUG

#include <main.h> 

BOOLEAN
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
