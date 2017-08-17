
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(wdslegacy.dll wdslegacy.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/wdslegacy_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/wdslegacy.def)

add_library(wdslegacy SHARED ${SOURCE})
set_module_type(wdslegacy win32dll)
target_link_libraries(wdslegacy wine)
add_importlibs(wdslegacy advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET wdslegacy DESTINATION reactos/system32 FOR all)
