
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(secbase.dll secbase.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/secbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/secbase.def)

add_library(secbase SHARED ${SOURCE})
set_module_type(secbase win32dll)
target_link_libraries(secbase wine)
add_importlibs(secbase advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET secbase DESTINATION reactos/system32 FOR all)
