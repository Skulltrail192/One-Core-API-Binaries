
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(netapi32_wrapper.dll netapi32_wrapper.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/netapi32_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/netapi32_wrapper.def)

add_library(netapi32_wrapper SHARED ${SOURCE})
set_module_type(netapi32_wrapper win32dll)
target_link_libraries(netapi32_wrapper wine)
add_importlibs(netapi32_wrapper advapi32 msvcrt kernel32 netapibase netapinew ntdll)
add_cd_file(TARGET netapi32_wrapper DESTINATION reactos/system32 FOR all)
