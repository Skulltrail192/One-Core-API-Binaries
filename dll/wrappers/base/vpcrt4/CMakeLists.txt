
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(vpcrt4.dll vpcrt4.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vpcrt4_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vpcrt4.def)

add_library(vpcrt4 SHARED ${SOURCE})
set_module_type(vpcrt4 win32dll)
target_link_libraries(vpcrt4 wine)
add_importlibs(vpcrt4 rpcrtbase msvcrt kernel32 ntdll)
add_cd_file(TARGET vpcrt4 DESTINATION reactos/system32 FOR all)
