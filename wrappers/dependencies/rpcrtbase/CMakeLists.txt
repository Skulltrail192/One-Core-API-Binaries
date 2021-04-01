
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)

add_definitions(
    -D__WINESRC__
    -D_rpcrtbase_
    -DCOM_NO_WINDOWS_H
    -DMSWMSG)

include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(rpcrtbase.dll rpcrtbase.spec ADD_IMPORTLIB)

add_rpc_files(client epm.idl)
add_idl_headers(ndr_types_header_base ndr_types.idl)

set(OLD_IDL_FLAGS ${IDL_FLAGS})
set(IDL_FLAGS ${IDL_FLAGS} -Oicf)
add_rpcproxy_files(ndr_types.idl)
set(IDL_FLAGS ${OLD_IDL_FLAGS})

list(APPEND SOURCE
    cproxy.c
    cpsf.c
    cstub.c
    ndr_clientserver.c
    ndr_contexthandle.c
    ndr_es.c
    ndr_fullpointer.c
    ndr_marshall.c
    ndr_ole.c
    ndr_stubless.c
    rpc_assoc.c
    rpc_async.c
    rpc_binding.c
    rpc_epmap.c
    rpc_message.c
    rpcrtbase_main.c
    rpc_server.c
    rpc_transport.c
    unix_func.c
    precomp.h
    ${CMAKE_CURRENT_BINARY_DIR}/epm_c.c)

if(MSVC AND NOT ARCH STREQUAL "arm")
    add_asm_files(rpcrtbase_asm msvc.S)
endif()

add_library(rpcrtbase MODULE
    ${SOURCE}
    ndr_typelib.c
    ${CMAKE_CURRENT_BINARY_DIR}/ndr_types_p.c
    ${CMAKE_CURRENT_BINARY_DIR}/proxy.dlldata.c
    ${rpcrtbase_asm}
    rpcrtbase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/rpcrtbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/rpcrtbase.def)

set_module_type(rpcrtbase win32dll)
target_link_libraries(rpcrtbase wine uuid ${PSEH_LIB})
add_delay_importlibs(rpcrtbase iphlpapi wininet secur32 user32 oleaut32)
add_importlibs(rpcrtbase advapi32 advapi32_vista kernel32_vista ws2_32 msvcrt kernel32 ntdll)
add_dependencies(rpcrtbase ndr_types_header_base)
add_pch(rpcrtbase precomp.h SOURCE)
add_cd_file(TARGET rpcrtbase DESTINATION reactos/system32 FOR all)
