
add_definitions(
    -D__WINESRC__
    -D_WINE
    -D_CRYPT32_)

include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(cryptfull.dll cryptfull.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    base64.c
    cert.c
    chain.c
    collectionstore.c
    context.c
    crl.c
    ctl.c
    decode.c
    encode.c
    filestore.c
    main.c
    message.c
    msg.c
    object.c
    oid.c
    proplist.c
    protectdata.c
    provstore.c
    regstore.c
    rootstore.c
    serialize.c
    sip.c
    store.c
    str.c
	unimplemented.c
    crypt32_private.h)

add_library(cryptfull SHARED
    ${SOURCE}
    cryptfull.rc
    ${CMAKE_CURRENT_BINARY_DIR}/cryptfull_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/cryptfull.def)

set_module_type(cryptfull win32dll)
target_link_libraries(cryptfull wine ${PSEH_LIB} oldnames)
add_importlibs(cryptfull user32 advapi32 shlwapi msvcrt kernel32 ntdll)
add_delay_importlibs(cryptfull cryptnet)
add_pch(cryptfull crypt32_private.h SOURCE)
add_cd_file(TARGET cryptfull DESTINATION reactos/system32 FOR all)
