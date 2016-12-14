
spec2def(ntvdm64.dll ntvdm64.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    ntvdm64.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ntvdm64_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ntvdm64.def)

add_library(ntvdm64 SHARED ${SOURCE})

set_module_type(ntvdm64 win32dll ENTRYPOINT 0)
add_importlibs(ntvdm64 kernel32 ntdll)
add_dependencies(ntvdm64 psdk)
add_cd_file(TARGET ntvdm64 DESTINATION reactos/system32 FOR all)
