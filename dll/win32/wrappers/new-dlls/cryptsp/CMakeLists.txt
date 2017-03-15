
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(cryptsp.dll cryptsp.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/cryptsp_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/cryptsp.def)

add_library(cryptsp SHARED ${SOURCE})
set_module_type(cryptsp win32dll)
target_link_libraries(cryptsp wine)
add_importlibs(cryptsp advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET cryptsp DESTINATION reactos/system32 FOR all)
