
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(d3d9_reactos.dll d3d9.spec)

list(APPEND SOURCE
    adapter.c
    d3d9.c
    d3d9_baseobject.c
    d3d9_callbacks.c
    d3d9_caps.c
    d3d9_create.c
    d3d9_cursor.c
    d3d9_device.c
    d3d9_haldevice.c
    d3d9_helpers.c
    d3d9_impl.c
    d3d9_mipmap.c
    d3d9_puredevice.c
    d3d9_resource.c
    d3d9_swapchain.c
    d3d9_texture.c
    device.c
    format.c
    d3d9_private.h)

add_library(d3d9_reactos SHARED
    ${SOURCE}
    d3d9.rc
    ${CMAKE_CURRENT_BINARY_DIR}/d3d9_reactos_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/d3d9_reactos.def)

set_module_type(d3d9_reactos win32dll UNICODE)
target_link_libraries(d3d9_reactos wine)
add_importlibs(d3d9_reactos d3dwine user32 msvcrt kernel32 ntdll)
add_pch(d3d9 d3d9_private.h SOURCE)
add_cd_file(TARGET d3d9_reactos DESTINATION reactos/system32 FOR all)
