
add_definitions(
    -D__WINESRC__
    -D_olebase_
    -DCOM_NO_WINDOWS_H
    -DENTRY_PREFIX=olebase_
    -DPROXY_CLSID=CLSID_PSFactoryBuffer
    -DWINE_REGISTER_DLL)

spec2def(vle32.dll vle32.spec)

generate_idl_iids(dcom.idl)
add_idl_headers(dcom.idl)

add_rpcproxy_files(
    dcom.idl)

list(APPEND SOURCE
    main.c
	socket.c
	version.rc
	${CMAKE_CURRENT_BINARY_DIR}/dcom_i.c
    ${CMAKE_CURRENT_BINARY_DIR}/vle32_stubs.c
	${CMAKE_CURRENT_BINARY_DIR}/proxy.dlldata.c
    ${CMAKE_CURRENT_BINARY_DIR}/vle32.def)

add_library(vle32 SHARED  ${SOURCE} ${CMAKE_CURRENT_BINARY_DIR}/dcom_p.c)
set_module_type(vle32 win32dll ENTRYPOINT 0)
target_link_libraries(vle32 wine)
add_importlibs(vle32 olebase rpcrt4 msvcrt kernel32 ntdll)
add_cd_file(TARGET vle32 DESTINATION reactos/system32 FOR all)
