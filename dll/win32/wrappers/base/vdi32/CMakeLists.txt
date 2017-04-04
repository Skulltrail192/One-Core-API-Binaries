
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/3rdparty/freetype/include)
spec2def(vdi32.dll vdi32.spec)

list(APPEND SOURCE
    font.c
	main.c
	d3dkmt.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vdi32_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vdi32.def)

add_library(vdi32 SHARED ${SOURCE})
set_module_type(vdi32 win32dll)
target_link_libraries(vdi32 wine)
add_importlibs(vdi32 advapi32 msvcrt kernel32 user32 ntdll gdibase)
add_cd_file(TARGET vdi32 DESTINATION reactos/system32 FOR all)
