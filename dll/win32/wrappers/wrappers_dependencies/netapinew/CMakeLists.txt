
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(netapinew.dll netapinew.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/netapinew_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/netapinew.def)

add_library(netapinew SHARED ${SOURCE})
set_module_type(netapinew win32dll)
target_link_libraries(netapinew wine)
add_importlibs(netapinew advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET netapinew DESTINATION reactos/system32 FOR all)
