
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(wdscore.dll wdscore.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/wdscore_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/wdscore.def)

add_library(wdscore SHARED ${SOURCE})
set_module_type(wdscore win32dll)
target_link_libraries(wdscore wine)
add_importlibs(wdscore advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET wdscore DESTINATION reactos/system32 FOR all)
