
add_definitions(
    -D__WINESRC__
    -DUSE_WIN32_OPENGL)

include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/include/psdk)

spec2def(d3d10_1.dll d3d10_1.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    d3d10_1_main.c)

add_library(d3d10_1 SHARED
    ${SOURCE}
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/d3d10_1_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/d3d10_1.def)

set_module_type(d3d10_1 win32dll UNICODE HOTPATCHABLE)
target_link_libraries(d3d10_1 wine d3d10_1_uuids dxgi_uuids dxguid uuid)
add_importlibs(d3d10_1 msvcrt kernel32 ntdll dxgi d3d10core d3d10)
add_cd_file(TARGET d3d10_1 DESTINATION reactos/system32 FOR all)
