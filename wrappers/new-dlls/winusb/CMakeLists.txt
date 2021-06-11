
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(winusb.dll winusb.spec)

list(APPEND SOURCE
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/winusb_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/winusb.def)

add_library(winusb SHARED ${SOURCE})
set_module_type(winusb win32dll)
target_link_libraries(winusb wine)
add_importlibs(winusb advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET winusb DESTINATION reactos/system32 FOR all)
