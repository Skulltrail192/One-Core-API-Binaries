
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(ninput.dll ninput.spec)

list(APPEND SOURCE
    main.c	
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ninput_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ninput.def)

add_library(ninput SHARED ${SOURCE})
set_module_type(ninput win32dll)
target_link_libraries(ninput wine)
add_importlibs(ninput advapi32 bcrypt msvcrt kernel32 ntdll)
add_cd_file(TARGET ninput DESTINATION reactos/system32 FOR all)
