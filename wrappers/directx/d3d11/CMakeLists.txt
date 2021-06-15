
add_definitions(
    -D__WINESRC__
    -DUSE_WIN32_OPENGL)

include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)

spec2def(d3d11.dll d3d11.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	async.c
	buffer.c
    d3d11_main.c
	device.c
	inputlayout.c
	shader.c
	state.c
	texture.c
	utils.c
    view.c
	d3d11_private.h)

add_library(d3d11 SHARED
    ${SOURCE}
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/d3d11_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/d3d11.def)

set_module_type(d3d11 win32dll)
target_link_libraries(d3d11 wine uuid dxguid d3d11_uuids d3d10_uuids d3d10_1_uuids dxgi_uuids)
add_importlibs(d3d11 wined3d msvcrt dxgi kernel32 ntdll gdi32)
add_dependencies(d3d11 wineheaders)
add_cd_file(TARGET d3d11 DESTINATION reactos/system32 FOR all)
