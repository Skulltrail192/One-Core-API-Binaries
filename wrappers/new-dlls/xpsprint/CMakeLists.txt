
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(xpsprint.dll xpsprint.spec)

list(APPEND SOURCE
	main.c
    xpsprint.rc
    ${CMAKE_CURRENT_BINARY_DIR}/xpsprint_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/xpsprint.def)

add_library(xpsprint SHARED ${SOURCE})
set_module_type(xpsprint win32dll)
target_link_libraries(xpsprint wine)
add_importlibs(xpsprint advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET xpsprint DESTINATION reactos/system32 FOR all)
