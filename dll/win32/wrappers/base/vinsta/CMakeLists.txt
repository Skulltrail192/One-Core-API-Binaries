
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(vinsta.dll vinsta.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vinsta_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vinsta.def)

add_library(vinsta SHARED ${SOURCE})
set_module_type(vinsta win32dll)
target_link_libraries(vinsta wine)
add_importlibs(vinsta advapi32 msvcrt kernel32 ntdll winstabase winstanew)
add_cd_file(TARGET vinsta DESTINATION reactos/system32 FOR all)
