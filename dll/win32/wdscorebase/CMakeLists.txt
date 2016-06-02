
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(wdscorebase.dll wdscorebase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/wdscorebase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/wdscorebase.def)

add_library(wdscorebase SHARED ${SOURCE})
set_module_type(wdscorebase win32dll)
target_link_libraries(wdscorebase wine)
add_importlibs(wdscorebase advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET wdscorebase DESTINATION reactos/system32 FOR all)
