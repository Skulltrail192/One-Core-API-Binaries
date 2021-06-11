
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(xpssvcs.dll xpssvcs.spec)

list(APPEND SOURCE
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/xpssvcs_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/xpssvcs.def)

add_library(xpssvcs SHARED ${SOURCE})
set_module_type(xpssvcs win32dll)
target_link_libraries(xpssvcs wine)
add_importlibs(xpssvcs advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET xpssvcs DESTINATION reactos/system32 FOR all)
