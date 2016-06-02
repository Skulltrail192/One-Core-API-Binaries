
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)

spec2def(vtdllXP.dll vtdllXP.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	error.c
	etw.c
	info.c
	ldrinit.c
	locale.c
    main.c
	ntapi.c
	path.c
	pointer.c
	sqm.c
	sync.c
	transaction.c
	unimplemented.c
	wow64.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vtdllXP_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vtdllXP.def)

add_library(vtdllXP SHARED ${SOURCE})
set_module_type(vtdllXP win32dll)

target_link_libraries(vtdllXP wine)

add_importlibs(vtdllXP msvcrt kernel32 ntdllnew ntdll)

add_cd_file(TARGET vtdllXP DESTINATION reactos/system32 FOR all)
