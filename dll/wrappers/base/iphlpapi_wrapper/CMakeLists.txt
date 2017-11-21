
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(iphlpapi_wrapper.dll iphlpapi_wrapper.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/iphlpapi_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/iphlpapi_wrapper.def)

add_library(iphlpapi_wrapper SHARED ${SOURCE})
set_module_type(iphlpapi_wrapper win32dll)
target_link_libraries(iphlpapi_wrapper wine)
add_importlibs(iphlpapi_wrapper advapi32 msvcrt kernel32 iphlpapibase ntdll)
add_cd_file(TARGET iphlpapi_wrapper DESTINATION reactos/system32 FOR all)
