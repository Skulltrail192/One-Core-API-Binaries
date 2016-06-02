
add_definitions(
    -D__WINESRC__
    -DUSE_WIN32_OPENGL)

include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/include/psdk)

spec2def(d2d1.dll d2d1.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    bitmap.c
	brush.c
	factory.c
	mesh.c
	render_target.c
	stroke.c
	wic_render_target.c
    d2d1_private.h)

add_library(d2d1 SHARED
    ${SOURCE}
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/d2d1_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/d2d1.def)

set_module_type(d2d1 win32dll UNICODE HOTPATCHABLE)
target_link_libraries(d2d1 wine d2d1_uuids dxguid uuid)
add_importlibs(d2d1 msvcrt kernel32 ntdll)
add_pch(d2d1 d2d1_private.h SOURCE)
add_cd_file(TARGET d2d1 DESTINATION reactos/system32 FOR all)
