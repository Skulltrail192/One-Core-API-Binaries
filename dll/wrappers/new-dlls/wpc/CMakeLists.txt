
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)

spec2def(wpc.dll wpc.spec)

list(APPEND SOURCE
    wpc.c
    ${CMAKE_CURRENT_BINARY_DIR}/wpc_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/wpc.def)

add_idl_headers(wpc_idlheader wpc_classes.idl)
add_typelib(wpc_classes.idl)
add_rpcproxy_files(wpc_classes.idl)
	
add_library(wpc SHARED ${SOURCE} wpc.rc)
add_dependencies(wpc wpc_idlheader stdole2)
set_module_type(wpc win32dll)
target_link_libraries(wpc wine uuid)
add_importlibs(wpc shell32 oleaut32 ole32 user32 advapi32 shlwapi ntdll kernel32 msvcrt)
add_cd_file(TARGET wpc DESTINATION reactos/system32 FOR all)
