
add_definitions(
    -D__WINESRC__
    -DUSE_WIN32_OPENGL)

include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)

spec2def(d3d10core.dll d3d10core.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    async.c
	buffer.c
	d3d10core_main.c
	device.c
	inputlayout.c
	shader.c
	state.c
	texture.c
	utils.c
	view.c
    d3d10core_private.h)

add_library(d3d10core SHARED
    ${SOURCE}
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/d3d10core_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/d3d10core.def)

set_module_type(d3d10core win32dll UNICODE HOTPATCHABLE)
target_link_libraries(d3d10core wine uuid dxguid d3d10_uuids d3d10_1_uuids dxgi_uuids)
add_dependencies(d3d10core d3d_idl_headers)
add_importlibs(d3d10core wined3d msvcrt dxgi kernel32 ntdll)
add_pch(d3d10core d3d10core_private.h SOURCE)
add_cd_file(TARGET d3d10core DESTINATION reactos/system32 FOR all)
