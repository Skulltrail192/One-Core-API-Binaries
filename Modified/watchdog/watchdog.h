#include <ntifs.h>

#define NDEBUG
#include <debug.h>

VOID
NTAPI
WdCompleteEvent(
    PVOID p1,
    PVOID p2);
	
BOOLEAN
NTAPI
WdMadeAnyProgress(
    PVOID p1);