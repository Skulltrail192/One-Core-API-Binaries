
spec2def(ws2_32_wrapper.dll ws2_32_wrapper.spec)

include_directories(${REACTOS_SOURCE_DIR}/sdk/include/wine)

list(APPEND SOURCE
    main.c
	socket.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ws2_32_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ws2_32_wrapper.def)

add_library(ws2_32_wrapper SHARED ${SOURCE})
set_module_type(ws2_32_wrapper win32dll ENTRYPOINT 0)
target_link_libraries(ws2_32_wrapper wine)
add_importlibs(ws2_32_wrapper ws2_base msvcrt kernelbase ntdll)
add_cd_file(TARGET ws2_32_wrapper DESTINATION reactos/system32 FOR all)
