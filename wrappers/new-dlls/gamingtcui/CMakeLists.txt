
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

spec2def(gamingtcui.dll gamingtcui.spec)

list(APPEND SOURCE
    gamingtcui_main.c
    ${CMAKE_CURRENT_BINARY_DIR}/gamingtcui_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/gamingtcui.def)
	

add_library(gamingtcui SHARED ${SOURCE})
set_module_type(gamingtcui win32dll)
target_link_libraries(gamingtcui wine)
add_importlibs(gamingtcui msvcrt kernel32 ntdll)
add_cd_file(TARGET gamingtcui DESTINATION reactos/system32 FOR all)
