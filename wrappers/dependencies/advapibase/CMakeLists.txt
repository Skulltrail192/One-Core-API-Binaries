
spec2def(advapibase.dll advapibase.spec ADD_IMPORTLIB)
add_definitions(-D_advapibase_)

include_directories(
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/idl
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/drivers/ksecdd
    ${REACTOS_SOURCE_DIR}/sdk/lib/cryptlib
    ${CMAKE_CURRENT_BINARY_DIR})

add_rpc_files(client
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/idl/eventlogrpc.idl
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/idl/lsa.idl
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/idl/svcctl.idl
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/idl/winreg.idl)

list(APPEND SOURCE
	unimplemented.c
    misc/dllmain.c
    misc/efs.c
    misc/evntrace.c
    misc/hwprofiles.c
    misc/logon.c
    misc/msi.c
    misc/shutdown.c
    misc/sysfunc.c
    misc/unicode.c
    reg/hkcr.c
    reg/reg.c
    sec/ac.c
    sec/audit.c
    sec/lsa.c
    sec/misc.c
    sec/safer.c
    sec/sec.c
    sec/trustee.c
    service/eventlog.c
    service/rpc.c
    service/scm.c
    service/sctrl.c
    service/winreg.c
    token/token.c
    wine/cred.c
    wine/crypt.c
    wine/crypt_des.c
    wine/crypt_lmhash.c
    wine/security.c
    advapi32.h)

add_library(advapibase MODULE
    ${SOURCE}
    advapibase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/advapibase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/advapibase.def
    ${CMAKE_CURRENT_BINARY_DIR}/eventlogrpc_c.c
    ${CMAKE_CURRENT_BINARY_DIR}/lsa_c.c
    ${CMAKE_CURRENT_BINARY_DIR}/svcctl_c.c
    ${CMAKE_CURRENT_BINARY_DIR}/winreg_c.c)

set_module_type(advapibase win32dll UNICODE ENTRYPOINT DllMain 12)
target_link_libraries(advapibase cryptlib wine ${PSEH_LIB})
add_delay_importlibs(advapibase secur32)
add_importlibs(advapibase advapi32_vista rpcrt4 kernel32 ntdll)
add_pch(advapibase advapi32.h SOURCE)
add_cd_file(TARGET advapibase DESTINATION reactos/system32 FOR all)
