/*
 * PROJECT:         ReactOS system libraries
 * LICENSE:         GPLv2+ - See COPYING in the top level directory
 * PURPOSE:         debug functions
 * PROGRAMMER:      Rama Teja Gampa <ramateja.g@gmail.com>
*/
#include "usbxhci.h"

#define NDEBUG
#include <debug.h>
#include <ntstrsafe.h>

// VOID
// NTAPI
// XHCI_DumpHwTD(IN PXHCI_HCD_TD TD)
// {
    // if (!TD)
    // {
        // return;
    // }

    // //DPRINT(": TD                       - %p\n", TD);
    // //DPRINT(": TD->PhysicalAddress      - %p\n", TD->PhysicalAddress);
    // //DPRINT(": TD->HwTD.NextTD          - %p\n", TD->HwTD.NextTD);
    // //DPRINT(": TD->HwTD.AlternateNextTD - %p\n", TD->HwTD.AlternateNextTD);
    // //DPRINT(": TD->HwTD.Token.AsULONG   - %p\n", TD->HwTD.Token.AsULONG);
// }

// VOID
// NTAPI
// XHCI_DumpHwQH(IN PXHCI_HCD_QH QH)
// {
    // if (!QH)
    // {
        // return;
    // }

    // //DPRINT(": QH->sqh.HwQH.CurrentTD       - %p\n", QH->sqh.HwQH.CurrentTD);
    // //DPRINT(": QH->sqh.HwQH.NextTD          - %p\n", QH->sqh.HwQH.NextTD);
    // //DPRINT(": QH->sqh.HwQH.AlternateNextTD - %p\n", QH->sqh.HwQH.AlternateNextTD);
    // //DPRINT(": QH->sqh.HwQH.Token.AsULONG   - %p\n", QH->sqh.HwQH.Token.AsULONG);
// }
#define BUFFER_SIZE 1024

// VOID 
// //XHCI_LOG(char* text) {

    // UNICODE_STRING     uniName;
    // OBJECT_ATTRIBUTES  objAttr;
    // HANDLE   handle;
    // NTSTATUS ntstatus;
    // IO_STATUS_BLOCK    ioStatusBlock;	
    // CHAR     buffer[BUFFER_SIZE];
    // size_t  cb;	

    // RtlInitUnicodeString(&uniName, L"\\SystemRoot\\xhci.txt");  // or L"\\SystemRoot\\example.txt"
    // InitializeObjectAttributes(&objAttr, &uniName,
        // OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
        // NULL, NULL);



    // // Do not try to perform any file operations at higher IRQL levels.
    // // Instead, you may use a work item or a system worker thread to perform file operations.

    // if (KeGetCurrentIrql() != PASSIVE_LEVEL) return;
        // //return STATUS_INVALID_DEVICE_STATE;

    // ntstatus = ZwCreateFile(&handle,
        // GENERIC_WRITE,
        // &objAttr, &ioStatusBlock, NULL,
        // FILE_ATTRIBUTE_NORMAL,
        // 0,
        // FILE_OPEN_IF,
        // FILE_SYNCHRONOUS_IO_NONALERT,
        // NULL, 0);

    // if (NT_SUCCESS(ntstatus)) {
        // ntstatus = RtlStringCbPrintfA(buffer, sizeof(buffer), text, 0x0);
        // if (NT_SUCCESS(ntstatus)) {
            // ntstatus = RtlStringCbLengthA(buffer, sizeof(buffer), &cb);
            // if (NT_SUCCESS(ntstatus)) {
                // ntstatus = ZwWriteFile(handle, NULL, NULL, NULL, &ioStatusBlock,
                    // buffer, (ULONG)cb, NULL, NULL);
            // }
        // }
        // ZwClose(handle);
    // }
// }