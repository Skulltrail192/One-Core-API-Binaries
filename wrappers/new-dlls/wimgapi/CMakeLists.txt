
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(wimgapi.dll wimgapi.spec)

list(APPEND SOURCE
    main.c	
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/wimgapi_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/wimgapi.def)

add_library(wimgapi SHARED ${SOURCE})
set_module_type(wimgapi win32dll)
target_link_libraries(wimgapi wine)
add_importlibs(wimgapi advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET wimgapi DESTINATION reactos/system32 FOR all)
