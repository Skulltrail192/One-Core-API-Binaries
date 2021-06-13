
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(bluetoothapis.dll bluetoothapis.spec)

list(APPEND SOURCE
	dllmain.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/bluetoothapis_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/bluetoothapis.def)

add_library(bluetoothapis SHARED ${SOURCE})
set_module_type(bluetoothapis win32dll)
target_link_libraries(bluetoothapis wine)
add_importlibs(bluetoothapis advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET bluetoothapis DESTINATION reactos/system32 FOR all)
