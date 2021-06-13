
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(connect.dll connect.spec)

list(APPEND SOURCE
    dllmain.c	
    main.c	
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/connect_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/connect.def)

add_library(connect SHARED ${SOURCE})
set_module_type(connect win32dll)
target_link_libraries(connect wine)
add_importlibs(connect advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET connect DESTINATION reactos/system32 FOR all)
