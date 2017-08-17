
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(securnew.dll securnew.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/securnew_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/securnew.def)

add_library(securnew SHARED ${SOURCE})
set_module_type(securnew win32dll)
target_link_libraries(securnew wine)
add_importlibs(securnew advapi32 msvcrt kernel32 ntdll securbase)
add_cd_file(TARGET securnew DESTINATION reactos/system32 FOR all)
