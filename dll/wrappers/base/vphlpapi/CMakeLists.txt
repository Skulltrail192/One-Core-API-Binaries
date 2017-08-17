
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(vphlpapi.dll vphlpapi.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vphlpapi_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vphlpapi.def)

add_library(vphlpapi SHARED ${SOURCE})
set_module_type(vphlpapi win32dll)
target_link_libraries(vphlpapi wine)
add_importlibs(vphlpapi advapi32 msvcrt kernel32 iphlpapibase ntdll)
add_cd_file(TARGET vphlpapi DESTINATION reactos/system32 FOR all)
