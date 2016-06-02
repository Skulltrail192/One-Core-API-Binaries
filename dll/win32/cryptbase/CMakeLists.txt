
add_definitions(
    -D__WINESRC__
    -D_WINE
    -D_CRYPT32_)

include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(cryptbase.dll cryptbase.spec ADD_IMPORTLIB)

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
    crypt32_private.h)

add_library(cryptbase SHARED
    ${SOURCE}
    cryptbase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/cryptbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/cryptbase.def)

set_module_type(cryptbase win32dll)
target_link_libraries(cryptbase wine ${PSEH_LIB} oldnames)
add_importlibs(cryptbase user32 advapi32 shlwapi msvcrt kernel32 ntdll)
add_delay_importlibs(cryptbase cryptnet)
add_pch(cryptbase crypt32_private.h SOURCE)
add_cd_file(TARGET cryptbase DESTINATION reactos/system32 FOR all)
