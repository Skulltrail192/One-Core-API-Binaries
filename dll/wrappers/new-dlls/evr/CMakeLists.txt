
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/dxsdk)

spec2def(evr.dll evr.spec)

list(APPEND SOURCE
    evr.c
    ${CMAKE_CURRENT_BINARY_DIR}/evr_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/evr.def)

add_idl_headers(evr_idlheader evr_classes.idl)
add_typelib(evr_classes.idl)
add_rpcproxy_files(evr_classes.idl)
	
add_library(evr SHARED ${SOURCE} evr.rc)
add_dependencies(evr evr_idlheader stdole2 dxsdk)
set_module_type(evr win32dll)
target_link_libraries(evr wine uuid)
add_importlibs(evr shell32 oleaut32 ole32 user32 advapi32 shlwapi ntdll kernel32 msvcrt)
add_cd_file(TARGET evr DESTINATION reactos/system32 FOR all)
