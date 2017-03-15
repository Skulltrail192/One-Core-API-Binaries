
include_directories(${REACTOS_SOURCE_DIR}/include/host)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/win32ss/user/winsrv/usersrv)

spec2def(vbrowseui.dll vbrowseui.spec)

list(APPEND SOURCE
	main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vbrowseui_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vbrowseui.def)

add_library(vbrowseui SHARED ${SOURCE})
set_module_type(vbrowseui win32dll ENTRYPOINT 0)

target_link_libraries(vbrowseui wine)

add_importlibs(vbrowseui browsebase kernel32 gdi32 ntdll advapi32)

add_cd_file(TARGET vbrowseui DESTINATION reactos/system32 FOR all)
