
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
add_definitions(-D__WINESRC__)
spec2def(uxthemebase.dll uxthemebase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    buffer.c
    draw.c
    main.c
    metric.c
    msstyles.c
    ncscrollbar.c
    nonclient.c
    property.c
    stylemap.c
    system.c
    themehooks.c
	unimplemented.c
    uxini.c
    uxthemep.h
    ${CMAKE_CURRENT_BINARY_DIR}/uxthemebase_stubs.c)

add_library(uxthemebase SHARED
    ${SOURCE}
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/uxthemebase.def)

set_module_type(uxthemebase win32dll)
target_link_libraries(uxthemebase wine)
add_delay_importlibs(uxthemebase msimg32)
add_importlibs(uxthemebase user32 advapi32 gdi32 msvcrt kernel32 ntdll)
add_pch(uxthemebase uxthemep.h SOURCE)
add_cd_file(TARGET uxthemebase DESTINATION reactos/system32 FOR all)
