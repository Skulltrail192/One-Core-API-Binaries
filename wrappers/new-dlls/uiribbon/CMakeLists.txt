
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502 -DWINVER=0x502)
add_definitions(-D_WIN32_WINNT=0x600 -DWINVER=0x600)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_BINARY_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/dxsdk)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)
include_directories(${REACTOS_SOURCE_DIR}/wrappers/includes)

spec2def(uiribbon.dll uiribbon.spec)
add_idl_headers(uiribbon_idlheader uiribbon_classes.idl)

list(APPEND SOURCE
	main.c
	uiribbon.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/uiribbon_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/uiribbon.def)
	
add_library(uiribbon SHARED ${SOURCE})
add_dependencies(uiribbon uiribbon_idlheader)
set_module_type(uiribbon win32dll)
target_link_libraries(uiribbon wine uuid)
add_importlibs(uiribbon ole32 kernel32 ntdll msvcrt)
add_cd_file(TARGET uiribbon DESTINATION reactos/system32 FOR all)
