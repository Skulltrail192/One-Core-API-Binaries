
include_directories(${REACTOS_SOURCE_DIR}/include/host)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/win32ss/user/winsrv/usersrv)

spec2def(vbrowse.dll vbrowse.spec)

list(APPEND SOURCE
	main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vbrowse_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vbrowse.def)

add_library(vbrowse SHARED ${SOURCE})
set_module_type(vbrowse win32dll ENTRYPOINT 0)

target_link_libraries(vbrowse wine)

add_importlibs(vbrowse browsebase kernel32 gdi32 ntdll advapi32)

add_cd_file(TARGET vbrowse DESTINATION reactos/system32 FOR all)
