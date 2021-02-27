
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(shlwapi_wrapper.dll shlwapi_wrapper.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/shlwapi_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/shlwapi_wrapper.def)

add_library(shlwapi_wrapper SHARED ${SOURCE})
set_module_type(shlwapi_wrapper win32dll)
target_link_libraries(shlwapi_wrapper wine)
add_importlibs(shlwapi_wrapper shlwapibase shlwnew advapi32 msvcrt kernel32 user32 ole32 gdi32 ntdll userenv wininet propsys)
add_cd_file(TARGET shlwapi_wrapper DESTINATION reactos/system32 FOR all)
