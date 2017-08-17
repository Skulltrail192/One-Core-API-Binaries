
spec2def(userenvbase.dll userenvbase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    desktop.c
    directory.c
    environment.c
    gpolicy.c
    misc.c
    profile.c
    registry.c
    setup.c
	unimplemented.c
    userenvbase.c
    precomp.h)

add_library(userenvbase SHARED
    ${SOURCE}
    userenvbase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/userenvbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/userenvbase.def)

set_module_type(userenvbase win32dll)
target_link_libraries(userenvbase uuid)
add_importlibs(userenvbase advapi32 user32 msvcrt kernel32 ntdll)
add_pch(userenvbase precomp.h SOURCE)
add_cd_file(TARGET userenvbase DESTINATION reactos/system32 FOR all)
