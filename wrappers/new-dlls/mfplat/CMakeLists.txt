
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/dxsdk)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)

spec2def(mfplat.dll mfplat.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    buffer.c
	main.c
	mediatype.c
	queue.c
	sample.c
    ${CMAKE_CURRENT_BINARY_DIR}/evr_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/mfplat.def)
	
add_library(mfplat SHARED ${SOURCE} mfplat.rc)
add_dependencies(mfplat dxsdk)
set_module_type(mfplat win32dll)
target_link_libraries(mfplat wine uuid)
add_importlibs(mfplat shell32 oleaut32 ole32 user32 advapi32 shlwapi ntdll kernel32 msvcrt)
add_cd_file(TARGET mfplat DESTINATION reactos/system32 FOR all)
