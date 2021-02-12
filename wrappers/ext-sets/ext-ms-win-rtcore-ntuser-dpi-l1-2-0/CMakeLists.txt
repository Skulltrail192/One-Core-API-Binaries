
spec2def(ext-ms-win-rtcore-ntuser-dpi-l1-2-0.dll ext-ms-win-rtcore-ntuser-dpi-l1-2-0.spec)

list(APPEND SOURCE
    ext-ms-win-rtcore-ntuser-dpi-l1-2-0.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ext-ms-win-rtcore-ntuser-dpi-l1-2-0_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ext-ms-win-rtcore-ntuser-dpi-l1-2-0.def)

add_library(ext-ms-win-rtcore-ntuser-dpi-l1-2-0 SHARED ${SOURCE})

set_module_type(ext-ms-win-rtcore-ntuser-dpi-l1-2-0 win32dll ENTRYPOINT 0)
add_importlibs(ext-ms-win-rtcore-ntuser-dpi-l1-2-0 user32)
add_dependencies(ext-ms-win-rtcore-ntuser-dpi-l1-2-0 psdk)
add_cd_file(TARGET ext-ms-win-rtcore-ntuser-dpi-l1-2-0 DESTINATION reactos/system32 FOR all)
