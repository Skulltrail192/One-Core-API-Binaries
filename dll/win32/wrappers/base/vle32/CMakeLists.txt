
spec2def(vle32.dll vle32.spec)

list(APPEND SOURCE
    main.c
	socket.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vle32_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vle32.def)

add_library(vle32 SHARED ${SOURCE})
set_module_type(vle32 win32dll ENTRYPOINT 0)
target_link_libraries(vle32 wine)
add_importlibs(vle32 ole32 msvcrt kernel32 ntdll)
add_cd_file(TARGET vle32 DESTINATION reactos/system32 FOR all)
