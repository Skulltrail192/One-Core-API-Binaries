
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(secur32_wrapper.dll secur32_wrapper.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/secur32_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/secur32_wrapper.def)

add_library(secur32_wrapper SHARED ${SOURCE})
set_module_type(secur32_wrapper win32dll)
target_link_libraries(secur32_wrapper wine)
add_importlibs(secur32_wrapper advapi32 msvcrt kernel32 ntdll securbase sspicli securnew)
add_delay_importlibs(secur32_wrapper sspicli)
add_cd_file(TARGET secur32_wrapper DESTINATION reactos/system32 FOR all)
