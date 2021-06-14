
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)

spec2def(windows.globalization.dll windows.globalization.spec)

list(APPEND SOURCE
    main.c
    ${CMAKE_CURRENT_BINARY_DIR}/windows.globalization_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/windows.globalization.def)
	
add_idl_headers(windows.globalization_idlheader classes.idl)	

add_library(windows.globalization SHARED ${SOURCE} version.rc)
set_module_type(windows.globalization win32dll)
add_dependencies(windows.globalization windows.globalization_idlheader)
target_link_libraries(windows.globalization uuid wine)
add_importlibs(windows.globalization msvcrt kernel32 ntdll combase)
add_cd_file(TARGET windows.globalization DESTINATION reactos/system32 FOR all)
