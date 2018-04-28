
add_definitions(-D_ADVAPI32_)

remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/lib/rtl)

spec2def(d3d9_wrapper.dll d3d9_wrapper.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/d3d9_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/d3d9_wrapper.def)

add_library(d3d9_wrapper SHARED ${SOURCE} guid.c)
set_module_type(d3d9_wrapper win32dll ENTRYPOINT 0)

target_link_libraries(d3d9_wrapper wine)

add_importlibs(d3d9_wrapper d3d9 ntdll kernel32)

add_cd_file(TARGET d3d9_wrapper DESTINATION reactos/system32 FOR all)
