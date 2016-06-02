
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/vtdll)

spec2def(ntdllbase.dll ntdllbase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	${REACTOS_SOURCE_DIR}/dll/win32/vtdll/etw.c
	${REACTOS_SOURCE_DIR}/dll/win32/vtdll/info.c
	${REACTOS_SOURCE_DIR}/dll/win32/vtdll/ldrinit.c
	${REACTOS_SOURCE_DIR}/dll/win32/vtdll/locale.c
    ${REACTOS_SOURCE_DIR}/dll/win32/vtdll/main.c
	${REACTOS_SOURCE_DIR}/dll/win32/vtdll/ntapi.c
	${REACTOS_SOURCE_DIR}/dll/win32/vtdll/path.c
	${REACTOS_SOURCE_DIR}/dll/win32/vtdll/pointer.c
	${REACTOS_SOURCE_DIR}/dll/win32/vtdll/rtl.c
	${REACTOS_SOURCE_DIR}/dll/win32/vtdll/sqm.c
	${REACTOS_SOURCE_DIR}/dll/win32/vtdll/sync.c
	${REACTOS_SOURCE_DIR}/dll/win32/vtdll/transaction.c
	${REACTOS_SOURCE_DIR}/dll/win32/vtdll/unimplemented.c
    ${REACTOS_SOURCE_DIR}/dll/win32/vtdll/version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ntdllbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ntdllbase.def)

add_library(ntdllbase SHARED ${SOURCE})
set_module_type(ntdllbase win32dll)

target_link_libraries(ntdllbase wine)

add_importlibs(ntdllbase msvcrt kernel32 ntdll ntdllnew)

add_cd_file(TARGET ntdllbase DESTINATION reactos/system32 FOR all)
