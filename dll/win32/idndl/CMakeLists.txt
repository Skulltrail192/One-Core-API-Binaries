
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(idndl.dll idndl.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/idndl_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/idndl.def)

add_library(idndl SHARED ${SOURCE})
set_module_type(idndl win32dll)
target_link_libraries(idndl wine)
add_importlibs(idndl advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET idndl DESTINATION reactos/system32 FOR all)
