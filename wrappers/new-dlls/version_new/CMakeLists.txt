
add_definitions(-D__WINESRC__)
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(version_new.dll version_new.spec)

list(APPEND SOURCE
    version_new.c
    ${CMAKE_CURRENT_BINARY_DIR}/version_new.def)

add_library(version_new MODULE ${SOURCE} version_new.rc)
set_module_type(version_new win32dll)
target_link_libraries(version_new wine)
add_importlibs(version_new msvcrt kernel32 ntdll)
add_cd_file(TARGET version_new DESTINATION reactos/system32 FOR all)
