
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(umpnpmgr_wrapper.dll umpnpmgr_wrapper.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/umpnpmgr_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/umpnpmgr_wrapper.def)

add_library(umpnpmgr_wrapper SHARED ${SOURCE})
set_module_type(umpnpmgr_wrapper win32dll)
target_link_libraries(umpnpmgr_wrapper wine)
add_importlibs(umpnpmgr_wrapper advapi32 msvcrt kernel32 ntdll umpnpbase)
add_cd_file(TARGET umpnpmgr_wrapper DESTINATION reactos/system32 FOR all)
