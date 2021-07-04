
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

spec2def(npmshtml.dll npmshtml.spec)

list(APPEND SOURCE
    main.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/npmshtml_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/npmshtml.def)
	

add_library(npmshtml SHARED ${SOURCE})
set_module_type(npmshtml win32dll)
target_link_libraries(npmshtml wine)
add_importlibs(npmshtml msvcrt kernel32 ntdll mshtml)
add_cd_file(TARGET npmshtml DESTINATION reactos/system32 FOR all)
