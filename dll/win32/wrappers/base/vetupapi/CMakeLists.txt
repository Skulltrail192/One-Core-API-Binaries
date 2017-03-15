
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(vetupapi.dll vetupapi.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vetupapi_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vetupapi.def)

add_library(vetupapi SHARED ${SOURCE})
set_module_type(vetupapi win32dll ENTRYPOINT 0)
target_link_libraries(vetupapi wine)
add_importlibs(vetupapi setupapibase advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET vetupapi DESTINATION reactos/system32 FOR all)
