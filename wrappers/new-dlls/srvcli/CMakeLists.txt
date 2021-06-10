
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)

spec2def(srvcli.dll srvcli.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/srvcli_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/srvcli.def)
	
add_library(srvcli SHARED ${SOURCE})
set_module_type(srvcli win32dll)
target_link_libraries(srvcli wine uuid)
add_importlibs(srvcli netapi32 ntdll kernel32 msvcrt)
add_cd_file(TARGET srvcli DESTINATION reactos/system32 FOR all)
