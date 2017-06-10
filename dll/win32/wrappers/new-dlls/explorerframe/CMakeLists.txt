
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)

spec2def(explorerframe.dll explorerframe.spec)

list(APPEND SOURCE
    explorerframe_main.c
	nstc.c
	taskbarlist.c
    ${CMAKE_CURRENT_BINARY_DIR}/explorerframe_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/explorerframe.def)

add_library(explorerframe SHARED ${SOURCE} version.rc)
set_module_type(explorerframe win32dll)
target_link_libraries(explorerframe uuid wine)
add_importlibs(explorerframe msvcrt kernel32 ntdll ole32 comctl32 shell32 user32)
add_cd_file(TARGET explorerframe DESTINATION reactos/system32 FOR all)
