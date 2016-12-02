
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)

add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(vdh.dll vdh.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    pdh_main.c
	vdh.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vdh_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vdh.def)

add_library(vdh SHARED ${SOURCE})
set_module_type(vdh win32dll)
target_link_libraries(vdh wine)
add_importlibs(vdh pdhbase msvcrt kernel32 ntdll)
add_cd_file(TARGET vdh DESTINATION reactos/system32 FOR all)
