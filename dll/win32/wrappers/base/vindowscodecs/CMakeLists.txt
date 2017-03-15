
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(vindowscodecs.dll vindowscodecs.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vindowscodecs_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vindowscodecs.def)

add_library(vindowscodecs SHARED ${SOURCE})
set_module_type(vindowscodecs win32dll)
target_link_libraries(vindowscodecs wine)
add_importlibs(vindowscodecs advapi32 msvcrt kernel32 ntdll windowscodecsbase)
add_cd_file(TARGET vindowscodecs DESTINATION reactos/system32 FOR all)
