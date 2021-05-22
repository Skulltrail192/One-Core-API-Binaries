
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)

spec2def(combase.dll combase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	apartment.c
	combase.c
	errorinfo.c
	hglobalstream.c
	git.c #from wine ol32, because WinXP/2003 not implement some functions
	imports.c
	malloc.c
	marshal.c
	roapi.c
	rpc.c
	string.c
	stubmanager.c
	usrmarshal.c
    combase.rc
	${CMAKE_CURRENT_BINARY_DIR}/combase_stubs.c
	${CMAKE_CURRENT_BINARY_DIR}/combase.def
	)

add_library(combase SHARED ${SOURCE})

add_idl_headers(combase_idlheader dcom.idl irpcss.idl)

set_module_type(combase win32dll UNICODE)
add_dependencies(combase combase_idlheader)
target_link_libraries(combase uuid wine ${PSEH_LIB})
add_importlibs(combase user32 gdi32 advapi32 msvcrt kernelbase rpcrt4 ole32 oleaut32 ntdll)
add_cd_file(TARGET combase DESTINATION reactos/system32 FOR all)
