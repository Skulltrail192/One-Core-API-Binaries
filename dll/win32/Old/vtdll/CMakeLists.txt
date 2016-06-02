
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)

spec2def(vtdll.dll vtdll.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	etw.c
	locale.c
    main.c
	sqm.c
	sync.c
	transaction.c
	unimplemented.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vtdll_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vtdll.def)

add_library(vtdll SHARED ${SOURCE})
set_module_type(vtdll win32dll)

target_link_libraries(vtdll wine)

add_importlibs(vtdll msvcrt kernel32 ntdll ntdllnew)

add_cd_file(TARGET vtdll DESTINATION reactos/system32 FOR all)
