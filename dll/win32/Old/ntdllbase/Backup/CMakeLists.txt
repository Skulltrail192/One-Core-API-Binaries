
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)

spec2def(ntdllbase.dll ntdllbase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	etw.c
	ldrinit.c
	locale.c
    main.c
	sqm.c
	sync.c
	transaction.c
	unimplemented.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ntdllbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ntdllbase.def)

add_library(ntdllbase SHARED ${SOURCE})
set_module_type(ntdllbase win32dll)

target_link_libraries(ntdllbase wine)

add_importlibs(ntdllbase msvcrt kernel32 ntdll ntdllnew)

add_cd_file(TARGET ntdllbase DESTINATION reactos/system32 FOR all)
