
spec2def(ext-ms-win-ntuser-window-l1-1-1.dll ext-ms-win-ntuser-window-l1-1-1.spec)

list(APPEND SOURCE
    ext-ms-win-ntuser-window-l1-1-1.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ext-ms-win-ntuser-window-l1-1-1_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ext-ms-win-ntuser-window-l1-1-1.def)

add_library(ext-ms-win-ntuser-window-l1-1-1 SHARED ${SOURCE})

set_module_type(ext-ms-win-ntuser-window-l1-1-1 win32dll ENTRYPOINT 0)
add_importlibs(ext-ms-win-ntuser-window-l1-1-1 user32 ntdll kernel32)
add_dependencies(ext-ms-win-ntuser-window-l1-1-1 psdk)
add_cd_file(TARGET ext-ms-win-ntuser-window-l1-1-1 DESTINATION reactos/system32 FOR all)
