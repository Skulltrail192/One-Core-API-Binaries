
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(vxtheme.dll vxtheme.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vxtheme_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vxtheme.def)

add_library(vxtheme SHARED ${SOURCE})
set_module_type(vxtheme win32dll)
target_link_libraries(vxtheme wine)
add_importlibs(vxtheme user32 advapi32 gdi32 msvcrt kernel32 ntdll uxthemebase)
add_cd_file(TARGET vxtheme DESTINATION reactos/system32 FOR all)
