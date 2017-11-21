
add_definitions(
    -D__WINESRC__
    -DUSE_WIN32_OPENGL)

include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)

spec2def(d3d10.dll d3d10.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    d3d10_main.c
    effect.c
	shader.c
	stateblock.c
	utils.c
    d3d10_private.h)

add_library(d3d10 SHARED
    ${SOURCE}
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/d3d10_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/d3d10.def)

set_module_type(d3d10 win32dll)
target_link_libraries(d3d10 wine uuid dxguid d3d10_uuids dxgi_uuids)
add_importlibs(d3d10 wined3d msvcrt d3dcompiler_43 dxgi d3d10core kernel32 ntdll)
#add_pch(d3d10 d3d10_private.h d3d10effect.h SOURCE)
add_cd_file(TARGET d3d10 DESTINATION reactos/system32 FOR all)
