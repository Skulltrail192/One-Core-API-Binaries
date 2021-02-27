add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(bcrypt_new.dll bcrypt_new.spec)

list(APPEND SOURCE
    bcrypt_main.c
	md2.c
	sha256.c
	sha512.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/bcrypt_new_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/bcrypt_new.def)

add_library(bcrypt_new SHARED ${SOURCE})
set_module_type(bcrypt_new win32dll)
target_link_libraries(bcrypt_new wine)
add_importlibs(bcrypt_new advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET bcrypt_new DESTINATION reactos/system32 FOR all)