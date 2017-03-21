/* MAIN INCLUDES **********************************************************/

#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <winbase.h>

/* PSEH for SEH Support */
#include <pseh/pseh2.h>

#include <cmfuncs.h>
#include <psfuncs.h>
#include <rtlfuncs.h>
#include <csr/csr.h>

#include <config.h>
#include <port.h>
#include <ntdllbase.h>
#include <winnls.h>
#include <unicode.h>
#include <base.h>
// #include <kefuncs.h>
// #include <exfuncs.h>
// #include <strsafe.h>
// #include <iofuncs.h>
// #include <mmfuncs.h>

#define FIND_DATA_SIZE 0x4000
#define BASESRV_SERVERDLL_INDEX 1

PBASE_STATIC_SERVER_DATA BaseStaticServerData;

/* TYPE DEFINITIONS **********************************************************/
typedef UINT(WINAPI * PPROCESS_START_ROUTINE)(VOID);

/* STRUCTS DEFINITIONS ******************************************************/
typedef struct _FIBER                                    /* Field offsets:    */
{                                                        /* i386  arm   x64   */
    PVOID FiberData;                                     /* 0x000 0x000 0x000 */
    struct _EXCEPTION_REGISTRATION_RECORD *ExceptionList;/* 0x004 0x004 0x008 */
    PVOID StackBase;                                     /* 0x008 0x008 0x010 */
    PVOID StackLimit;                                    /* 0x00C 0x00C 0x018 */
    PVOID DeallocationStack;                             /* 0x010 0x010 0x020 */
    CONTEXT FiberContext;                                /* 0x014 0x018 0x030 */
#if (NTDDI_VERSION >= NTDDI_LONGHORN)
    PVOID Wx86Tib;                                       /* 0x2E0 0x1b8 0x500 */
    struct _ACTIVATION_CONTEXT_STACK *ActivationContextStackPointer; /* 0x2E4 0x1bc 0x508 */
    PVOID FlsData;                                       /* 0x2E8 0x1c0 0x510 */
    ULONG GuaranteedStackBytes;                          /* 0x2EC 0x1c4 0x518 */
    ULONG TebFlags;                                      /* 0x2F0 0x1c8 0x51C */
#else
    ULONG GuaranteedStackBytes;                          /* 0x2E0         */
    PVOID FlsData;                                       /* 0x2E4         */
    struct _ACTIVATION_CONTEXT_STACK *ActivationContextStackPointer;
#endif
} FIBER, *PFIBER;

typedef enum _FIND_DATA_TYPE
{
    FindFile   = 1,
    FindStream = 2
} FIND_DATA_TYPE;

typedef union _DIR_INFORMATION
{
    PVOID DirInfo;
    PFILE_FULL_DIR_INFORMATION FullDirInfo;
    PFILE_BOTH_DIR_INFORMATION BothDirInfo;
} DIR_INFORMATION;

typedef struct _FIND_FILE_DATA
{
    HANDLE Handle;
    FINDEX_INFO_LEVELS InfoLevel;
    FINDEX_SEARCH_OPS SearchOp;

    /*
     * For handling STATUS_BUFFER_OVERFLOW errors emitted by
     * NtQueryDirectoryFile in the FildNextFile function.
     */
    BOOLEAN HasMoreData;

    /*
     * "Pointer" to the next file info structure in the buffer.
     * The type is defined by the 'InfoLevel' parameter.
     */
    DIR_INFORMATION NextDirInfo;

    BYTE Buffer[FIND_DATA_SIZE];
} FIND_FILE_DATA, *PFIND_FILE_DATA;

typedef struct _FIND_STREAM_DATA
{
    STREAM_INFO_LEVELS InfoLevel;
    PFILE_STREAM_INFORMATION FileStreamInfo;
    PFILE_STREAM_INFORMATION CurrentInfo;
} FIND_STREAM_DATA, *PFIND_STREAM_DATA;

typedef struct _FIND_DATA_HANDLE
{
    FIND_DATA_TYPE Type;
    RTL_CRITICAL_SECTION Lock;

    /*
     * Pointer to the following finding data, located at
     * (this + 1). The type is defined by the 'Type' parameter.
     */
    union
    {
        PFIND_FILE_DATA FindFileData;
        PFIND_STREAM_DATA FindStreamData;
    } u;

} FIND_DATA_HANDLE, *PFIND_DATA_HANDLE;

ULONG
WINAPI
BaseSetLastNTError(
    IN NTSTATUS Status
);