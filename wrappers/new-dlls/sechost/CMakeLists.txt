
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/dxsdk)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)

spec2def(sechost.dll sechost.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    security.c
	service.c
	trace.c
    ${CMAKE_CURRENT_BINARY_DIR}/sechost_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/sechost.def)

add_idl_headers(sechost_idlheader svcctl.idl plugplay.idl)
# add_typelib(sechost_classes.idl)
# add_rpcproxy_files(sechost_classes.idl)
	
add_library(sechost SHARED ${SOURCE})
add_dependencies(sechost sechost_idlheader)
set_module_type(sechost win32dll)
add_importlibs(sechost shell32 ntdll kernel32)
add_cd_file(TARGET sechost DESTINATION reactos/system32 FOR all)
