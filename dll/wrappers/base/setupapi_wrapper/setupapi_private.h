#include <stdarg.h>
#include <stdio.h>

#include "windef.h"
#include "winbase.h"
#include "winnt.h"
#include "winreg.h"
#include "wingdi.h"
#include "winuser.h"
#include "winnls.h"
#include "winsvc.h"
#include "setupapi.h"
#include "wine/debug.h"
#include "wine/heap.h"
#include "wine/list.h"
#include "cfgmgr32.h"
#include "winioctl.h"
#include "rpc.h"
#include "rpcdce.h"
#include "cguid.h"
#include "devpropdef.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static inline void * __WINE_ALLOC_SIZE(2) heap_realloc_zero(void *mem, size_t len)
{
    return HeapReAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, mem, len);
}