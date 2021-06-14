
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)

spec2def(windows.gaming.input.dll windows.gaming.input.spec)

list(APPEND SOURCE
    main.c
    ${CMAKE_CURRENT_BINARY_DIR}/windows.gaming.input_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/windows.gaming.input.def)
	
add_idl_headers(windows.gaming.input_idlheader classes.idl)	

add_library(windows.gaming.input SHARED ${SOURCE} version.rc)
set_module_type(windows.gaming.input win32dll)
add_dependencies(windows.gaming.input windows.gaming.input_idlheader)
target_link_libraries(windows.gaming.input uuid wine)
add_importlibs(windows.gaming.input msvcrt kernel32 ntdll combase)
add_cd_file(TARGET windows.gaming.input DESTINATION reactos/system32 FOR all)
