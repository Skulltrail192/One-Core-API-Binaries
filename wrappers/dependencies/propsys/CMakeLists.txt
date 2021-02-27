
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
add_definitions(-D__WINESRC__)
spec2def(propsys_new.dll propsys_new.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    propstore.c
    propsys_main.c
    propvar.c
	unimplemented.c
    propsys_private.h
    ${CMAKE_CURRENT_BINARY_DIR}/propsys_new_stubs.c)

add_library(propsys_new SHARED
    ${SOURCE}
    propsys_new.rc
    ${CMAKE_CURRENT_BINARY_DIR}/propsys_new.def)

set_module_type(propsys_new win32dll)
target_link_libraries(propsys_new uuid wine)
add_importlibs(propsys_new ole32 oleaut32 msvcrt kernel32 ntdll)
add_pch(propsys_new propsys_new_private.h SOURCE)
add_cd_file(TARGET propsys_new DESTINATION reactos/system32 FOR all)
