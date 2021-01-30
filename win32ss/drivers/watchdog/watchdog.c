
#include <ntifs.h>

#define NDEBUG
#include <debug.h>

NTSTATUS
NTAPI
DriverEntry (
    _In_ PDRIVER_OBJECT	DriverObject,
    _In_ PUNICODE_STRING RegistryPath)
{
    UNIMPLEMENTED;
    return STATUS_SUCCESS;
}

ULONG DMgrGetDisplayOwnership(PULONG a1)
{
	*a1 = 2;
	return 0;
}