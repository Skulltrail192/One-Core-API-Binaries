
spec2def(winstabase.dll winstabase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    logon.c
    main.c
    misc.c
    query.c
    security.c
    server.c
    ws.c
    winstabase.h)

add_library(winstabase SHARED
    ${SOURCE}
    winstabase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/winstabase.def)

set_module_type(winstabase win32dll)
target_link_libraries(winstabase wine)
add_importlibs(winstabase msvcrt kernel32 ntdll)
add_pch(winstabase winstabase.h SOURCE)
add_cd_file(TARGET winstabase DESTINATION reactos/system32 FOR all)
