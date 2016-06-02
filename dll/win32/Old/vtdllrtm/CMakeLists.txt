
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)

spec2def(vtdllrtm.dll vtdllrtm.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	etw.c
	info.c
	ldrinit.c
	locale.c
    main.c
	pointer.c
	sqm.c
	sync.c
	transaction.c
	unimplemented.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vtdllrtm_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vtdllrtm.def)

add_library(vtdllrtm SHARED ${SOURCE})
set_module_type(vtdllrtm win32dll)

target_link_libraries(vtdllrtm wine)

add_importlibs(vtdllrtm msvcrt kernel32 ntdll ntdllnew)

add_cd_file(TARGET vtdllrtm DESTINATION reactos/system32 FOR all)
