
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/lib/3rdparty/freetype/include)
include_directories(${REACTOS_SOURCE_DIR}/sdk/lib/3rdparty/freetype/include/freetype)
spec2def(gdi32_wrapper.dll gdi32_wrapper.spec)

list(APPEND SOURCE
	dc.c
    font.c
	main.c
	d3dkmt.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/gdi32_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/gdi32_wrapper.def)

add_library(gdi32_wrapper SHARED ${SOURCE})
set_module_type(gdi32_wrapper win32dll ENTRYPOINT DllMain 12 UNICODE)
target_link_libraries(gdi32_wrapper wine dxguid freetype)
add_delay_importlibs(gdi32_wrapper setupapi advapi32)
add_importlibs(gdi32_wrapper gdibase d3dkmt msvcrt kernel32 user32 ntdll)
add_cd_file(TARGET gdi32_wrapper DESTINATION reactos/system32 FOR all)
