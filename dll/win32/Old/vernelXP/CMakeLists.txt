
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/ntdll/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/idndl)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/kernelfull)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/normaliz)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/ntdllbase)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/ntdllnew)
include_directories(${REACTOS_SOURCE_DIR}/win32ss/user/ntuser)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/libs/pseh)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/csr)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/csrss)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/Win)

spec2def(vernelXP.dll vernelXP.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	alloc.c
	base.c
	boundary.c
	check.c
	console.c
	context.c
	dep.c
	missing/fiber.c
	file.c
	firmware.c
	handle.c
	info.c
    internal.c
	loader.c
	locale.c
	main.c
	memory.c
	misc.c
	missing/missing.c
	namespace.c
	numa.c
	pipe.c
	pointer.c
	power.c
	process.c
	processor.c
	recovery.c
	sortkey.c
	state.c
	sync.c
	thread.c
	time.c
	transaction.c
	unimplemented.c
	wer.c
	wow64.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vernelXP_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vernelXP.def)

add_library(vernelXP SHARED ${SOURCE})
set_module_type(vernelXP win32dll ENTRYPOINT 0)
target_link_libraries(vernelXP wine)
add_importlibs(vernelXP kernelfull ntdllbase ntdllnew ntdll normaliz psapi idndl advapinew advapi32 user32)
add_cd_file(TARGET vernelXP DESTINATION reactos/system32 FOR all)
