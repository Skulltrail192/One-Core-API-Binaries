
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)

add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(pdhbase.dll pdhbase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    pdh_main.c
    ${CMAKE_CURRENT_BINARY_DIR}/pdhbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/pdhbase.def)

add_library(pdhbase SHARED ${SOURCE})
set_module_type(pdhbase win32dll)
target_link_libraries(pdhbase wine)
add_importlibs(pdhbase msvcrt kernel32_vista kernel32 ntdll)
add_cd_file(TARGET pdhbase DESTINATION reactos/system32 FOR all)
