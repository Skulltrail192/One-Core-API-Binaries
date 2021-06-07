
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/dxsdk)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)

spec2def(mf.dll mf.spec)

list(APPEND SOURCE
    copier.c
    evr.c
	main.c
	samplegrabber.c
	sar.c
	session.c
	topology.c
    ${CMAKE_CURRENT_BINARY_DIR}/mf_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/mf.def)

add_idl_headers(mf_idlheader mf.idl)
# add_typelib(mf.idl)
# add_rpcproxy_files(mf.idl)
	
add_library(mf SHARED ${SOURCE} mf.rc)
add_dependencies(mf mf_idlheader stdole2 dxsdk)
set_module_type(mf win32dll ENTRYPOINT DllMain)
target_link_libraries(mf wine uuid mfuuid strmiids)
add_delay_importlibs(mf evr user32)
add_importlibs(mf advapi32 mfplat ole32 ntdll kernel32 msvcrt)
add_cd_file(TARGET mf DESTINATION reactos/system32 FOR all)
