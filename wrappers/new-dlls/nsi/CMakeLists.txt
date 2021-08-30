
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

spec2def(nsi.dll nsi.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    nsi.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/nsi_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/nsi.def)
	

add_library(nsi SHARED ${SOURCE})
set_module_type(nsi win32dll)
target_link_libraries(nsi wine)
add_importlibs(nsi msvcrt kernel32 ntdll mshtml)
add_cd_file(TARGET nsi DESTINATION reactos/system32 FOR all)
