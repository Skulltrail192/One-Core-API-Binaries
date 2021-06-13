
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(ncrypt.dll ncrypt.spec)

list(APPEND SOURCE
    main.c	
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ncrypt_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ncrypt.def)

add_library(ncrypt SHARED ${SOURCE})
set_module_type(ncrypt win32dll)
target_link_libraries(ncrypt wine)
add_importlibs(ncrypt advapi32 bcrypt msvcrt kernel32 ntdll)
add_cd_file(TARGET ncrypt DESTINATION reactos/system32 FOR all)
