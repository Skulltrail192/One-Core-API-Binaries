/*++

Copyright (c) 2021  Shorthorn Project

Module Name:

    eventlog.c

Abstract:

    Event Log functions

Author:

    Skulltrail 08-March-2021

Revision History:

--*/

#include "main.h"

WINE_DEFAULT_DEBUG_CHANNEL(advapi32);

/* GLOBALS *******************************************************************/

static const EVENTLOGEOF EOFRecord =
{
    sizeof(EOFRecord),
    0x11111111, 0x22222222, 0x33333333, 0x44444444,
    0, 0, 0, 0,
    sizeof(EOFRecord)
};

#define OFFSET_INFO_INCREMENT   64

static LIST_ENTRY LogHandleListHead;
static CRITICAL_SECTION LogHandleListCs;

typedef BOOL (WINAPI *ELF_REPORT_EVENT_AND_SOURCE)(
						 HANDLE,
                         ULONG,
                         PUNICODE_STRING,
                         USHORT,
                         USHORT,
                         ULONG,
                         PSID,
                         PUNICODE_STRING,
                         USHORT,
                         ULONG,
                         PUNICODE_STRING*,
                         PVOID,
                         USHORT,
                         PULONG,
                         PULONG);
						 

static PLOGHANDLE
ElfGetLogHandleEntryByHandle(IELF_HANDLE EventLogHandle)
{
    PLIST_ENTRY CurrentEntry;
    PLOGHANDLE Handle, pLogHandle = NULL;

    EnterCriticalSection(&LogHandleListCs);

    CurrentEntry = LogHandleListHead.Flink;
    while (CurrentEntry != &LogHandleListHead)
    {
        Handle = CONTAINING_RECORD(CurrentEntry,
                                   LOGHANDLE,
                                   LogHandleListEntry);
        CurrentEntry = CurrentEntry->Flink;

        if (Handle == EventLogHandle)
        {
            pLogHandle = Handle;
            break;
        }
    }

    LeaveCriticalSection(&LogHandleListCs);

    return pLogHandle;
}	

PEVENTLOGRECORD
LogfAllocAndBuildNewRecord(PSIZE_T pRecSize,
                           ULONG   Time,
                           USHORT  wType,
                           USHORT  wCategory,
                           ULONG   dwEventId,
                           PUNICODE_STRING SourceName,
                           PUNICODE_STRING ComputerName,
                           ULONG   dwSidLength,
                           PSID    pUserSid,
                           USHORT  wNumStrings,
                           PWSTR   pStrings,
                           ULONG   dwDataSize,
                           PVOID   pRawData)
{
    SIZE_T RecSize;
    SIZE_T SourceNameSize, ComputerNameSize, StringLen;
    PBYTE Buffer;
    PEVENTLOGRECORD pRec;
    PWSTR str;
    UINT i, pos;

    SourceNameSize   = (SourceName   && SourceName->Buffer)   ? SourceName->Length   : 0;
    ComputerNameSize = (ComputerName && ComputerName->Buffer) ? ComputerName->Length : 0;

    RecSize = sizeof(EVENTLOGRECORD) + /* Add the sizes of the strings, NULL-terminated */
        SourceNameSize + ComputerNameSize + 2*sizeof(UNICODE_NULL);

    /* Align on DWORD boundary for the SID */
    RecSize = ROUND_UP(RecSize, sizeof(ULONG));

    RecSize += dwSidLength;

    /* Add the sizes for the strings array */
    ASSERT((pStrings == NULL && wNumStrings == 0) ||
           (pStrings != NULL && wNumStrings >= 0));
    for (i = 0, str = pStrings; i < wNumStrings; i++)
    {
        StringLen = wcslen(str) + 1; // str must be != NULL
        RecSize += StringLen * sizeof(WCHAR);
        str += StringLen;
    }

    /* Add the data size */
    RecSize += dwDataSize;

    /* Align on DWORD boundary for the full structure */
    RecSize = ROUND_UP(RecSize, sizeof(ULONG));

    /* Size of the trailing 'Length' member */
    RecSize += sizeof(ULONG);

    Buffer = RtlAllocateHeap(GetProcessHeap(), HEAP_ZERO_MEMORY, RecSize);
    if (!Buffer)
    {
        DbgPrint("Cannot allocate heap!\n");
        return NULL;
    }

    pRec = (PEVENTLOGRECORD)Buffer;
    pRec->Length = RecSize;
    pRec->Reserved = LOGFILE_SIGNATURE;

    /*
     * Do not assign here any precomputed record number to the event record.
     * The true record number will be assigned atomically and sequentially in
     * LogfWriteRecord, so that all the event records will have consistent and
     * unique record numbers.
     */
    pRec->RecordNumber = 0;

    /*
     * Set the generated time, and temporarily set the written time
     * with the generated time.
     */
    pRec->TimeGenerated = Time;
    pRec->TimeWritten   = Time;

    pRec->EventID = dwEventId;
    pRec->EventType = wType;
    pRec->EventCategory = wCategory;

    pos = sizeof(EVENTLOGRECORD);

    /* NOTE: Equivalents of RtlStringCbCopyUnicodeString calls */
    if (SourceNameSize)
    {
        StringCbCopyNW((PWSTR)(Buffer + pos), SourceNameSize + sizeof(UNICODE_NULL),
                       SourceName->Buffer, SourceNameSize);
    }
    pos += SourceNameSize + sizeof(UNICODE_NULL);
    if (ComputerNameSize)
    {
        StringCbCopyNW((PWSTR)(Buffer + pos), ComputerNameSize + sizeof(UNICODE_NULL),
                       ComputerName->Buffer, ComputerNameSize);
    }
    pos += ComputerNameSize + sizeof(UNICODE_NULL);

    /* Align on DWORD boundary for the SID */
    pos = ROUND_UP(pos, sizeof(ULONG));

    pRec->UserSidLength = 0;
    pRec->UserSidOffset = 0;
    if (dwSidLength)
    {
        RtlCopyMemory(Buffer + pos, pUserSid, dwSidLength);
        pRec->UserSidLength = dwSidLength;
        pRec->UserSidOffset = pos;
        pos += dwSidLength;
    }

    pRec->StringOffset = pos;
    for (i = 0, str = pStrings; i < wNumStrings; i++)
    {
        StringLen = wcslen(str) + 1; // str must be != NULL
        StringCchCopyW((PWSTR)(Buffer + pos), StringLen, str);
        str += StringLen;
        pos += StringLen * sizeof(WCHAR);
    }
    pRec->NumStrings = wNumStrings;

    pRec->DataLength = 0;
    pRec->DataOffset = 0;
    if (dwDataSize)
    {
        RtlCopyMemory(Buffer + pos, pRawData, dwDataSize);
        pRec->DataLength = dwDataSize;
        pRec->DataOffset = pos;
        pos += dwDataSize;
    }

    /* Align on DWORD boundary for the full structure */
    pos = ROUND_UP(pos, sizeof(ULONG));

    /* Initialize the trailing 'Length' member */
    *((PDWORD)(Buffer + pos)) = RecSize;

    *pRecSize = RecSize;
    return pRec;
}

/* Returns 0 if nothing is found */
static ULONG
ElfpOffsetByNumber(
    IN PEVTLOGFILE LogFile,
    IN ULONG RecordNumber)
{
    UINT i;

    for (i = 0; i < LogFile->OffsetInfoNext; i++)
    {
        if (LogFile->OffsetInfo[i].EventNumber == RecordNumber)
            return LogFile->OffsetInfo[i].EventOffset;
    }
    return 0;
}

static BOOL
ElfpDeleteOffsetInformation(
    IN PEVTLOGFILE LogFile,
    IN ULONG ulNumberMin,
    IN ULONG ulNumberMax)
{
    UINT i;

    if (ulNumberMin > ulNumberMax)
        return FALSE;

    /* Remove records ulNumberMin to ulNumberMax inclusive */
    while (ulNumberMin <= ulNumberMax)
    {
        /*
         * As the offset information is listed in increasing order, and we want
         * to keep the list without holes, we demand that ulNumberMin is the first
         * element in the list.
         */
        if (ulNumberMin != LogFile->OffsetInfo[0].EventNumber)
            return FALSE;

        /*
         * RtlMoveMemory(&LogFile->OffsetInfo[0],
         *               &LogFile->OffsetInfo[1],
         *               sizeof(EVENT_OFFSET_INFO) * (LogFile->OffsetInfoNext - 1));
         */
        for (i = 0; i < LogFile->OffsetInfoNext - 1; i++)
        {
            LogFile->OffsetInfo[i].EventNumber = LogFile->OffsetInfo[i + 1].EventNumber;
            LogFile->OffsetInfo[i].EventOffset = LogFile->OffsetInfo[i + 1].EventOffset;
        }
        LogFile->OffsetInfoNext--;

        /* Go to the next offset information */
        ulNumberMin++;
    }

    return TRUE;
}

static NTSTATUS
WriteLogBuffer(
    IN  PEVTLOGFILE LogFile,
    IN  PVOID   Buffer,
    IN  SIZE_T  Length,
    OUT PSIZE_T WrittenLength OPTIONAL,
    IN  PLARGE_INTEGER ByteOffset,
    OUT PLARGE_INTEGER NextOffset OPTIONAL)
{
    NTSTATUS Status;
    LARGE_INTEGER FileOffset;
    SIZE_T BufSize;
    SIZE_T WrittenBufLength = 0, OldWrittenBufLength;

    /* We must have write access to the log file */
    ASSERT(!LogFile->ReadOnly);

    /*
     * It is expected that the log file is already correctly expanded
     * before we can write in it. Therefore the following assertions hold.
     */
    ASSERT(LogFile->CurrentSize <= LogFile->Header.MaxSize);
    ASSERT(ByteOffset->QuadPart <= LogFile->CurrentSize);

    if (WrittenLength)
        *WrittenLength = 0;

    if (NextOffset)
        NextOffset->QuadPart = 0LL;

    /* Write the first part of the buffer */
    FileOffset = *ByteOffset;
    BufSize = min(Length, LogFile->CurrentSize - FileOffset.QuadPart);

    Status = LogFile->FileWrite(LogFile,
                                &FileOffset,
                                Buffer,
                                BufSize,
                                &WrittenBufLength);
    if (!NT_SUCCESS(Status))
    {
        DbgPrint("FileWrite() failed (Status 0x%08lx)\n", Status);
        return Status;
    }

    if (Length > BufSize)
    {
        OldWrittenBufLength = WrittenBufLength;

        /*
         * The buffer was splitted in two, its second part
         * is written at the beginning of the log.
         */
        Buffer = (PVOID)((ULONG_PTR)Buffer + BufSize);
        BufSize = Length - BufSize;
        FileOffset.QuadPart = sizeof(EVENTLOGHEADER);

        Status = LogFile->FileWrite(LogFile,
                                    &FileOffset,
                                    Buffer,
                                    BufSize,
                                    &WrittenBufLength);
        if (!NT_SUCCESS(Status))
        {
            DbgPrint("FileWrite() failed (Status 0x%08lx)\n", Status);
            return Status;
        }
        /* Add the written number of bytes from the first write */
        WrittenBufLength += OldWrittenBufLength;

        /* The log wraps */
        LogFile->Header.Flags |= ELF_LOGFILE_HEADER_WRAP;
    }

    if (WrittenLength)
        *WrittenLength = WrittenBufLength;

    /* We return the offset just after the end of the written buffer */
    if (NextOffset)
        NextOffset->QuadPart = FileOffset.QuadPart + BufSize;

    return Status;
}

static BOOL
ElfpAddOffsetInformation(
    IN PEVTLOGFILE LogFile,
    IN ULONG ulNumber,
    IN ULONG ulOffset)
{
    PVOID NewOffsetInfo;

    if (LogFile->OffsetInfoNext == LogFile->OffsetInfoSize)
    {
        /* Allocate a new offset table */
        NewOffsetInfo = LogFile->Allocate((LogFile->OffsetInfoSize + OFFSET_INFO_INCREMENT) *
                                              sizeof(EVENT_OFFSET_INFO),
                                          HEAP_ZERO_MEMORY,
                                          TAG_ELF);
        if (!NewOffsetInfo)
        {
            DbgPrint("Cannot reallocate heap.\n");
            return FALSE;
        }

        /* Free the old offset table and use the new one */
        if (LogFile->OffsetInfo)
        {
            /* Copy the handles from the old table to the new one */
            RtlCopyMemory(NewOffsetInfo,
                          LogFile->OffsetInfo,
                          LogFile->OffsetInfoSize * sizeof(EVENT_OFFSET_INFO));
            LogFile->Free(LogFile->OffsetInfo, 0, TAG_ELF);
        }
        LogFile->OffsetInfo = (PEVENT_OFFSET_INFO)NewOffsetInfo;
        LogFile->OffsetInfoSize += OFFSET_INFO_INCREMENT;
    }

    LogFile->OffsetInfo[LogFile->OffsetInfoNext].EventNumber = ulNumber;
    LogFile->OffsetInfo[LogFile->OffsetInfoNext].EventOffset = ulOffset;
    LogFile->OffsetInfoNext++;

    return TRUE;
}

NTSTATUS
NTAPI
ElfFlushFile(
    IN PEVTLOGFILE LogFile)
{
    NTSTATUS Status;
    LARGE_INTEGER FileOffset;
    SIZE_T WrittenLength;

    ASSERT(LogFile);

    if (LogFile->ReadOnly)
        return STATUS_SUCCESS; // STATUS_ACCESS_DENIED;

    /*
     * NOTE that both the EOF record *AND* the log file header
     * are supposed to be already updated!
     * We just remove the dirty log bit.
     */
    LogFile->Header.Flags &= ~ELF_LOGFILE_HEADER_DIRTY;

    /* Update the log file header */
    FileOffset.QuadPart = 0LL;
    Status = LogFile->FileWrite(LogFile,
                                &FileOffset,
                                &LogFile->Header,
                                sizeof(EVENTLOGHEADER),
                                &WrittenLength);
    if (!NT_SUCCESS(Status))
    {
        DbgPrint("FileWrite() failed (Status 0x%08lx)\n", Status);
        return Status;
    }

    /* Flush the log file */
    Status = LogFile->FileFlush(LogFile, NULL, 0);
    if (!NT_SUCCESS(Status))
    {
        DbgPrint("FileFlush() failed (Status 0x%08lx)\n", Status);
        return Status;
    }

    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
ElfWriteRecord(
    IN PEVTLOGFILE LogFile,
    IN PEVENTLOGRECORD Record,
    IN SIZE_T BufSize)
{
    NTSTATUS Status;
    LARGE_INTEGER FileOffset, NextOffset;
    SIZE_T ReadLength, WrittenLength;
    EVENTLOGEOF EofRec;
    EVENTLOGRECORD RecBuf;
    ULONG FreeSpace = 0;
    ULONG UpperBound;
    ULONG RecOffset, WriteOffset;

    ASSERT(LogFile);

    if (LogFile->ReadOnly)
        return STATUS_ACCESS_DENIED;

    // ASSERT(sizeof(*Record) == sizeof(RecBuf));

    if (!Record || BufSize < sizeof(*Record))
        return STATUS_INVALID_PARAMETER;

    Record->RecordNumber = LogFile->Header.CurrentRecordNumber;

    /* Compute the available log free space */
    if (LogFile->Header.StartOffset <= LogFile->Header.EndOffset)
        FreeSpace = LogFile->Header.MaxSize - LogFile->Header.EndOffset + LogFile->Header.StartOffset - sizeof(EVENTLOGHEADER);
    else // if (LogFile->Header.StartOffset > LogFile->Header.EndOffset)
        FreeSpace = LogFile->Header.StartOffset - LogFile->Header.EndOffset;

    LogFile->Header.Flags |= ELF_LOGFILE_HEADER_DIRTY;

    /* If the event log was empty, it will now contain one record */
    if (LogFile->Header.OldestRecordNumber == 0)
        LogFile->Header.OldestRecordNumber = 1;

    /* By default we append the new record at the old EOF record offset */
    WriteOffset = LogFile->Header.EndOffset;

    /*
     * Check whether the log is going to wrap (the events being overwritten).
     */

    if (LogFile->Header.StartOffset <= LogFile->Header.EndOffset)
        UpperBound = LogFile->Header.MaxSize;
    else // if (LogFile->Header.StartOffset > LogFile->Header.EndOffset)
        UpperBound = LogFile->Header.StartOffset;

    // if (LogFile->Header.MaxSize - WriteOffset < BufSize + sizeof(EofRec))
    if (UpperBound - WriteOffset < BufSize + sizeof(EofRec))
    {
        DbgPrint("The event log file has reached maximum size (0x%x), wrapping...\n"
               "UpperBound = 0x%x, WriteOffset = 0x%x, BufSize = 0x%x\n",
               LogFile->Header.MaxSize, UpperBound, WriteOffset, BufSize);
        /* This will be done later */
    }

    if ( (LogFile->Header.StartOffset < LogFile->Header.EndOffset) &&
         (LogFile->Header.MaxSize - WriteOffset < sizeof(RecBuf)) ) // (UpperBound - WriteOffset < sizeof(RecBuf))
    {
        // ASSERT(UpperBound  == LogFile->Header.MaxSize);
        // ASSERT(WriteOffset == LogFile->Header.EndOffset);

        /*
         * We cannot fit the EVENTLOGRECORD header of the buffer before
         * the end of the file. We need to pad the end of the log with
         * 0x00000027, normally we will need to pad at most 0x37 bytes
         * (corresponding to sizeof(EVENTLOGRECORD) - 1).
         */

        /* Rewind to the beginning of the log, just after the header */
        WriteOffset = sizeof(EVENTLOGHEADER);
        /**/UpperBound = LogFile->Header.StartOffset;/**/

        FreeSpace = LogFile->Header.StartOffset - WriteOffset;

        LogFile->Header.Flags |= ELF_LOGFILE_HEADER_WRAP;
    }
    /*
     * Otherwise, we can fit the header and only part
     * of the data will overwrite the oldest records.
     *
     * It might be possible that all the event record can fit in one piece,
     * but that the EOF record needs to be split. This is not a problem,
     * EVENTLOGEOF can be splitted while EVENTLOGRECORD cannot be.
     */

    if (UpperBound - WriteOffset < BufSize + sizeof(EofRec))
    {
        ULONG OrgOldestRecordNumber, OldestRecordNumber;

        // DPRINT("EventLogFile has reached maximum size, wrapping...\n");

        OldestRecordNumber = OrgOldestRecordNumber = LogFile->Header.OldestRecordNumber;

        // FIXME: Assert whether LogFile->Header.StartOffset is the beginning of a record???
        // NOTE: It should be, by construction (and this should have been checked when
        // initializing a new, or existing log).

        /*
         * Determine how many old records need to be overwritten.
         * Check the size of the record as the record added may be larger.
         * Need to take into account that we append the EOF record.
         */
        while (FreeSpace < BufSize + sizeof(EofRec))
        {
            /* Get the oldest record data */
            RecOffset = ElfpOffsetByNumber(LogFile, OldestRecordNumber);
            if (RecOffset == 0)
            {
                DbgPrint("Record number %d cannot be found, or log file is full and cannot wrap!\n", OldestRecordNumber);
                LogFile->Header.Flags |= ELF_LOGFILE_LOGFULL_WRITTEN;
                return STATUS_LOG_FILE_FULL;
            }

            RtlZeroMemory(&RecBuf, sizeof(RecBuf));

            FileOffset.QuadPart = RecOffset;
            Status = LogFile->FileRead(LogFile,
                                       &FileOffset,
                                       &RecBuf,
                                       sizeof(RecBuf),
                                       &ReadLength);
            if (!NT_SUCCESS(Status))
            {
                DbgPrint("FileRead() failed (Status 0x%08lx)\n", Status);
                // Status = STATUS_EVENTLOG_FILE_CORRUPT;
                return Status;
            }

            if (RecBuf.Reserved != LOGFILE_SIGNATURE)
            {
                DbgPrint("The event log file is corrupted!\n");
                return STATUS_EVENTLOG_FILE_CORRUPT;
            }

            /*
             * Check whether this event can be overwritten by comparing its
             * written timestamp with the log's retention value. This value
             * is the time interval, in seconds, that events records are
             * protected from being overwritten.
             *
             * If the retention value is zero the events are always overwritten.
             *
             * If the retention value is non-zero, when the age of an event,
             * in seconds, reaches or exceeds this value, it can be overwritten.
             * Also if the events are in the future, we do not overwrite them.
             */
            if (LogFile->Header.Retention != 0 &&
                (Record->TimeWritten <  RecBuf.TimeWritten ||
                (Record->TimeWritten >= RecBuf.TimeWritten &&
                 Record->TimeWritten -  RecBuf.TimeWritten < LogFile->Header.Retention)))
            {
                DbgPrint("The event log file is full and cannot wrap because of the retention policy.\n");
                LogFile->Header.Flags |= ELF_LOGFILE_LOGFULL_WRITTEN;
                return STATUS_LOG_FILE_FULL;
            }

            /*
             * Advance the oldest record number, add the event record length
             * (as long as it is valid...) then take account for the possible
             * paddind after the record, in case this is the last one at the
             * end of the file.
             */
            OldestRecordNumber++;
            RecOffset += RecBuf.Length;
            FreeSpace += RecBuf.Length;

            /*
             * If this was the last event record before the end of the log file,
             * the next one should start at the beginning of the log and the space
             * between the last event record and the end of the file is padded.
             */
            if (LogFile->Header.MaxSize - RecOffset < sizeof(EVENTLOGRECORD))
            {
                /* Add the padding size */
                FreeSpace += LogFile->Header.MaxSize - RecOffset;
            }
        }

        DbgPrint("Record will fit. FreeSpace %d, BufSize %d\n", FreeSpace, BufSize);

        /* The log records are wrapping */
        LogFile->Header.Flags |= ELF_LOGFILE_HEADER_WRAP;


        // FIXME: May lead to corruption if the other subsequent calls fail...

        /*
         * We have validated all the region of events to be discarded,
         * now we can perform their deletion.
         */
        ElfpDeleteOffsetInformation(LogFile, OrgOldestRecordNumber, OldestRecordNumber - 1);
        LogFile->Header.OldestRecordNumber = OldestRecordNumber;
        LogFile->Header.StartOffset = ElfpOffsetByNumber(LogFile, OldestRecordNumber);
        if (LogFile->Header.StartOffset == 0)
        {
            /*
             * We have deleted all the existing event records to make place
             * for the new one. We can put it at the start of the event log.
             */
            LogFile->Header.StartOffset = sizeof(EVENTLOGHEADER);
            WriteOffset = LogFile->Header.StartOffset;
            LogFile->Header.EndOffset = WriteOffset;
        }

        DbgPrint("MaxSize = 0x%x, StartOffset = 0x%x, WriteOffset = 0x%x, EndOffset = 0x%x, BufSize = 0x%x\n"
                  "OldestRecordNumber = %d\n",
                  LogFile->Header.MaxSize, LogFile->Header.StartOffset, WriteOffset, LogFile->Header.EndOffset, BufSize,
                  OldestRecordNumber);
    }

    /*
     * Expand the log file if needed.
     * NOTE: It may be needed to perform this task a bit sooner if we need
     * such a thing for performing read operations, in the future...
     * Or if this operation needs to modify 'FreeSpace'...
     */
    if (LogFile->CurrentSize < LogFile->Header.MaxSize)
    {
        DbgPrint("Expanding the log file from %lu to %lu\n",
                LogFile->CurrentSize, LogFile->Header.MaxSize);

        LogFile->CurrentSize = LogFile->Header.MaxSize;
        LogFile->FileSetSize(LogFile, LogFile->CurrentSize, 0);
    }

    /* Since we can write events in the log, clear the log full flag */
    LogFile->Header.Flags &= ~ELF_LOGFILE_LOGFULL_WRITTEN;

    /* Pad the end of the log */
    // if (LogFile->Header.EndOffset + sizeof(RecBuf) > LogFile->Header.MaxSize)
    if (WriteOffset < LogFile->Header.EndOffset)
    {
        /* Pad all the space from LogFile->Header.EndOffset to LogFile->Header.MaxSize */
        WrittenLength = ROUND_DOWN(LogFile->Header.MaxSize - LogFile->Header.EndOffset, sizeof(ULONG));
        RtlFillMemoryUlong(&RecBuf, WrittenLength, 0x00000027);

        FileOffset.QuadPart = LogFile->Header.EndOffset;
        Status = LogFile->FileWrite(LogFile,
                                    &FileOffset,
                                    &RecBuf,
                                    WrittenLength,
                                    &WrittenLength);
        if (!NT_SUCCESS(Status))
        {
            DbgPrint("FileWrite() failed (Status 0x%08lx)\n", Status);
            // return Status;
        }
    }

    /* Write the event record buffer with possible wrap at offset sizeof(EVENTLOGHEADER) */
    FileOffset.QuadPart = WriteOffset;
    Status = WriteLogBuffer(LogFile,
                            Record,
                            BufSize,
                            &WrittenLength,
                            &FileOffset,
                            &NextOffset);
    if (!NT_SUCCESS(Status))
    {
        DbgPrint("WriteLogBuffer failed (Status 0x%08lx)\n", Status);
        return Status;
    }
    /* FileOffset now contains the offset just after the end of the record buffer */
    FileOffset = NextOffset;

    if (!ElfpAddOffsetInformation(LogFile,
                                  Record->RecordNumber,
                                  WriteOffset))
    {
        return STATUS_NO_MEMORY; // STATUS_EVENTLOG_FILE_CORRUPT;
    }

    LogFile->Header.CurrentRecordNumber++;
    if (LogFile->Header.CurrentRecordNumber == 0)
        LogFile->Header.CurrentRecordNumber = 1;

    /*
     * Write the new EOF record offset just after the event record.
     * The EOF record can wrap (be splitted) if less than sizeof(EVENTLOGEOF)
     * bytes remains between the end of the record and the end of the log file.
     */
    LogFile->Header.EndOffset = FileOffset.QuadPart;

    RtlCopyMemory(&EofRec, &EOFRecord, sizeof(EOFRecord));
    EofRec.BeginRecord = LogFile->Header.StartOffset;
    EofRec.EndRecord   = LogFile->Header.EndOffset;
    EofRec.CurrentRecordNumber = LogFile->Header.CurrentRecordNumber;
    EofRec.OldestRecordNumber  = LogFile->Header.OldestRecordNumber;

    // FileOffset.QuadPart = LogFile->Header.EndOffset;
    Status = WriteLogBuffer(LogFile,
                            &EofRec,
                            sizeof(EofRec),
                            &WrittenLength,
                            &FileOffset,
                            &NextOffset);
    if (!NT_SUCCESS(Status))
    {
        DbgPrint("WriteLogBuffer failed (Status 0x%08lx)\n", Status);
        return Status;
    }
    FileOffset = NextOffset;

    /* Flush the log file */
    Status = ElfFlushFile(LogFile);
    if (!NT_SUCCESS(Status))
    {
        DbgPrint("ElfFlushFile() failed (Status 0x%08lx)\n", Status);
        return STATUS_EVENTLOG_FILE_CORRUPT; // Status;
    }

    return Status;
}

NTSTATUS
LogfWriteRecord(PLOGFILE LogFile,
                PEVENTLOGRECORD Record,
                SIZE_T BufSize)
{
    NTSTATUS Status;
    LARGE_INTEGER SystemTime;

    // ASSERT(sizeof(*Record) == sizeof(RecBuf));

    if (!Record || BufSize < sizeof(*Record))
        return STATUS_INVALID_PARAMETER;

    /* Lock the log file exclusive */
    RtlAcquireResourceExclusive(&LogFile->Lock, TRUE);

    /*
     * Retrieve the record written time now, that will also be compared
     * with the existing events timestamps in case the log is wrapping.
     */
    NtQuerySystemTime(&SystemTime);
    RtlTimeToSecondsSince1970(&SystemTime, &Record->TimeWritten);

    Status = ElfWriteRecord(&LogFile->LogFile, Record, BufSize);
    if (Status == STATUS_LOG_FILE_FULL)
    {
        /* The event log file is full, queue a message box for the user and exit */
        // TODO!
        DbgPrint("Log file `%S' is full!\n", LogFile->LogName);
    }

    /* Unlock the log file */
    RtlReleaseResource(&LogFile->Lock);

    return Status;
}
	 
/* Helper function for ElfrReportEventW/A and ElfrReportEventAndSourceW */
NTSTATUS
ElfrIntReportEventW(
    IELF_HANDLE LogHandle,
    ULONG Time,
    USHORT EventType,
    USHORT EventCategory,
    ULONG EventID,
    PRPC_UNICODE_STRING SourceName OPTIONAL,
    USHORT NumStrings,
    ULONG DataSize,
    PRPC_UNICODE_STRING ComputerName,
    PRPC_SID UserSID,
    PRPC_UNICODE_STRING Strings[],
    PBYTE Data,
    USHORT Flags,
    PULONG RecordNumber,
    PULONG TimeWritten)
{
    NTSTATUS Status;
    PLOGHANDLE pLogHandle;
    UNICODE_STRING LocalSourceName, LocalComputerName;
    PEVENTLOGRECORD LogBuffer;
    USHORT i;
    SIZE_T RecSize;
    ULONG dwStringsSize = 0;
    ULONG dwUserSidLength = 0;
    PWSTR lpStrings, str;

    pLogHandle = ElfGetLogHandleEntryByHandle(LogHandle);
    if (!pLogHandle)
        return STATUS_INVALID_HANDLE;

    /* Flags must be 0 */
    if (Flags)
        return STATUS_INVALID_PARAMETER;

    for (i = 0; i < NumStrings; i++)
    {
        switch (EventType)
        {
            case EVENTLOG_SUCCESS:
                DbgPrint("Success: %wZ\n", Strings[i]);
                break;

            case EVENTLOG_ERROR_TYPE:
                DbgPrint("Error: %wZ\n", Strings[i]);
                break;

            case EVENTLOG_WARNING_TYPE:
                DbgPrint("Warning: %wZ\n", Strings[i]);
                break;

            case EVENTLOG_INFORMATION_TYPE:
                DbgPrint("Info: %wZ\n", Strings[i]);
                break;

            case EVENTLOG_AUDIT_SUCCESS:
                DbgPrint("Audit Success: %wZ\n", Strings[i]);
                break;

            case EVENTLOG_AUDIT_FAILURE:
                DbgPrint("Audit Failure: %wZ\n", Strings[i]);
                break;

            default:
                DbgPrint("Type %hu: %wZ\n", EventType, Strings[i]);
                break;
        }
        dwStringsSize += Strings[i]->Length + sizeof(UNICODE_NULL);
    }

    lpStrings = HeapAlloc(GetProcessHeap(), 0, dwStringsSize);
    if (!lpStrings)
    {
        DbgPrint("Failed to allocate heap\n");
        return STATUS_NO_MEMORY;
    }

    str = lpStrings;
    for (i = 0; i < NumStrings; i++)
    {
        RtlCopyMemory(str, Strings[i]->Buffer, Strings[i]->Length);
        str += Strings[i]->Length / sizeof(WCHAR);
        *str = UNICODE_NULL;
        str++;
    }

    if (UserSID)
        dwUserSidLength = FIELD_OFFSET(SID, SubAuthority[UserSID->SubAuthorityCount]);

    if (SourceName && SourceName->Buffer)
        LocalSourceName = *(PUNICODE_STRING)SourceName;
    else
        RtlInitUnicodeString(&LocalSourceName, pLogHandle->szName);

    LocalComputerName = *(PUNICODE_STRING)ComputerName;

    LogBuffer = LogfAllocAndBuildNewRecord(&RecSize,
                                           Time,
                                           EventType,
                                           EventCategory,
                                           EventID,
                                           &LocalSourceName,
                                           &LocalComputerName,
                                           dwUserSidLength,
                                           UserSID,
                                           NumStrings,
                                           lpStrings,
                                           DataSize,
                                           Data);
    if (LogBuffer == NULL)
    {
        DbgPrint("LogfAllocAndBuildNewRecord failed!\n");
        HeapFree(GetProcessHeap(), 0, lpStrings);
        return STATUS_NO_MEMORY;
    }

    Status = LogfWriteRecord(pLogHandle->LogFile, LogBuffer, RecSize);
    if (!NT_SUCCESS(Status))
    {
        DbgPrint("ERROR writing to event log `%S' (Status 0x%08lx)\n",
                pLogHandle->LogFile->LogName, Status);
    }

    if (NT_SUCCESS(Status))
    {
        /* Retrieve the two fields that were set by LogfWriteRecord into the record */
        if (RecordNumber)
            *RecordNumber = LogBuffer->RecordNumber;
        if (TimeWritten)
            *TimeWritten = LogBuffer->TimeWritten;
    }

    LogfFreeRecord(LogBuffer);

    HeapFree(GetProcessHeap(), 0, lpStrings);

    return Status;
}


/* Function 24 */
NTSTATUS
WINAPI
ElfrReportEventAndSourceW(
    IELF_HANDLE LogHandle,
    ULONG Time,
    USHORT EventType,
    USHORT EventCategory,
    ULONG EventID,
    PRPC_UNICODE_STRING SourceName,
    USHORT NumStrings,
    ULONG DataSize,
    PRPC_UNICODE_STRING ComputerName,
    PRPC_SID UserSID,
    PRPC_UNICODE_STRING Strings[],
    PBYTE Data,
    USHORT Flags,
    PULONG RecordNumber,
    PULONG TimeWritten)
{
    /* Call the helper function. The event source is specified by the caller. */
    return ElfrIntReportEventW(LogHandle,
                               Time,
                               EventType,
                               EventCategory,
                               EventID,
                               SourceName,
                               NumStrings,
                               DataSize,
                               ComputerName,
                               UserSID,
                               Strings,
                               Data,
                               Flags,
                               RecordNumber,
                               TimeWritten);
}
	
/*
 * @implemented
 */
NTSTATUS
NTAPI
ElfReportEventAndSourceW(
	IN HANDLE hEventLog,
    IN ULONG Time,
    IN PUNICODE_STRING ComputerName,
    IN USHORT EventType,
    IN USHORT EventCategory,
    IN ULONG EventID,
    IN PSID UserSID,
    IN PUNICODE_STRING SourceName,
    IN USHORT NumStrings,
    IN ULONG DataSize,
    IN PUNICODE_STRING* Strings,
    IN PVOID Data,
    IN USHORT Flags,
    IN OUT PULONG RecordNumber,
    IN OUT PULONG TimeWritten)
{
	ELF_REPORT_EVENT_AND_SOURCE elfReportEventAndSource;
	NTSTATUS Status;
	
    elfReportEventAndSource = (ELF_REPORT_EVENT_AND_SOURCE) GetProcAddress(
                            GetModuleHandle(TEXT("advapibase")),
                            "ElfReportEventAndSourceW");
    if (NULL == elfReportEventAndSource) 
    {	

		RpcTryExcept
		{
			Status = ElfrReportEventAndSourceW(hEventLog,
											   Time,
											   EventType,
											   EventCategory,
											   EventID,
											   (PRPC_UNICODE_STRING)SourceName,
											   NumStrings,
											   DataSize,
											   (PRPC_UNICODE_STRING)ComputerName,
											   (PRPC_SID)UserSID,
											   (PRPC_UNICODE_STRING*)Strings,
											   (PBYTE)Data,
											   Flags,
											   RecordNumber,
											   TimeWritten);
		}
		RpcExcept(EXCEPTION_EXECUTE_HANDLER)
		{
			Status = I_RpcMapWin32Status(RpcExceptionCode());
		}
		RpcEndExcept;

		return Status;

    }else{
		return (NTSTATUS)elfReportEventAndSource(hEventLog, 
											Time,
											ComputerName,
											EventType,
											EventCategory,
											EventID,
											UserSID,
											SourceName,
											NumStrings,
											DataSize,
											Strings,
											Data,
											Flags,
											RecordNumber,
											TimeWritten);
	}
}	

void
ElfReportInitialize(VOID)
{
    InitializeCriticalSection(&LogHandleListCs);
    InitializeListHead(&LogHandleListHead);;
}