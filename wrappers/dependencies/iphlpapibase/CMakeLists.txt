
include_directories(
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine
    ${REACTOS_SOURCE_DIR}/dll/win32/dhcpcsvc/include
    ${REACTOS_SOURCE_DIR}/sdk/lib/tdilib)

spec2def(iphlpapibase.dll iphlpapibase.spec ADD_IMPORTLIB)

add_definitions(-DGetAdaptersAddressesV2)
list(APPEND SOURCE
    address.c
    dhcp_reactos.c
    icmp.c
    ifenum_reactos.c
    ipstats_reactos.c
    iphlpapi_reactos.c
    iphlpapi_main.c
    media.c
    registry.c
    resinfo_reactos.c
    route_reactos.c
	unimplemented.c
    iphlpapi_private.h
    ${CMAKE_CURRENT_BINARY_DIR}/iphlpapibase_stubs.c)

add_library(iphlpapibase SHARED
    ${SOURCE}
    iphlpapibase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/iphlpapibase.def)

set_module_type(iphlpapibase win32dll UNICODE)
target_link_libraries(iphlpapibase wine tdilib)
add_importlibs(iphlpapibase dhcpcsvc advapi32 ws2_32 msvcrt kernel32 ntdll)
add_pch(iphlpapibase iphlpapi_private.h SOURCE)
add_cd_file(TARGET iphlpapibase DESTINATION reactos/system32 FOR all)
