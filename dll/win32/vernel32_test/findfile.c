/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
 *
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
#include <main.h>

typedef struct _FINDFILE_HANDLE
{
RTL_CRITICAL_SECTION FindBufferLock;
HANDLE DirectoryHandle;
PVOID FindBufferBase;
PVOID FindBufferNext;
ULONG FindBufferLength;
ULONG FindBufferValidLength;
} FINDFILE_HANDLE, * PFINDFILE_HANDLE; 

BOOL
APIENTRY
FindNextStreamW(
    HANDLE hFindStream,
    LPVOID lpFindStreamData
    )
/*++

Routine Description:

    Once a successful call has been made to FindFirstStream, subsequent
    matching files can be located using FindNextStream.

    This API is used to continue a file search from a previous call to
    FindFirstStream.  This API returns successfully with the next stream
    If no file match can be found HANDLE_EOF is returned.
    
Arguments:

    hFindStream  - handle obtained previously from FindFirstStreamW
    
    lpFindStreamData - appropriate buffer to contain next stream data

Return Value:

    TRUE if more to enumerate
    
    
--*/
{
    PFINDFILE_HANDLE FindFileHandle = (PFINDFILE_HANDLE)hFindStream;
    PWIN32_FIND_STREAM_DATA FindStreamData = (PWIN32_FIND_STREAM_DATA)lpFindStreamData; 
    PFILE_STREAM_INFORMATION StreamInfo;

    //
    //  Check to see if there is anymore data to return
    //  

    if (FindFileHandle->FindBufferNext == (PCHAR)FindFileHandle->FindBufferBase + FindFileHandle->FindBufferLength) {

        BaseSetLastNTError( STATUS_END_OF_FILE );
        return FALSE;
    }

    //
    //  Transfer the next result into the output buffer
    //  Note if there is not enough space in the cStreamName field we'll just AV
    //  the string size is bounded by max attribute name (255) + 2 colons + 32 chars for the type == 292
    // 
    //

    StreamInfo = (PFILE_STREAM_INFORMATION)FindFileHandle->FindBufferNext;

    FindStreamData->StreamSize.QuadPart = StreamInfo->StreamSize.QuadPart;
    RtlCopyMemory( FindStreamData->cStreamName, StreamInfo->StreamName, StreamInfo->StreamNameLength );
    FindStreamData->cStreamName[StreamInfo->StreamNameLength / sizeof( WCHAR )] = L'\0';

    if (StreamInfo->NextEntryOffset > 0) {
        FindFileHandle->FindBufferNext = (PCHAR)FindFileHandle->FindBufferNext + StreamInfo->NextEntryOffset;
    } else {
        FindFileHandle->FindBufferNext = (PCHAR)FindFileHandle->FindBufferBase + FindFileHandle->FindBufferLength;
    }

    return TRUE;
}
