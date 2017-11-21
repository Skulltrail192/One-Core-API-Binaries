	
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502 -DWINVER=0x502)
add_definitions(-D_WIN32_WINNT=0x600 -DWINVER=0x600)
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)
spec2def(dwrite.dll dwrite.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	analyzer.c
	bidi.c
	bracket.c
	font.c
	freetype.c
	gdiinterop.c
	layout.c
	# linebreak.c
	# main.c
	# mirror.c
	# opentype.c
	# scripts.c
	shape.c
	dwrite_private.h
	scripts.h
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/dwrite_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/dwrite.def)

add_library(dwrite SHARED ${SOURCE})
set_module_type(dwrite win32dll)
target_link_libraries(dwrite wine uuid dwrite_uuids d2d1_uuids dxguid d3d11_uuids d3d10_uuids d3d10_1_uuids dxgi_uuids)
add_importlibs(dwrite advapi32 msvcrt kernel32 ntdll user32 gdi32)
add_cd_file(TARGET dwrite DESTINATION reactos/system32 FOR all)
