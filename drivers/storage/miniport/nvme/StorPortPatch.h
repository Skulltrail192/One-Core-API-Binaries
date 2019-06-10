
/**
 *******************************************************************************
 ** Copyright (C) 2018 Kai Schtrom                                            **
 **                                                                           **
 ** All rights reserved.                                                      **
 **                                                                           **
 *******************************************************************************
 **                                                                           **
 ** Redistribution and use in source and binary forms, with or without        **
 ** modification, are permitted provided that the following conditions are    **
 ** met:                                                                      **
 **                                                                           **
 **   1. Redistributions of source code must retain the above copyright       **
 **      notice, this list of conditions and the following disclaimer.        **
 **                                                                           **
 **   2. Redistributions in binary form must reproduce the above copyright    **
 **      notice, this list of conditions and the following disclaimer in the  **
 **      documentation and/or other materials provided with the distribution. **
 **                                                                           **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS   **
 ** IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, **
 ** THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR    **
 ** PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR         **
 ** CONTRIBUTORS BE LIABLE FOR ANY DIRECT,INDIRECT, INCIDENTAL, SPECIAL,      **
 ** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,       **
 ** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR        **
 ** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF    **
 ** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING      **
 ** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS        **
 ** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.              **
 **                                                                           **
 ** The views and conclusions contained in the software and documentation     **
 ** are those of the authors and should not be interpreted as representing    **
 ** official policies, either expressed or implied, of Intel Corporation,     **
 ** Integrated Device Technology Inc., or Sandforce Corporation.              **
 **                                                                           **
 *******************************************************************************
**/

/*
 * File: StorPortPatch.h
 */

#ifndef __STOR_PORT_PATCH_H__
#define __STOR_PORT_PATCH_H__


//------------------------------------------------------------------------------
// _StorpGetCurrentProcessorNumber@8
//------------------------------------------------------------------------------
ULONG FORCEINLINE StorPortPatchGetCurrentProcessorNumber(__in PVOID HwDeviceExtension,__out PPROCESSOR_NUMBER ProcNumber)
{
	UNREFERENCED_PARAMETER(HwDeviceExtension);

	if(ProcNumber == NULL)
	{
		return STOR_STATUS_INVALID_PARAMETER;
	}

	KeGetCurrentProcessorNumberEx(ProcNumber);

	return STOR_STATUS_SUCCESS;
}


//------------------------------------------------------------------------------
// _StorpGetHighestNodeNumber@8
//------------------------------------------------------------------------------
ULONG FORCEINLINE StorPortPatchGetHighestNodeNumber(__in PVOID HwDeviceExtension,__out PULONG HighestNode)
{
	UNREFERENCED_PARAMETER(HwDeviceExtension);

	if(HighestNode == NULL)
	{
		return STOR_STATUS_INVALID_PARAMETER;
	}

	// the following function call is not supported in procgrp.h
	// therefore we fake the node number to be zero
	// *HighestNode = (ULONG)KeQueryHighestNodeNumber();
	*HighestNode = 0;

	return STOR_STATUS_SUCCESS;
}


//------------------------------------------------------------------------------
// _StorpGetActiveGroupCount@8
//------------------------------------------------------------------------------
ULONG FORCEINLINE StorPortPatchGetActiveGroupCount(__in PVOID HwDeviceExtension,__out PUSHORT NumberGroups)
{
	UNREFERENCED_PARAMETER(HwDeviceExtension);

	if(NumberGroups == NULL)
	{
		return STOR_STATUS_INVALID_PARAMETER;
	}

	*NumberGroups = (USHORT)KeQueryActiveGroupCount();

	return STOR_STATUS_SUCCESS;
}


//------------------------------------------------------------------------------
// _StorpGetGroupAffinity@12
//------------------------------------------------------------------------------
ULONG FORCEINLINE StorPortPatchGetGroupAffinity(__in PVOID HwDeviceExtension,__in USHORT GroupNumber,__out PKAFFINITY GroupAffinityMask)
{
	USHORT GroupCount;

	UNREFERENCED_PARAMETER(HwDeviceExtension);

	if(GroupAffinityMask == NULL)
	{
		return STOR_STATUS_INVALID_PARAMETER;
	}

	GroupCount = (USHORT)KeQueryActiveGroupCount();
	if(GroupNumber > GroupCount)
	{
		return STOR_STATUS_UNSUCCESSFUL;
	}

	*GroupAffinityMask = KeQueryGroupAffinity(GroupNumber);

	return STOR_STATUS_SUCCESS;
}


//------------------------------------------------------------------------------
// _StorpGetNodeAffinity@12
//------------------------------------------------------------------------------
ULONG FORCEINLINE StorPortPatchGetNodeAffinity(__in PVOID HwDeviceExtension,__in ULONG NodeNumber,__out PGROUP_AFFINITY NodeAffinityMask)
{
	KAFFINITY GroupAffinityMask;

	UNREFERENCED_PARAMETER(HwDeviceExtension);

	if(NodeAffinityMask == NULL || NodeNumber > 0xFFFF)
	{
		return STOR_STATUS_INVALID_PARAMETER;
	}

	// the following function call is not supported in procgrp.h
	// we fake the node number to be zero and the node affinity mask to be the group affinity mask
	// KeQueryNodeActiveAffinity(NodeNumber,NodeAffinityMask,NULL);
	GroupAffinityMask = KeQueryGroupAffinity(0);
	memcpy(&NodeAffinityMask->Mask,&GroupAffinityMask,sizeof(KAFFINITY));
	NodeAffinityMask->Group = 0;
	NodeAffinityMask->Reserved[0] = 0;
	NodeAffinityMask->Reserved[1] = 0;
	NodeAffinityMask->Reserved[2] = 0;

	return STOR_STATUS_SUCCESS;
}


//------------------------------------------------------------------------------
// _StorpAllocateContiguousMemorySpecifyCacheNode@44
//------------------------------------------------------------------------------
__drv_maxIRQL(DISPATCH_LEVEL) ULONG FORCEINLINE StorPortPatchAllocateContiguousMemorySpecifyCacheNode(__in PVOID HwDeviceExtension,__in SIZE_T NumberOfBytes,__in PHYSICAL_ADDRESS LowestAcceptableAddress,__in PHYSICAL_ADDRESS HighestAcceptableAddress,__in_opt PHYSICAL_ADDRESS BoundaryAddressMultiple,__in MEMORY_CACHING_TYPE CacheType,__in NODE_REQUIREMENT PreferredNode,__out PVOID* BufferPointer)
{
	UNREFERENCED_PARAMETER(HwDeviceExtension);
	UNREFERENCED_PARAMETER(PreferredNode);

	// we can not use MmAllocateContiguousMemorySpecifyCacheNode for Windows Server 2003, because the minimum target OS is Windows Vista
	*BufferPointer = MmAllocateContiguousMemorySpecifyCache(NumberOfBytes,LowestAcceptableAddress,HighestAcceptableAddress,BoundaryAddressMultiple,CacheType);
	if(*BufferPointer == NULL)
	{
		return STOR_STATUS_INSUFFICIENT_RESOURCES;
	}

	return STOR_STATUS_SUCCESS;
}


//------------------------------------------------------------------------------
// _StorpFreeContiguousMemorySpecifyCache@16
//------------------------------------------------------------------------------
ULONG FORCEINLINE StorPortPatchFreeContiguousMemorySpecifyCache(__in PVOID HwDeviceExtension,__in_bcount (NumberOfBytes) PVOID BaseAddress,__in SIZE_T NumberOfBytes,__in MEMORY_CACHING_TYPE CacheType)
{
	UNREFERENCED_PARAMETER(HwDeviceExtension);

	MmFreeContiguousMemorySpecifyCache(BaseAddress,NumberOfBytes,CacheType);

	return STOR_STATUS_SUCCESS;
}


#endif /* __STOR_PORT_PATCH_H__ */