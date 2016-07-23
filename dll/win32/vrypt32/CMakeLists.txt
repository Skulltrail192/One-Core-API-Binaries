
add_definitions(
    -D__WINESRC__
    -D_WINE
    -D_CRYPT32_)
include_directories(${REACTOS_SOURCE_DIR}/include/host)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/win32ss/user/winsrv/usersrv)

spec2def(vrypt32.dll vrypt32.spec)

list(APPEND SOURCE
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vrypt32_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vrypt32.def)

add_library(vrypt32 SHARED ${SOURCE})
set_module_type(vrypt32 win32dll ENTRYPOINT 0)

target_link_libraries(vrypt32 wine)

add_importlibs(vrypt32 cryptfull kernel32 gdi32 ntdll advapi32)

add_cd_file(TARGET vrypt32 DESTINATION reactos/system32 FOR all)
