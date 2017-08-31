
add_definitions(
    -D__WINESRC__
    -D_olebase_
    -DCOM_NO_WINDOWS_H
    -DENTRY_PREFIX=olebase_
    -DPROXY_CLSID=CLSID_PSFactoryBuffer
    -DWINE_REGISTER_DLL)

spec2def(ole32_wrapper32.dll ole32_wrapper32.spec)

generate_idl_iids(dcom.idl)
add_idl_headers(dcom.idl)

add_rpcproxy_files(
    dcom.idl)

list(APPEND SOURCE
    main.c
	socket.c
	version.rc
	${CMAKE_CURRENT_BINARY_DIR}/dcom_i.c
    ${CMAKE_CURRENT_BINARY_DIR}/ole32_wrapper32_stubs.c
	${CMAKE_CURRENT_BINARY_DIR}/proxy.dlldata.c
    ${CMAKE_CURRENT_BINARY_DIR}/ole32_wrapper32.def)

add_library(ole32_wrapper32 SHARED  ${SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/dcom_p.c)
set_module_type(ole32_wrapper32 win32dll ENTRYPOINT 0)
target_link_libraries(ole32_wrapper32 wine)
add_importlibs(ole32_wrapper32 olebase rpcrt4 msvcrt propsys kernel32 ntdll)
add_cd_file(TARGET ole32_wrapper32 DESTINATION reactos/system32 FOR all)
