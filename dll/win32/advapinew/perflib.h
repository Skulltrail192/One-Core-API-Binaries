/**
* This file has no copyright assigned and is placed in the Public Domain.
* This file is part of the w64 mingw-runtime package.
* No warranty is given; refer to the file DISCLAIMER.PD within this package.
*/
#ifndef _INC_PERFLIB
#define _INC_PERFLIB
#ifdef __cplusplus
extern "C" {
#endif
#ifndef WINADVAPI
#define WINADVAPI DECLSPEC_IMPORT
#endif
typedef LPVOID (CALLBACK *PERF_MEM_ALLOC)(SIZE_T AllocSize,LPVOID pContext);

typedef ULONG (WINAPI *PERFLIBREQUEST)(ULONG RequestCode,PVOID Buffer,ULONG BufferSize);

typedef void (CALLBACK *PERF_MEM_FREE)(LPVOID pBuffer,LPVOID pContext);

typedef struct _PERF_PROVIDER_CONTEXT {
	DWORD ContextSize;
	DWORD Reserved;
	PERFLIBREQUEST ControlCallback;
	PERF_MEM_ALLOC MemAllocRoutine;
	PERF_MEM_FREE MemFreeRoutine;
	LPVOID pMemContext;
} PERF_PROVIDER_CONTEXT, *PPERF_PROVIDER_CONTEXT;

typedef struct _PERF_COUNTER_IDENTITY {
	GUID CounterSetGuid;
	ULONG BufferSize;
	ULONG CounterId;
	ULONG InstanceId;
	ULONG MachineOffset;
	ULONG NameOffset;
	ULONG Reserved;
} PERF_COUNTER_IDENTITY, *PPERF_COUNTER_IDENTITY;

typedef struct _PERF_COUNTER_INFO {
	ULONG CounterId;
	ULONG Type;
	ULONGLONG Attrib;
	ULONG Size;
	ULONG DetailLevel;
	LONG Scale;
	ULONG Offset;
} PERF_COUNTER_INFO, *PPERF_COUNTER_INFO;

typedef struct _PERF_COUNTERSET_INFO {
	GUID CounterSetGuid;
	GUID ProviderGuid;
	ULONG NumCounters;
	ULONG InstanceType;
} PERF_COUNTERSET_INFO, *PPERF_COUNTERSET_INFO;

typedef struct _PERF_COUNTERSET_INSTANCE {
	GUID CounterSetGuid;
	ULONG dwSize;
	ULONG InstanceId;
	ULONG InstanceNameOffset;
	ULONG InstanceNameSize;
} PERF_COUNTERSET_INSTANCE, *PPERF_COUNTERSET_INSTANCE;

PPERF_COUNTERSET_INSTANCE WINAPI PerfCreateInstance(
	HANDLE hProvider,
	LPCGUID CounterSetGuid,
	LPCWSTR szInstanceName,
	ULONG dwInstance
);

ULONG WINAPI PerfDecrementULongCounterValue(
	HANDLE hProvider,
	PPERF_COUNTERSET_INSTANCE pInstance,
	ULONG CounterId,
	ULONG lValue
);

ULONG WINAPI PerfDecrementULongLongCounterValue(
	HANDLE hProvider,
	PPERF_COUNTERSET_INSTANCE pInstance,
	ULONG CounterId,
	ULONGLONG llValue
);

ULONG WINAPI PerfDeleteInstance(
	HANDLE hProvider,
	PPERF_COUNTERSET_INSTANCE InstanceBlock
);

ULONG WINAPI PerfIncrementULongCounterValue(
	HANDLE hProvider,
	PPERF_COUNTERSET_INSTANCE pInstance,
	ULONG CounterId,
	ULONG lValue
);

ULONG WINAPI PerfIncrementULongLongCounterValue(
	HANDLE hProvider,
	PPERF_COUNTERSET_INSTANCE pInstance,
	ULONG CounterId,
	ULONGLONG llValue
);

PPERF_COUNTERSET_INSTANCE WINAPI PerfQueryInstance(
	HANDLE hProvider,
	LPCGUID CounterSetGuid,
	LPCWSTR szInstance,
	ULONG dwInstance
);

ULONG WINAPI PerfSetCounterRefValue(
	HANDLE hProvider,
	PPERF_COUNTERSET_INSTANCE pInstance,
	ULONG CounterId,
	PVOID lpAddr
);

ULONG WINAPI PerfSetCounterSetInfo(
	HANDLE hProvider,
	PPERF_COUNTERSET_INFO pTemplate,
	ULONG dwTemplateSize
);

ULONG WINAPI PerfSetULongCounterValue(
	HANDLE hProvider,
	PPERF_COUNTERSET_INSTANCE pInstance,
	ULONG CounterId,
	ULONG lValue
);

ULONG WINAPI PerfSetULongLongCounterValue(
	HANDLE hProvider,
	PPERF_COUNTERSET_INSTANCE pInstance,
	ULONG CounterId,
	ULONGLONG llValue
);

ULONG WINAPI PerfStartProvider(
	LPGUID ProviderGuid,
	PERFLIBREQUEST ControlCallback,
	HANDLE *phProvider
);

ULONG WINAPI PerfStartProviderEx(
	LPGUID ProviderGuid,
	PPERF_PROVIDER_CONTEXT ProviderContext,
	HANDLE *phProvider
);

ULONG WINAPI PerfStopProvider(
	HANDLE hProvider
);
#ifdef __cplusplus
}
#endif
#endif /*_INC_PERFLIB*/