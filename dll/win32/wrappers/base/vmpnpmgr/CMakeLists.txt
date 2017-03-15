
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(vmpnpmgr.dll vmpnpmgr.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vmpnpmgr_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vmpnpmgr.def)

add_library(vmpnpmgr SHARED ${SOURCE})
set_module_type(vmpnpmgr win32dll)
target_link_libraries(vmpnpmgr wine)
add_importlibs(vmpnpmgr advapi32 msvcrt kernel32 ntdll umpnpbase)
add_cd_file(TARGET vmpnpmgr DESTINATION reactos/system32 FOR all)
