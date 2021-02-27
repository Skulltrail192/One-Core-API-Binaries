
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine
					${REACTOS_SOURCE_DIR}/sdk/lib/tdilib)
spec2def(iphlpapi_wrapper.dll iphlpapi_wrapper.spec)

list(APPEND SOURCE
    iphlpapi.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/iphlpapi_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/iphlpapi_wrapper.def)

add_library(iphlpapi_wrapper SHARED ${SOURCE})
set_module_type(iphlpapi_wrapper win32dll)
target_link_libraries(iphlpapi_wrapper wine tdilib)
add_importlibs(iphlpapi_wrapper advapi32 msvcrt kernel32 iphlpapibase ws2_32 ntdll)
add_cd_file(TARGET iphlpapi_wrapper DESTINATION reactos/system32 FOR all)
