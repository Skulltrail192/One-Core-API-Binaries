
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(wevtsvc.dll wevtsvc.spec)

list(APPEND SOURCE
    wevtsvc.c	
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/wevtsvc_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/wevtsvc.def)

add_library(wevtsvc SHARED ${SOURCE})
set_module_type(wevtsvc win32dll)
target_link_libraries(wevtsvc wine)
add_importlibs(wevtsvc advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET wevtsvc DESTINATION reactos/system32 FOR all)
