
add_definitions(
    -D__WINESRC__
    -D_SHLWAPIBASE_)

include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(shlwapibase.dll shlwapibase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    assoc.c
    clist.c
    istream.c
    msgbox.c
    ordinal.c
    path.c
    reg.c
    regstream.c
    shlwapibase_main.c
    stopwatch.c
    string.c
    thread.c
    url.c
    precomp.h)

add_library(shlwapibase SHARED
    ${SOURCE}
    shlwapibase.rc
    wsprintf.c
    ${CMAKE_CURRENT_BINARY_DIR}/shlwapibase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/shlwapibase.def)

set_module_type(shlwapibase win32dll)
target_link_libraries(shlwapibase uuid wine)
add_delay_importlibs(shlwapibase userenv oleaut32 ole32 comdlg32 mpr mlang urlmon shell32 winmm version)
add_importlibs(shlwapibase user32 gdi32 advapi32 wininet msvcrt kernel32 ntdll)
add_pch(shlwapibase precomp.h SOURCE)
add_cd_file(TARGET shlwapibase DESTINATION reactos/system32 FOR all)
