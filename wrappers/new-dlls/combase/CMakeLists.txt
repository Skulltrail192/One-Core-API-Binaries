
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)

spec2def(combase.dll combase.spec ADD_IMPORTLIB)
generate_idl_iids(dcom.idl)
add_idl_headers(combase_idlheader dcom.idl irpcss.idl)
add_rpc_files(client irpcss.idl)

list(APPEND SOURCE
	apartment.c
	combase.c
	errorinfo.c
	hglobalstream.c
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
	${CMAKE_CURRENT_BINARY_DIR}/dcom_i.c
    ${CMAKE_CURRENT_BINARY_DIR}/irpcss_c.c)

add_library(combase SHARED ${SOURCE})

set_module_type(combase win32dll UNICODE)
add_dependencies(combase combase_idlheader)
target_link_libraries(combase uuid wine ${PSEH_LIB})
add_importlibs(combase user32 gdi32 advapi32 msvcrt kernelbase rpcrt4 oleaut32 ole32 ntdll)
add_cd_file(TARGET combase DESTINATION reactos/system32 FOR all)
