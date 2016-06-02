
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)

spec2def(vtdllwow.dll vtdllwow.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	etw.c
	info.c
	locale.c
    main.c
	sqm.c
	sync.c
	transaction.c
	unimplemented.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vtdllwow_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vtdllwow.def)

add_library(vtdllwow SHARED ${SOURCE})
set_module_type(vtdllwow win32dll)

target_link_libraries(vtdllwow wine)

add_importlibs(vtdllwow msvcrt kernel32 ntdll ntdllnew)

add_cd_file(TARGET vtdllwow DESTINATION reactos/system32 FOR all)
