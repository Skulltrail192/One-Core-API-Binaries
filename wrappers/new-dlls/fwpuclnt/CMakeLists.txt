
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

spec2def(fwpuclnt.dll fwpuclnt.spec)

list(APPEND SOURCE
    main.c
    ${CMAKE_CURRENT_BINARY_DIR}/fwpuclnt_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/fwpuclnt.def)

add_library(fwpuclnt SHARED ${SOURCE} version.rc)
set_module_type(fwpuclnt win32dll)
target_link_libraries(fwpuclnt wine)
add_importlibs(fwpuclnt msvcrt advapi32 kernel32 ntdll)
add_cd_file(TARGET fwpuclnt DESTINATION reactos/system32 FOR all)
