
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(sspicli.dll sspicli.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/sspicli_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/sspicli.def)

add_library(sspicli SHARED ${SOURCE})
set_module_type(sspicli win32dll)
target_link_libraries(sspicli wine)
add_importlibs(sspicli advapi32 secur32 msvcrt kernel32 ntext)
add_cd_file(TARGET sspicli DESTINATION reactos/system32 FOR all)
