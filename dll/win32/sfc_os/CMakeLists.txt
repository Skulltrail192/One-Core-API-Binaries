
spec2def(sfc_os.dll sfc_os.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    sfc_os.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/sfc_os_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/sfc_os.def)

add_library(sfc_os SHARED ${SOURCE})

set_module_type(sfc_os win32dll ENTRYPOINT 0)
add_importlibs(sfc_os kernel32 ntdll)

add_cd_file(TARGET sfc_os DESTINATION reactos/system32 FOR all)
