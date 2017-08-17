
spec2def(cryptbasex.dll cryptbasex.spec)

list(APPEND SOURCE
    cryptbasex.rc
    ${CMAKE_CURRENT_BINARY_DIR}/cryptbasex_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/cryptbasex.def)

add_library(cryptbasex SHARED ${SOURCE})

set_module_type(cryptbasex win32dll)
add_importlibs(cryptbasex msvcrt advapi32 kernel32 ntdll)
add_dependencies(cryptbasex psdk)
add_cd_file(TARGET cryptbasex DESTINATION reactos/system32 FOR all)
