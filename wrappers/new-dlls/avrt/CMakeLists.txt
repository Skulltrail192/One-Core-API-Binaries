
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(avrt.dll avrt.spec)

list(APPEND SOURCE
    main.c	
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/avrt_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/avrt.def)

add_library(avrt SHARED ${SOURCE})
set_module_type(avrt win32dll)
target_link_libraries(avrt wine)
add_importlibs(avrt advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET avrt DESTINATION reactos/system32 FOR all)
