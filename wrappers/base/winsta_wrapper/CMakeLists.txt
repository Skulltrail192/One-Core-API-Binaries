
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(winsta_wrapper.dll winsta_wrapper.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/winsta_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/winsta_wrapper.def)

add_library(winsta_wrapper SHARED ${SOURCE})
set_module_type(winsta_wrapper win32dll)
target_link_libraries(winsta_wrapper wine)
add_importlibs(winsta_wrapper advapi32 msvcrt kernel32 ntdll winstabase winstanew)
add_cd_file(TARGET winsta_wrapper DESTINATION reactos/system32 FOR all)
