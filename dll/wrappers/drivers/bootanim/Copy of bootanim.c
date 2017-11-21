/*
 * PROJECT:         ReactOS WMI driver
 * COPYRIGHT:       GPL - See COPYING in the top level directory
 * FILE:            drivers/bootanim/main.c
 * PURPOSE:         Windows Management Intstrumentation
 * PROGRAMMERS:     Aleksey Bragin (aleksey@reactos.org)
 *                  
 */

/* INCLUDES *****************************************************************/

#include <stdio.h>
#include <ntddk.h>
#include <wmilib.h>

#define NDEBUG
#include <debug.h>
#include <ntimage.h>
#include <inbvfuncs.h>
#include <bootvid.h>
#include <ldrfuncs.h>
#include <wdm.h>

/* GLOBALS ******************************************************************/

KSPIN_LOCK SpinLock = PASSIVE_LEVEL;

KIRQL NewIrql = PASSIVE_LEVEL;

ULONG ResourceCount = 0;

PIMAGE_RESOURCE_DATA_ENTRY vetorImage[91];

PDRIVER_OBJECT GlobalDriverObject;

PLDR_DATA_TABLE_ENTRY LdrEntry;

KTIMER Timer;

PEPROCESS pProc;


/* FUNCTIONS ****************************************************************/
/*
KIRQL NTAPI AcquiringSpinLock()
{
  KIRQL result; // al@3

  DbgPrint("Bootanim::InbvAcquireLock: Increasing Irql and acquiring SpinLock.\n");
  NewIrql = KeGetCurrentIrql();
  if ( NewIrql < DISPATCH_LEVEL )
    NewIrql = KfRaiseIrql(DISPATCH_LEVEL);
  result = KfAcquireSpinLock(&SpinLock);
  NewIrql = result;
  return result;
}

void NTAPI ReleasingSpinLock()
{
  DbgPrint("Bootanim::InbvReleaseLock: Lowering Irql and releasing SpinLock.\n");
  KfReleaseSpinLock(&SpinLock, NewIrql);
  if ( NewIrql < DISPATCH_LEVEL )
    KfLowerIrql(NewIrql);
}*/

void NTAPI VidBitmap(PIMAGE_RESOURCE_DATA_ENTRY Buffer, ULONG Left, ULONG Top, ULONG count)
{
	  DbgPrint("Bootanim::InbvBitBlt: Entered worker thread.\n");
	  //AcquiringSpinLock();
      VidBitBlt(Buffer, Left, Top);
	  //ReleasingSpinLock();
}

PIMAGE_RESOURCE_DATA_ENTRY NTAPI CountBitmap(ULONG BitmapNumber)
{
  PIMAGE_RESOURCE_DATA_ENTRY result; // eax@2

  DbgPrint("Bootanim::InbvGetResourceAddress: Entered function for resource Address.\n");
  DbgPrint("Bootanim::InbvGetResourceAddress: ResNumber = %d.\n", BitmapNumber);
  DbgPrint("Bootanim::InbvGetResourceAddress: ResCount = %d.\n", ResourceCount);
  if ( BitmapNumber <= ResourceCount )
    result = vetorImage[BitmapNumber];
  else
    result = 0;
  return result;
}

void NTAPI StartRoutine()
{
  NTSTATUS statusSize; // esi@1
  PIMAGE_RESOURCE_DATA_ENTRY image; // ecx@2
  ULONG size; // edx@3
  ULONG ReceiveSize; // eax@4
  ULONG variable; // [sp+Ch] [bp-8h]@1
  ULONG count; // [sp+10h] [bp-4h]@1
  LARGE_INTEGER DueTime;
  DueTime.QuadPart = -380000;

  statusSize = 0;
  variable = 0;
  count = 0;
  DbgPrint("Bootanim::BitBltBitmaps: Entered worker thread.\n");
  DbgPrint("Bootanim::DriverEntry: %d = Bitmap Count.\n", 1);
  DbgPrint("Bootanim::Variables: %d = i.\n", 0);
  do
  {
	DbgPrint("Bootanim::DriverEntry: %d = Bitmap Count.\n", count);
    image = CountBitmap(count);
    if ( (ULONG)image != statusSize )
    {
      size =  image->CodePage;
      if ( (size & 0x80000000u) == 0 )
        ReceiveSize = 480 - size;
      else
        ReceiveSize = size + 480;
      VidBitmap(image, (640 - image->Size) / 2, ReceiveSize / 2, count);
      statusSize = 0;
    }    
	KeDelayExecutionThread(KernelMode, TRUE, &DueTime);	
    ++count;
    /*if ( count > 91 )
    {
      ++variable;
      count = 2;
      DbgPrint("Bootanim::Variables: %d = i.\n", variable);
    }
    if ( variable > 7 )
    {
      InbvSolidColorFill(statusSize, statusSize, 639, 439, statusSize);
      PsTerminateSystemThread(statusSize);
    }*/
	if ( count > 91 )
    {
      InbvSolidColorFill(statusSize, statusSize, 639, 439, statusSize);
      PsTerminateSystemThread(statusSize);
    }
  }
  while ( count < 92 );
}

PIMAGE_RESOURCE_DATA_ENTRY NTAPI FindResource(PIMAGE_RESOURCE_DATA_ENTRY DataEntry)
{
  PIMAGE_RESOURCE_DATA_ENTRY Image; // esi@1
  PVOID section; // ecx@1
  NTSTATUS FindStatus; // esi@2
  NTSTATUS status; // eax@3
  PIMAGE_RESOURCE_DATA_ENTRY unsed; // [sp+Ch] [bp-14h]@2
  ULONG DataLength; // [sp+18h] [bp-8h]@3
  PIMAGE_RESOURCE_DATA_ENTRY Data; // [sp+1Ch] [bp-4h]@1
  LDR_RESOURCE_INFO ResourceInfo;

  Data = 0;
  Image = DataEntry;
  DbgPrint("NtEmu: FindBitmapResource: _ULONG ResourceId = %d\n", DataEntry);
  section = GlobalDriverObject->DriverSection;
  ResourceInfo.Type = 2; //RT_BITMAP;
  ResourceInfo.Name = (ULONG_PTR)DataEntry;
  ResourceInfo.Language = 0;    
    DbgPrint("ResourceInfo.Type = %d\nResourceInfo.Name = %d\nResourceInfo.Language = %d\n", 2, Image, 0);
    unsed = 0;
    FindStatus = LdrFindResource_U(GlobalDriverObject->DriverStart, &ResourceInfo, 3, &DataEntry);
    DbgPrint("NtEmu: LdrFindResource_U: status = 0x%x\n", FindStatus);
    if ( FindStatus >= 0 )
    {
      status = LdrAccessResource(GlobalDriverObject->DriverStart, DataEntry, &Data, &DataLength);
      if ( status < 0 )
        Data = 0;
      DbgPrint("NtEmu: LdrAccessResource: status = 0x%x\n", status);
    }
  return Data;
}

NTSTATUS NTAPI Animation(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
  ULONG image; // edi@1
  NTSTATUS status; // eax@3
  OBJECT_ATTRIBUTES ObjectAttributes; // [sp+8h] [bp-18h]@1

  GlobalDriverObject = DriverObject;
  ObjectAttributes.Length = 24;
  ObjectAttributes.RootDirectory = 0;
  ObjectAttributes.Attributes = 0;
  ObjectAttributes.ObjectName = 0;
  ObjectAttributes.SecurityDescriptor = 0;
  ObjectAttributes.SecurityQualityOfService = 0;
  DbgPrint("Bootanim::DriverEntry: Driver Entered.\n");
  InbvAcquireDisplayOwnership();
  InbvResetDisplay();
  InbvSolidColorFill(0, 0, 639, 439, 0);
  InbvSetTextColor(15);
  InbvInstallDisplayStringFilter(0);
  InbvEnableDisplayString(1);
  InbvSetScrollRegion(0, 0, 635, 475);
  ResourceCount = 91;
  image = 0;
  do
  {
    vetorImage[(int)image] = FindResource((PIMAGE_RESOURCE_DATA_ENTRY)image);
    ++image;
  }
  while ( (int)image < 91 );
  DbgPrint("Bootanim::DriverEntry: Creating system thread.\n");
  status = PsCreateSystemThread(&DriverObject, 0x1F03FFu, &ObjectAttributes, 0, 0, (PKSTART_ROUTINE)StartRoutine, 0);
  if ( status )
    DbgPrint("Bootanim::DriverEntry: PsCreateSystemThread failed with status %x\n", status);
  ZwClose(DriverObject);
  return 0;
}

NTSTATUS
NTAPI
DriverEntry(IN PDRIVER_OBJECT DriverObject,
            IN PUNICODE_STRING RegistryPath)
{
    

    /* Return success */
    return Animation(DriverObject, RegistryPath);
}