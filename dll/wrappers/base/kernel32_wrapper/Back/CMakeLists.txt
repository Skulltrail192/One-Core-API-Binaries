
include_directories(
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/idl
    ${CMAKE_CURRENT_BINARY_DIR})
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/win)
include_directories(${REACTOS_SOURCE_DIR}/dll/wrappers/base/ntext_wrapper)
spec2def(kernel32_wrapper.dll kernel32_wrapper.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	appmodel.c
    DllMain.c
    # GetFileInformationByHandleEx.c
    # GetTickCount64.c
    # InitOnceExecuteOnce.c
   # sync.c
    ${CMAKE_CURRENT_BINARY_DIR}/kernel32_wrapper.def)

add_library(kernel32_wrapper SHARED ${SOURCE})
set_module_type(kernel32_wrapper win32dll ENTRYPOINT BaseDllInitialize 12)
add_importlibs(kernel32_wrapper kernel32 ntdll)
add_delay_importlibs(kernel32_wrapper ntdll_vista gdi32 comctl32 advapi32 user32 rpcrt4 version msvcrt kernel32 ntdll)
add_dependencies(kernel32_wrapper psdk)
target_link_libraries(kernel32_wrapper wine ${PSEH_LIB})
add_cd_file(TARGET kernel32_wrapper DESTINATION reactos/system32 FOR all)
