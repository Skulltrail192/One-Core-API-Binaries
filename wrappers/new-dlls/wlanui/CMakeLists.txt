
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(wlanui.dll wlanui.spec)

list(APPEND SOURCE
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/wlanui_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/wlanui.def)

add_library(wlanui SHARED ${SOURCE})
set_module_type(wlanui win32dll)
target_link_libraries(wlanui wine)
add_importlibs(wlanui advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET wlanui DESTINATION reactos/system32 FOR all)
