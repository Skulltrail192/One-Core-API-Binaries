
add_definitions(
    -D__WINESRC__
    -DHAVE_ZLIB)

include_directories(
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/libs/zlib)

spec2def(cabiext.dll cabiext.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    cabiext_main.c
    fci.c
    fdi.c
    cabinet.h)

add_library(cabiext SHARED
    ${SOURCE}
    stubs.c
    cabiext.rc
    ${CMAKE_CURRENT_BINARY_DIR}/cabiext.def)

set_module_type(cabiext win32dll)
target_link_libraries(cabiext wine zlib)
add_importlibs(cabiext msvcrt kernel32 ntdll)
add_pch(cabiext cabinet.h SOURCE)
add_cd_file(TARGET cabiext DESTINATION reactos/system32 FOR all)
