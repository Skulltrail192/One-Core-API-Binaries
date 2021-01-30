
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

VOID
NTAPI
WdAllocateWatchdog(
    PVOID p1,
    PVOID p2,
    ULONG p3)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdAllocateDeferredWatchdog(
    PVOID p1,
    PVOID p2,
    ULONG p3)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdFreeWatchdog(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdFreeDeferredWatchdog(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdStartWatch(
    PVOID p1,
    LARGE_INTEGER p2,
    ULONG p3)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdStartDeferredWatch(
    PVOID p1,
    PVOID p2,
    ULONG p3)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdStopWatch(
    PVOID p1,
    ULONG p2)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdStopDeferredWatch(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdSuspendWatch(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
FASTCALL
WdSuspendDeferredWatch(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdResumeWatch(
    PVOID p1,
    PVOID p2)
{
    UNIMPLEMENTED;
}

VOID
FASTCALL
WdResumeDeferredWatch(
    PVOID p1,
    PVOID p2)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdResetWatch(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
FASTCALL
WdResetDeferredWatch(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
FASTCALL
WdEnterMonitoredSection(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
FASTCALL
WdExitMonitoredSection(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdAttachContext(
    PVOID p1,
    PVOID p2)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdDetachContext(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdGetDeviceObject(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdGetLowestDeviceObject(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdGetLastEvent(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdCompleteEvent(
    PVOID p1,
    PVOID p2)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdReferenceObject(
    PVOID p1)
{
    UNIMPLEMENTED;
}

VOID
NTAPI
WdDereferenceObject(
    PVOID p1)
{
    UNIMPLEMENTED;
}

BOOLEAN
NTAPI
WdMadeAnyProgress(
    PVOID p1)
{
    UNIMPLEMENTED;
    return FALSE;
}

NTSTATUS 
GTF_FloatToInteger(double *a1, ULONG *a2)
{
	return STATUS_SUCCESS;
}

NTSTATUS GTF_FloatToRational(double *a1, ULONG *a2, ULONG *a3){
	return STATUS_SUCCESS;
}

NTSTATUS GTF_UsingVerticalRefreshFrequency_Stage1(int a1, signed int a2, signed int a3, int a4, int a5, double *a6)
{
	return STATUS_SUCCESS;
}

signed int SMgrGdiCallout(ULONG *a1, char a2, char a3)
{
	return 0;
}

signed int SMgrRegisterSessionStartCallout(int a1)
{
	return 0;
}

signed int SMgrUnregisterSessionStartCallout(int a1)
{
	return 0;
}

int WdDbgCreateSnapshot(int a1, int a2, size_t MaxCount, int a4, int a5)
{
	return 0;
}

void WdDbgDestroySnapshot(PVOID P)
{
	;
}

KIRQL WdDbgReportCancel(PVOID P)
{
  KIRQL result; // al

  result = KeGetCurrentIrql();
  
  return result;
}

KIRQL WdDbgReportComplete(PVOID P)
{
  KIRQL result; // al

  result = KeGetCurrentIrql();
  
  return result;
}

ULONG WdDbgReportCreate(int a1, int a2, int a3, int a4, int a5, int a6)
{
	return 0;
}

BOOLEAN WdDbgReportQueryInfo(ULONG *a1, ULONG *a2)
{
	return FALSE;
}

ULONG WdDbgReportRecreate(int a1, int a2, int a3, int a4, int a5, int a6, PVOID P)
{
	return 0;
}

BOOLEAN WdDbgReportSecondaryData(int a1, void *Src, size_t MaxCount){
	return FALSE;
}

__int64 WdDiagGetEtwHandle(){
	return 0;
}

int WdDiagInit(int a1)
{
	return 0;
}

BOOLEAN WdDiagIsTracingEnabled(int a1)
{
	return FALSE;
}

int WdDiagNotifyUser(int a1, size_t pcch, __int16 a3, int a4)
{
	return 0;
}

int WdDiagShutdown()
{
	return 0;
}

signed WdInitLogging()
{
	return 0;
}

void WdLogEvent5(int a1, int NewIrql, __int16 Response, int a4, int a5, int a6)
{
	;
}

int WdLogGetRecentEvents(signed int a1, unsigned int a2, int a3, int a4)
{
	return 0;
}

BOOLEAN WdQueryDebugFlag(int a1)
{
	return FALSE;
}

void WdpInterfaceReferenceNop(int a1)
{
  ;
}

//Vista functions
NTSTATUS DMgrAcquireGdiViewId(PULONG *a1)
{
	return STATUS_SUCCESS;
}



NTSTATUS DMgrGetSmbiosInfo(void *Dst, size_t Size)
{
	return STATUS_SUCCESS;
}

BOOLEAN 
DMgrIsSetupRunning(){
	return FALSE;
}

signed __int32 DMgrReleaseDisplayOwnership(int a1)
{
	return 1;
}

LONG DMgrReleaseGdiViewId(int a1, char a2)
{
	return 1;
}

NTSTATUS DMgrWriteDeviceCountToRegistry(){
	return STATUS_SUCCESS;
}

NTSTATUS DMgrAcquireDisplayOwnership(int a1)
{
	return STATUS_SUCCESS;
}
