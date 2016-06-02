
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(normaliz.dll normaliz.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/normaliz_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/normaliz.def)

add_library(normaliz SHARED ${SOURCE})
set_module_type(normaliz win32dll)
target_link_libraries(normaliz wine)
add_importlibs(normaliz advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET normaliz DESTINATION reactos/system32 FOR all)
