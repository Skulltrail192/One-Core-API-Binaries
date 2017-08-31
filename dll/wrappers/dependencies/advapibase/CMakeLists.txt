
spec2def(advapibase.dll advapibase.spec ADD_IMPORTLIB)

add_definitions(-D_advapibase_)

remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)

include_directories(
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/idl
    ${REACTOS_SOURCE_DIR}/sdk/lib/cryptlib
    ${CMAKE_CURRENT_BINARY_DIR})

add_rpc_files(client
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/idl/eventlogrpc.idl
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/idl/lsa.idl
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/idl/svcctl.idl)

list(APPEND SOURCE
    crypt/crypt.c
    crypt/crypt_des.c
    crypt/crypt_lmhash.c
    misc/dllmain.c
    misc/efs.c
    misc/hwprofiles.c
    misc/logon.c
    misc/msi.c
    misc/shutdown.c
    misc/sysfunc.c
    misc/trace.c
	misc/unimplemented.c
    reg/reg.c
    sec/ac.c
    sec/audit.c
    sec/cred.c
    sec/lsa.c
    sec/misc.c
    sec/safer.c
    sec/sec.c
    sec/sid.c
    sec/trustee.c
    service/eventlog.c
    service/rpc.c
    service/scm.c
    service/sctrl.c
    token/privilege.c
    token/token.c
    advapi32.h)

add_library(advapibase SHARED
    ${SOURCE}
    advapibase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/advapibase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/advapibase.def
    ${CMAKE_CURRENT_BINARY_DIR}/eventlogrpc_c.c
    ${CMAKE_CURRENT_BINARY_DIR}/lsa_c.c
    ${CMAKE_CURRENT_BINARY_DIR}/svcctl_c.c)

set_module_type(advapibase win32dll UNICODE ENTRYPOINT DllMain 12)
target_link_libraries(advapibase cryptlib wine ${PSEH_LIB})
add_delay_importlibs(advapibase secur32)
add_importlibs(advapibase rpcrt4 kernel32 ntdll)

add_pch(advapibase advapi32.h SOURCE)

add_cd_file(TARGET advapibase DESTINATION reactos/system32 FOR all)

