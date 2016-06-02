
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(comctlnew.dll comctlnew.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	dpa.c
	dsa.c
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/comctlnew_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/comctlnew.def)

add_library(comctlnew SHARED ${SOURCE})
set_module_type(comctlnew win32dll)
target_link_libraries(comctlnew wine)
add_importlibs(comctlnew msvcrt kernel32 ntdll comctlbase user32 gdi32)
add_cd_file(TARGET comctlnew DESTINATION reactos/system32 FOR all)
