
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(v3d9.dll v3d9.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/v3d9_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/v3d9.def)

add_library(v3d9 SHARED ${SOURCE})
set_module_type(v3d9 win32dll)
target_link_libraries(v3d9 wine)
add_importlibs(v3d9 advapi32 msvcrt kernel32 d3d9base ntdll)
add_cd_file(TARGET v3d9 DESTINATION reactos/system32 FOR all)
