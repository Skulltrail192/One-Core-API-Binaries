
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(d3dkmt.dll d3dkmt.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/d3dkmt_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/d3dkmt.def)

add_library(d3dkmt SHARED ${SOURCE})
set_module_type(d3dkmt win32dll ENTRYPOINT 0)
target_link_libraries(d3dkmt wine)
add_importlibs(d3dkmt advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET d3dkmt DESTINATION reactos/system32 FOR all)
