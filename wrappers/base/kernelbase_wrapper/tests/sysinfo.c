#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

HMODULE hkernel32;

void testNativeSystemInfo(){
	SYSTEM_INFO info;

	GetNativeSystemInfo(&info);

	wprintf(L"   *************************GetNativeSystemInfo Test*************************\n\n\n\n");
		
	wprintf(L"Processor architecture %d\n", info.wProcessorArchitecture);
	wprintf(L"Page Size %ld\n", info.dwPageSize);
	wprintf(L"Minimum Application Address %p\n", info.lpMinimumApplicationAddress);
	wprintf(L"Maximum Application Address %p\n", info.lpMaximumApplicationAddress);
	wprintf(L"Active Processor Mask %ld\n", info.dwActiveProcessorMask);
	wprintf(L"Number of processors %ld\n", info.dwNumberOfProcessors);
	wprintf(L"Processor Type %ld\n", info.dwProcessorType);
	wprintf(L"Allocation Granularity %ld\n", info.dwAllocationGranularity);
	wprintf(L"Processor Level %ld\n", info.wProcessorLevel);
	wprintf(L"Processor Revision %ld\n", info.wProcessorRevision);		
}

void testGetSystemInfo(){
	SYSTEM_INFO info;

	GetSystemInfo(&info);

	wprintf(L"   *************************GetSystemInfo Test*************************\n\n\n\n");
		
	wprintf(L"Processor architecture %d\n", info.wProcessorArchitecture);
	wprintf(L"Page Size %ld\n", info.dwPageSize);
	wprintf(L"Minimum Application Address %p\n", info.lpMinimumApplicationAddress);
	wprintf(L"Maximum Application Address %p\n", info.lpMaximumApplicationAddress);
	wprintf(L"Active Processor Mask %ld\n", info.dwActiveProcessorMask);
	wprintf(L"Number of processors %ld\n", info.dwNumberOfProcessors);
	wprintf(L"Processor Type %ld\n", info.dwProcessorType);
	wprintf(L"Allocation Granularity %ld\n", info.dwAllocationGranularity);
	wprintf(L"Processor Level %ld\n", info.wProcessorLevel);
	wprintf(L"Processor Revision %ld\n", info.wProcessorRevision);		
}

void main (){
	testNativeSystemInfo();
	testGetSystemInfo();
	system("PAUSE");
}
