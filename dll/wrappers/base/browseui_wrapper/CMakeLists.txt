
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/host)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/win32ss/user/winsrv/usersrv)

spec2def(browseui_wrapper.dll browseui_wrapper.spec)

list(APPEND SOURCE
	main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/browseui_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/browseui_wrapper.def)

add_library(browseui_wrapper SHARED ${SOURCE})
set_module_type(browseui_wrapper win32dll ENTRYPOINT 0)

target_link_libraries(browseui_wrapper wine)

add_importlibs(browseui_wrapper browsebase kernel32 gdi32 ntdll advapi32)

add_cd_file(TARGET browseui_wrapper DESTINATION reactos/system32 FOR all)
