
add_definitions(-D__WINESRC__)
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos)
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

spec2def(d3d9_dxg.dll d3d9_dxg.spec ADD_IMPORTLIB)

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

add_library(d3d9_dxg SHARED
    ${SOURCE}
    d3d9.rc
    ${CMAKE_CURRENT_BINARY_DIR}/d3d9_dxg.def)

set_module_type(d3d9_dxg win32dll)
target_link_libraries(d3d9_dxg dxguid uuid wine)
add_importlibs(d3d9_dxg gdi32 d3d8thk user32 advapi32 msvcrt kernel32 ntdll version)
add_dependencies(d3d9_dxg wineheaders d3d_idl_headers)
add_cd_file(TARGET d3d9_dxg DESTINATION reactos/system32 FOR all)
