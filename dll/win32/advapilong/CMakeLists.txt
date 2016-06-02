
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(advapilong.dll advapilong.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/advapilong_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/advapilong.def)

add_library(advapilong SHARED ${SOURCE})
set_module_type(advapilong win32dll)
target_link_libraries(advapilong wine)
add_importlibs(advapilong advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET advapilong DESTINATION reactos/system32 FOR all)
