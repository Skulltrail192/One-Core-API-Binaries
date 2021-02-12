
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(wer.dll wer.spec)

list(APPEND SOURCE
    main.c	
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/wer_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/wer.def)

add_library(wer SHARED ${SOURCE})
set_module_type(wer win32dll)
target_link_libraries(wer wine)
add_importlibs(wer advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET wer DESTINATION reactos/system32 FOR all)
