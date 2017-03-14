/* MAIN INCLUDES **********************************************************/

#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <winbase.h>

/* PSEH for SEH Support */
#include <pseh/pseh2.h>

//#include <obfuncs.h>
#include <psfuncs.h>
#include <rtlfuncs.h>
#include <csr/csr.h>
// #include <kefuncs.h>
// #include <exfuncs.h>
// #include <strsafe.h>
// #include <iofuncs.h>
// #include <mmfuncs.h>

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