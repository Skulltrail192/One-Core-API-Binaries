
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/dxsdk)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)

spec2def(opcservices.dll opcservices.spec)

list(APPEND SOURCE
    compress.c
    deflate.c
	factory.c
	main.c
	package.c
	uri.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/opcservices_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/opcservices.def)

add_idl_headers(opcservices_idlheader opcservices.idl)
	
add_library(opcservices SHARED ${SOURCE})
add_dependencies(opcservices opcservices_idlheader stdole2 dxsdk)
set_module_type(opcservices win32dll ENTRYPOINT DllMain)
target_link_libraries(opcservices wine uuid strmiids)
add_importlibs(opcservices ole32 advapi32 urlmon xmllite oleaut32 ntdll kernel32 msvcrt)
add_cd_file(TARGET opcservices DESTINATION reactos/system32 FOR all)
