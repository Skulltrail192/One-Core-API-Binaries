
spec2def(vs2_32.dll vs2_32.spec)

list(APPEND SOURCE
    main.c
	socket.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vs2_32_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vs2_32.def)

add_library(vs2_32 SHARED ${SOURCE})
set_module_type(vs2_32 win32dll ENTRYPOINT 0)
target_link_libraries(vs2_32 wine)
add_importlibs(vs2_32 ws2_base msvcrt kernel32 ntdll)
add_cd_file(TARGET vs2_32 DESTINATION reactos/system32 FOR all)
