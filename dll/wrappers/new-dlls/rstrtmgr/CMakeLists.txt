
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(rstrtmgr.dll rstrtmgr.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/rstrtmgr_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/rstrtmgr.def)

add_library(rstrtmgr SHARED ${SOURCE})
set_module_type(rstrtmgr win32dll)
target_link_libraries(rstrtmgr wine)
add_importlibs(rstrtmgr advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET rstrtmgr DESTINATION reactos/system32 FOR all)
