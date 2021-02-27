
spec2def(ext-ms-win-ntuser-window-l1-1-4.dll ext-ms-win-ntuser-window-l1-1-4.spec)

list(APPEND SOURCE
    ext-ms-win-ntuser-window-l1-1-4.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ext-ms-win-ntuser-window-l1-1-4_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ext-ms-win-ntuser-window-l1-1-4.def)

add_library(ext-ms-win-ntuser-window-l1-1-4 SHARED ${SOURCE})

set_module_type(ext-ms-win-ntuser-window-l1-1-4 win32dll ENTRYPOINT 0)
add_importlibs(ext-ms-win-ntuser-window-l1-1-4 user32)
add_dependencies(ext-ms-win-ntuser-window-l1-1-4 psdk)
add_cd_file(TARGET ext-ms-win-ntuser-window-l1-1-4 DESTINATION reactos/system32 FOR all)
