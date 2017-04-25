
add_definitions(
    -D__WINESRC__
    -DUSE_WIN32_OPENGL)

include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/include/psdk)

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
    view.c)

add_library(d3d11 SHARED
    ${SOURCE}
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/d3d11_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/d3d11.def)

set_module_type(d3d11 win32dll UNICODE HOTPATCHABLE)
target_link_libraries(d3d11 wine uuid dxguid dxgi_uuids)
add_importlibs(d3d11 d3dwine msvcrt dxgi kernel32 ntdll)
add_pch(d3d11 d3d11_private.h SOURCE)
add_cd_file(TARGET d3d11 DESTINATION reactos/system32 FOR all)
