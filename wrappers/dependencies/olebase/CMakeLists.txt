
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)

add_definitions(
    -D__WINESRC__
    -D_OLE32_
    -DCOM_NO_WINDOWS_H
    -DENTRY_PREFIX=OLE32_
    -DPROXY_CLSID=CLSID_PSFactoryBuffer
    -DWINE_REGISTER_DLL)

include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

spec2def(olebase.dll olebase.spec ADD_IMPORTLIB)
generate_idl_iids(dcom.idl)
add_idl_headers(olebaseidl dcom.idl irot.idl)
add_rpc_files(client irot.idl)

add_rpcproxy_files(
    dcom.idl
    olebase_unknwn.idl
    olebase_objidl.idl
    olebase_oleidl.idl)

list(APPEND SOURCE
    antimoniker.c
    bindctx.c
    classmoniker.c
    clipboard.c
    comcat.c
    compobj.c
    compositemoniker.c
    datacache.c
    defaulthandler.c
    dictionary.c
    enumx.c
    errorinfo.c
    filelockbytes.c
    filemoniker.c
    ftmarshal.c
    git.c
    hglobalstream.c
    ifs.c
    itemmoniker.c
    marshal.c
    memlockbytes.c
    moniker.c
    ole2.c
    ole2impl.c
    ole2stubs.c
    olebase_main.c
    oleobj.c
    oleproxy.c
    pointermoniker.c
    rpc.c
    stg_prop.c
    stg_stream.c
    storage32.c
    stubmanager.c
    usrmarshal.c
    precomp.h
    ${CMAKE_CURRENT_BINARY_DIR}/dcom_i.c
    ${CMAKE_CURRENT_BINARY_DIR}/irot_c.c)

if(MSVC AND (ARCH STREQUAL "i386"))
    list(APPEND SOURCE msvc-thiscall.c)
endif()

add_library(olebase SHARED
    ${SOURCE}
    guid.c
    olebaseres.rc
    ${CMAKE_CURRENT_BINARY_DIR}/dcom_p.c
    ${CMAKE_CURRENT_BINARY_DIR}/olebase_objidl_p.c
    ${CMAKE_CURRENT_BINARY_DIR}/olebase_oleidl_p.c
    ${CMAKE_CURRENT_BINARY_DIR}/olebase_unknwn_p.c
    ${CMAKE_CURRENT_BINARY_DIR}/olebase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/proxy.dlldata.c
    ${CMAKE_CURRENT_BINARY_DIR}/olebase.def)

set_module_type(olebase win32dll)
target_link_libraries(olebase wine uuid ${PSEH_LIB})
add_importlibs(olebase advapi32 user32 gdi32 rpcrt4 msvcrt kernel32 ntdll)
add_delay_importlibs(olebase oleaut32)
add_dependencies(olebase olebaseidl)
add_pch(olebase precomp.h SOURCE)
add_cd_file(TARGET olebase DESTINATION reactos/system32 FOR all)
