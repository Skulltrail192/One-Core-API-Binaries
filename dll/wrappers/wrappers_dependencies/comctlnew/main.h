#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <winbase.h>
#include <ntsecapi.h>
#include <winuser.h>
#include <commctrl.h>

typedef struct _DSA
{
    INT  nItemCount;
    LPVOID pData;
    INT  nMaxCount;
    INT  nItemSize;
    INT  nGrow;
}DSA;

typedef struct _DPA
{
    INT    nItemCount;
    LPVOID   *ptrs;
    HANDLE hHeap;
    INT    nGrow;
    INT    nMaxCount;
} DPA;
