
spec2def(VERNEL32.dll VERNEL32.spec)

list(APPEND SOURCE
    VERNEL32.rc
    ${CMAKE_CURRENT_BINARY_DIR}/VERNEL32_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/VERNEL32.def)

add_library(VERNEL32 SHARED ${SOURCE})

set_module_type(VERNEL32 win32dll ENTRYPOINT 0)
add_importlibs(VERNEL32 kernelfull vtdll)
add_dependencies(VERNEL32 psdk)
add_cd_file(TARGET VERNEL32 DESTINATION reactos/system32 FOR all)
