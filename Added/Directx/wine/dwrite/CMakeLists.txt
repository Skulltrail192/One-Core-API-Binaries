	
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(dwrite.dll dwrite.spec)

list(APPEND SOURCE
	analyzer.c
	font.c
	gdiinterop.c
	layout.c
	linebreak.c
	main.c
	opentype.c
	dwrite_private.h
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/dwrite_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/dwrite.def)

add_library(dwrite SHARED ${SOURCE})
set_module_type(dwrite win32dll)
target_link_libraries(dwrite wine dwrite_uuids)
add_importlibs(dwrite advapi32 msvcrt kernel32 ntdll user32 gdi32)
add_cd_file(TARGET dwrite DESTINATION reactos/system32 FOR all)
