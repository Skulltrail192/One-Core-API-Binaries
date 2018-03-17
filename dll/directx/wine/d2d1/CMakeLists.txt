
add_definitions(
    -D__WINESRC__)

include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)

spec2def(d2d1.dll d2d1.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    bitmap.c
	bitmap_render_target.c
	brush.c
	dc_render_target.c
	factory.c
	geometry.c
	hwnd_render_target.c
	layer.c
	mesh.c
	render_target.c
	state_block.c
	stroke.c
	wic_render_target.c
    d2d1_private.h)

add_library(d2d1 SHARED
    ${SOURCE}
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/d2d1_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/d2d1.def)

set_module_type(d2d1 win32dll)
target_link_libraries(d2d1 wine d3d10_uuids d3d10_1_uuids dwrite_uuids d2d1_uuids dxguid dxgi_uuids uuid)
add_importlibs(d2d1 d3d10_1 msvcrt dwrite gdi32 user32 kernel32 ntdll advapi32)
add_cd_file(TARGET d2d1 DESTINATION reactos/system32 FOR all)
