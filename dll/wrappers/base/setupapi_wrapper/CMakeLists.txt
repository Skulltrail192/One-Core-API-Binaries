
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(setupapi_wrapper.dll setupapi_wrapper.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/setupapi_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/setupapi_wrapper.def)

add_library(setupapi_wrapper SHARED ${SOURCE})
set_module_type(setupapi_wrapper win32dll ENTRYPOINT 0)
target_link_libraries(setupapi_wrapper wine)
add_importlibs(setupapi_wrapper setupapibase advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET setupapi_wrapper DESTINATION reactos/system32 FOR all)
