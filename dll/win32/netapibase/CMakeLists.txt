
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
add_definitions(-D__WINESRC__)

spec2def(netapibase.dll netapibase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    access.c
    apibuf.c
    browsr.c
    ds.c
    group.c
    local_group.c
    misc.c
    nbcmdqueue.c
    nbnamecache.c
    nbt.c
    netapi32.c
    netbios.c
    share.c
	unimplemented.c
    user.c
    wksta.c
    netapi32.h)

add_library(netapibase SHARED
    ${SOURCE}
    ${CMAKE_CURRENT_BINARY_DIR}/netapibase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/netapibase.def)

set_module_type(netapibase win32dll)

target_link_libraries(netapibase wine)

add_importlibs(netapibase iphlpapi ws2_32 advapi32 msvcrt kernel32 ntdll)

add_delay_importlibs(netapibase samlib secur32)
add_pch(netapibase netapi32.h SOURCE)
add_cd_file(TARGET netapibase DESTINATION reactos/system32 FOR all)

