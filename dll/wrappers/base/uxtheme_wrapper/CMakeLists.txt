
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(uxtheme_wrapper.dll uxtheme_wrapper.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/uxtheme_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/uxtheme_wrapper.def)

add_library(uxtheme_wrapper SHARED ${SOURCE})
set_module_type(uxtheme_wrapper win32dll)
target_link_libraries(uxtheme_wrapper wine)
add_importlibs(uxtheme_wrapper user32 advapi32 gdi32 msvcrt kernel32 ntdll uxthemebase)
add_cd_file(TARGET uxtheme_wrapper DESTINATION reactos/system32 FOR all)
