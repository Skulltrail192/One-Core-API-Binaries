
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(windowscodecs_wrapper.dll windowscodecs_wrapper.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/windowscodecs_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/windowscodecs_wrapper.def)

add_library(windowscodecs_wrapper SHARED ${SOURCE})
set_module_type(windowscodecs_wrapper win32dll)
target_link_libraries(windowscodecs_wrapper wine)
add_importlibs(windowscodecs_wrapper advapi32 msvcrt kernel32 ntdll windowscodecsbase)
add_cd_file(TARGET windowscodecs_wrapper DESTINATION reactos/system32 FOR all)
