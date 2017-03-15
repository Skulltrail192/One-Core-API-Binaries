
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(vecur32.dll vecur32.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vecur32_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vecur32.def)

add_library(vecur32 SHARED ${SOURCE})
set_module_type(vecur32 win32dll)
target_link_libraries(vecur32 wine)
add_importlibs(vecur32 advapi32 msvcrt kernel32 ntdll securbase securnew)
add_cd_file(TARGET vecur32 DESTINATION reactos/system32 FOR all)
