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

#include <csr/csr.h>
// #include <kefuncs.h>
// #include <exfuncs.h>
// #include <strsafe.h>
// #include <iofuncs.h>
// #include <mmfuncs.h>

/* TYPE DEFINITIONS **********************************************************/
typedef UINT(WINAPI * PPROCESS_START_ROUTINE)(VOID);