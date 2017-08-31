#include <ntddk.h>

VOID 
NTAPI 
IoDisconnectInterruptEx(
  PIO_DISCONNECT_INTERRUPT_PARAMETERS Parameters
)
{
	IoDisconnectInterrupt(Parameters->ConnectionContext.InterruptObject);	
}

NTSTATUS 
NTAPI 
IoConnectInterruptEx(
  PIO_CONNECT_INTERRUPT_PARAMETERS Parameters
)
{
  BOOLEAN (__stdcall *function)(struct _KINTERRUPT *, PVOID); // edi@3
  KIRQL otherIRQ; // al@4
  KIRQL localIRQ; // cl@4
  NTSTATUS result; // eax@5

  if ( Parameters->Version == 1 )
  {
    if ( Parameters->FullySpecified.PhysicalDeviceObject
      && (function = Parameters->FullySpecified.ServiceRoutine) != 0
      && (otherIRQ = Parameters->FullySpecified.SynchronizeIrql,
          localIRQ = Parameters->FullySpecified.Irql,
          otherIRQ >= localIRQ) )
      result = IoConnectInterrupt(
                 Parameters->FullySpecified.InterruptObject,
                 function,
                 Parameters->FullySpecified.ServiceContext,
                 Parameters->FullySpecified.SpinLock,
                 Parameters->FullySpecified.Vector,
                 localIRQ,
                 otherIRQ,
                 Parameters->FullySpecified.InterruptMode,
                 Parameters->FullySpecified.ShareVector,
                 Parameters->FullySpecified.ProcessorEnableMask,
                 Parameters->FullySpecified.FloatingSave);
    else
      result = 0xC000000Du;
  }
  return result;
}

NTSTATUS 
NTAPI
IoGetDeviceNumaNode(
  _In_  PDEVICE_OBJECT Pdo,
  _Out_ PUSHORT        NodeNumber
)
{
	//Maybe need implementation
	*NodeNumber = 1;
}