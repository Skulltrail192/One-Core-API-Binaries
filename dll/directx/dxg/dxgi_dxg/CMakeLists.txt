
add_definitions(-D__WINESRC__)
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

spec2def(dxgi_dxg.dll dxgi_dxg.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    adapter.c
    device.c
    dxgi_main.cpp
    factory.c
    output.c
    surface.c
    swapchain.c
    utils.c
    dxgi_private.h)

add_library(dxgi_dxg SHARED
    ${SOURCE}
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/dxgi_dxg.def)

set_module_type(dxgi_dxg win32dll)
target_link_libraries(dxgi_dxg dxguid uuid wine dxgi_uuids d3d10_uuids d3d10_1_uuids)
add_importlibs(dxgi_dxg d3dwine gdi32 user32 msvcrt kernel32 ntdll)
add_dependencies(dxgi_dxg wineheaders d3d_idl_headers)
add_cd_file(TARGET dxgi_dxg DESTINATION reactos/system32 FOR all)
