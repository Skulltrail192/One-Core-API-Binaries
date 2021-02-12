
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

spec2def(fntcache.dll fntcache.spec)

list(APPEND SOURCE
    main.c
    ${CMAKE_CURRENT_BINARY_DIR}/fntcache_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/fntcache.def)

add_library(fntcache SHARED ${SOURCE} version.rc)
set_module_type(fntcache win32dll)
target_link_libraries(fntcache wine)
add_importlibs(fntcache msvcrt advapi32 kernel32 ntdll)
add_cd_file(TARGET fntcache DESTINATION reactos/system32 FOR all)
