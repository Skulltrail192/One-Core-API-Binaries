
add_definitions(-D__securbase__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)

spec2def(securbase.dll securbase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    dllmain.c
    lsalpc.c
    secext.c
    sspi.c
    stubs.c
    thunks.c
	unimplemented.c
    wrapper.c
    precomp.h)

add_library(securbase SHARED
    ${SOURCE}
    securbase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/securbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/securbase.def)

set_module_type(securbase win32dll UNICODE)

target_link_libraries(securbase lsalib wine)

add_importlibs(securbase advapi32 msvcrt kernel32 ntdll)
add_pch(securbase precomp.h SOURCE)

add_cd_file(TARGET securbase DESTINATION reactos/system32 FOR all)

