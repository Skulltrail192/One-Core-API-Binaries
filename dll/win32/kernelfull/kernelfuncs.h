UINT WINAPI GetSystemFirmwareTable(IN DWORD FirmwareTableProviderSignature,
                        IN DWORD FirmwareTableID,
                        OUT PVOID pFirmwareTableBuffer,
                        IN DWORD BufferSize);

BOOL WINAPI GetLogicalProcessorInformation(
  _Out_    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION Buffer,
  _Inout_  PDWORD ReturnLength
);

typedef DWORD (WINAPI *PPROCESS_START_ROUTINE)(
    VOID
    );

VOID
WINAPI
BaseProcessStartThunk(
    PPROCESS_START_ROUTINE lpStartAddress,
    IN LPVOID lpParameter
);