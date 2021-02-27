
spec2def(ext-ms-win-ntuser-misc-l1-5-1.dll ext-ms-win-ntuser-misc-l1-5-1.spec)

list(APPEND SOURCE
    ext-ms-win-ntuser-misc-l1-5-1.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ext-ms-win-ntuser-misc-l1-5-1_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ext-ms-win-ntuser-misc-l1-5-1.def)

add_library(ext-ms-win-ntuser-misc-l1-5-1 SHARED ${SOURCE})

set_module_type(ext-ms-win-ntuser-misc-l1-5-1 win32dll ENTRYPOINT 0)
add_importlibs(ext-ms-win-ntuser-misc-l1-5-1 user32 kernel32 ntdll)
add_dependencies(ext-ms-win-ntuser-misc-l1-5-1 psdk)
add_cd_file(TARGET ext-ms-win-ntuser-misc-l1-5-1 DESTINATION reactos/system32 FOR all)
