
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(wevtapibase.dll wevtapibase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/wevtapibase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/wevtapibase.def)

add_library(wevtapibase SHARED ${SOURCE})
set_module_type(wevtapibase win32dll)
target_link_libraries(wevtapibase wine)
add_importlibs(wevtapibase advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET wevtapibase DESTINATION reactos/system32 FOR all)
