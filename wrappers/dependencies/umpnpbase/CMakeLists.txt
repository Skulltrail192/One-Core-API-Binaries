
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(umpnpbase.dll umpnpbase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/umpnpbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/umpnpbase.def)

add_library(umpnpbase SHARED ${SOURCE})
set_module_type(umpnpbase win32dll)
target_link_libraries(umpnpbase wine)
add_importlibs(umpnpbase advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET umpnpbase DESTINATION reactos/system32 FOR all)
