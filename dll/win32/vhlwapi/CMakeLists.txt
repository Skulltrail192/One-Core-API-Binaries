
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(vhlwapi.dll vhlwapi.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vhlwapi_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vhlwapi.def)

add_library(vhlwapi SHARED ${SOURCE})
set_module_type(vhlwapi win32dll)
target_link_libraries(vhlwapi wine)
add_importlibs(vhlwapi advapi32 msvcrt kernel32 ntdll shlwapibase)
add_cd_file(TARGET vhlwapi DESTINATION reactos/system32 FOR all)
