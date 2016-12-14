
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)

spec2def(nlsdl.dll nlsdl.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/nlsdl_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/nlsdl.def)

add_library(nlsdl SHARED ${SOURCE})
set_module_type(nlsdl win32dll)
target_link_libraries(nlsdl wine)
add_importlibs(nlsdl advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET nlsdl DESTINATION reactos/system32 FOR all)
