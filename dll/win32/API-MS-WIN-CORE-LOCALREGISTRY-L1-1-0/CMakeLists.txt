
spec2def(api-ms-win-core-localregistry-l1-1-0.dll api-ms-win-core-localregistry-l1-1-0.spec)

list(APPEND SOURCE
    api-ms-win-core-localregistry-l1-1-0.rc
    ${CMAKE_CURRENT_BINARY_DIR}/api-ms-win-core-localregistry-l1-1-0_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/api-ms-win-core-localregistry-l1-1-0.def)

add_library(api-ms-win-core-localregistry-l1-1-0 SHARED ${SOURCE})

set_module_type(api-ms-win-core-localregistry-l1-1-0 win32dll ENTRYPOINT 0)
add_importlibs(api-ms-win-core-localregistry-l1-1-0 kernel32)
add_dependencies(api-ms-win-core-localregistry-l1-1-0 psdk)
add_cd_file(TARGET api-ms-win-core-localregistry-l1-1-0 DESTINATION reactos/system32 FOR all)
