
add_definitions(
    -D__WINESRC__
    -DUSE_WIN32_OPENGL)

include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(d3d9base.dll d3d9base.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    buffer.c
    d3d9_main.c
    device.c
    directx.c
    query.c
    shader.c
    stateblock.c
    surface.c
    swapchain.c
    texture.c
    vertexdeclaration.c
    volume.c
    d3d9_private.h)

add_library(d3d9base SHARED
    ${SOURCE}
    guid.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/d3d9base_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/d3d9base.def)

set_module_type(d3d9base win32dll UNICODE)
target_link_libraries(d3d9base wine)
add_importlibs(d3d9base d3dwine user32 msvcrt kernel32 ntdll)
add_pch(d3d9base d3d9base_private.h SOURCE)
add_cd_file(TARGET d3d9base DESTINATION reactos/system32 FOR all)
