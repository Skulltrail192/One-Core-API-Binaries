
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(vetapi32.dll vetapi32.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vetapi32_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vetapi32.def)

add_library(vetapi32 SHARED ${SOURCE})
set_module_type(vetapi32 win32dll)
target_link_libraries(vetapi32 wine)
add_importlibs(vetapi32 advapi32 msvcrt kernel32 netapibase netapinew ntdll)
add_cd_file(TARGET vetapi32 DESTINATION reactos/system32 FOR all)
