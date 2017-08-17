/*++

Copyright (c) 2017 Shorthorn Project

Module Name:

    filefind.c

Abstract:

    This module implements Win32 FindFirst/FindNext

Author:

    Skulltrail 19-March-2017

Revision History:

--*/

#include "main.h"

static 
VOID
CopyStreamData(IN OUT PFIND_STREAM_DATA FindStreamData,
               OUT PWIN32_FIND_STREAM_DATA lpFindStreamData)
{
    _SEH2_TRY
    {
        ASSERT(FindStreamData->CurrentInfo);

        switch (FindStreamData->InfoLevel)
        {
            case FindStreamInfoStandard:
            {
                ULONG StreamNameLen = min(FindStreamData->CurrentInfo->StreamNameLength,
                                          sizeof(lpFindStreamData->cStreamName) - sizeof(WCHAR));

                RtlZeroMemory(lpFindStreamData, sizeof(*lpFindStreamData));

                lpFindStreamData->StreamSize.QuadPart = FindStreamData->CurrentInfo->StreamSize.QuadPart;
                RtlCopyMemory(lpFindStreamData->cStreamName,
                              FindStreamData->CurrentInfo->StreamName,
                              StreamNameLen);
                lpFindStreamData->cStreamName[StreamNameLen / sizeof(WCHAR)] = UNICODE_NULL;

                break;
            }

            default:
            {
                /* Invalid InfoLevel */
                ASSERT(FALSE);
                break;
            }
        }
    }
    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
    {
    }
    _SEH2_END;

    return;
}

/*
 * @implemented
 */
HANDLE
WINAPI
FindFirstStreamW(
	IN LPCWSTR lpFileName,
    IN STREAM_INFO_LEVELS InfoLevel,
    OUT LPVOID lpFindStreamData,
    IN DWORD dwFlags
)
{
    PFIND_DATA_HANDLE FindDataHandle = NULL;
    PFIND_STREAM_DATA FindStreamData;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;
    UNICODE_STRING NtFilePath;
    HANDLE FileHandle = NULL;
    NTSTATUS Status;
    ULONG BufferSize = 0;

    if (dwFlags != 0 || InfoLevel != FindStreamInfoStandard ||
        lpFindStreamData == NULL)
    {
        SetLastError(ERROR_INVALID_PARAMETER);
        return INVALID_HANDLE_VALUE;
    }

    /* Validate and translate the filename */
    if (!RtlDosPathNameToNtPathName_U(lpFileName,
                                      &NtFilePath,
                                      NULL, NULL))
    {
        SetLastError(ERROR_PATH_NOT_FOUND);
        return INVALID_HANDLE_VALUE;
    }

    /* Open the file */
    InitializeObjectAttributes(&ObjectAttributes,
                               &NtFilePath,
                               OBJ_CASE_INSENSITIVE,
                               NULL,
                               NULL);

    Status = NtCreateFile(&FileHandle,
                          0,
                          &ObjectAttributes,
                          &IoStatusBlock,
                          NULL, 0,
                          FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
                          FILE_OPEN,
                          0, NULL, 0);
    if (!NT_SUCCESS(Status)) goto Cleanup;

    /*
     * Initialization of the search handle.
     */
    FindDataHandle = RtlAllocateHeap(RtlGetProcessHeap(),
                                     HEAP_ZERO_MEMORY,
                                     sizeof(FIND_DATA_HANDLE) +
                                         sizeof(FIND_STREAM_DATA));
    if (!FindDataHandle)
    {
        Status = STATUS_NO_MEMORY;
        goto Cleanup;
    }

    FindDataHandle->Type = FindStream;
    FindDataHandle->u.FindStreamData = (PFIND_STREAM_DATA)(FindDataHandle + 1);
    FindStreamData = FindDataHandle->u.FindStreamData;

    FindStreamData->InfoLevel = InfoLevel;
    FindStreamData->FileStreamInfo = NULL;
    FindStreamData->CurrentInfo = NULL;

    /* The critical section must always be initialized */
    Status = RtlInitializeCriticalSection(&FindDataHandle->Lock);
    if (!NT_SUCCESS(Status))
    {
        RtlFreeHeap(RtlGetProcessHeap(), 0, FindDataHandle);
        goto Cleanup;
    }

    /* Capture all information about the streams */
    do
    {
        BufferSize += 0x1000;

        if (FindStreamData->FileStreamInfo == NULL)
        {
            FindStreamData->FileStreamInfo = RtlAllocateHeap(RtlGetProcessHeap(),
                                                              HEAP_ZERO_MEMORY,
                                                              BufferSize);
            if (FindStreamData->FileStreamInfo == NULL)
            {
                Status = STATUS_NO_MEMORY;
                break;
            }
        }
        else
        {
            PFILE_STREAM_INFORMATION pfsi;

            pfsi = RtlReAllocateHeap(RtlGetProcessHeap(),
                                     0, // HEAP_ZERO_MEMORY,
                                     FindStreamData->FileStreamInfo,
                                     BufferSize);
            if (pfsi == NULL)
            {
                Status = STATUS_NO_MEMORY;
                break;
            }

            FindStreamData->FileStreamInfo = pfsi;
        }

        Status = NtQueryInformationFile(FileHandle,
                                        &IoStatusBlock,
                                        FindStreamData->FileStreamInfo,
                                        BufferSize,
                                        FileStreamInformation);

    } while (Status == STATUS_BUFFER_TOO_SMALL);

    if (NT_SUCCESS(Status))
    {
        /* Select the first stream and return the information */
        FindStreamData->CurrentInfo = FindStreamData->FileStreamInfo;
        CopyStreamData(FindStreamData, lpFindStreamData);

        /* All done */
        Status = STATUS_SUCCESS;
    }
    else
    {
        if (FindStreamData->FileStreamInfo)
        {
            RtlFreeHeap(RtlGetProcessHeap(), 0, FindStreamData->FileStreamInfo);
        }

        RtlFreeHeap(RtlGetProcessHeap(), 0, FindDataHandle);
    }

Cleanup:
    if (FileHandle) NtClose(FileHandle);

    RtlFreeHeap(RtlGetProcessHeap(), 0, NtFilePath.Buffer);

    if (NT_SUCCESS(Status))
    {
        return (HANDLE)FindDataHandle;
    }
    else
    {
        BaseSetLastNTError(Status);
        return INVALID_HANDLE_VALUE;
    }
}

/*
 * @implemented
 */
BOOL
WINAPI
FindNextStreamW(IN HANDLE hFindStream,
                OUT LPVOID lpFindStreamData)
{
    PFIND_DATA_HANDLE FindDataHandle = (PFIND_DATA_HANDLE)hFindStream;
    PFIND_STREAM_DATA FindStreamData;

    if (hFindStream == NULL || hFindStream == INVALID_HANDLE_VALUE ||
        FindDataHandle->Type != FindStream)
    {
        SetLastError(ERROR_INVALID_HANDLE);
        return FALSE;
    }

    RtlEnterCriticalSection(&FindDataHandle->Lock);

    FindStreamData = FindDataHandle->u.FindStreamData;

    /* Select next stream if possible */
    if (FindStreamData->CurrentInfo->NextEntryOffset != 0)
    {
        FindStreamData->CurrentInfo = (PFILE_STREAM_INFORMATION)((ULONG_PTR)FindStreamData->CurrentInfo +
                                                                 FindStreamData->CurrentInfo->NextEntryOffset);

        /* Return the information */
        CopyStreamData(FindStreamData, lpFindStreamData);

        RtlLeaveCriticalSection(&FindDataHandle->Lock);
        return TRUE;
    }
    else
    {
        RtlLeaveCriticalSection(&FindDataHandle->Lock);

        SetLastError(ERROR_HANDLE_EOF);
        return FALSE;
    }
}