
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(pwrshplugin.dll pwrshplugin.spec)

list(APPEND SOURCE
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/pwrshplugin_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/pwrshplugin.def)

add_library(pwrshplugin SHARED ${SOURCE})
set_module_type(pwrshplugin win32dll)
target_link_libraries(pwrshplugin wine)
add_importlibs(pwrshplugin advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET pwrshplugin DESTINATION reactos/system32 FOR all)
