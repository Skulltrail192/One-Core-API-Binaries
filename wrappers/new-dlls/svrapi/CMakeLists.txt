
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)

spec2def(svrapi.dll svrapi.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    svrapi_main.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/svrapi_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/svrapi.def)
	
add_library(svrapi SHARED ${SOURCE})
set_module_type(svrapi win32dll)
target_link_libraries(svrapi wine uuid)
add_importlibs(svrapi netapi32 ntdll kernel32 msvcrt)
add_cd_file(TARGET svrapi DESTINATION reactos/system32 FOR all)
