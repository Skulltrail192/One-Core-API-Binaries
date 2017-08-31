
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(fileextd.dll fileextd.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/fileextd_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/fileextd.def)

add_library(fileextd SHARED ${SOURCE})
set_module_type(fileextd win32dll)
target_link_libraries(fileextd wine)
add_importlibs(fileextd advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET fileextd DESTINATION reactos/system32 FOR all)
