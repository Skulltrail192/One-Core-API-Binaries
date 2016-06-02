
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/ntdll/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/idndl)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/kernelbasex)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/normaliz)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/ntdllbase)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/ntdllnew)
include_directories(${REACTOS_SOURCE_DIR}/win32ss/user/ntuser)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/csr)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/csrss)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/Win)

spec2def(vernelwow.dll vernelwow.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	alloc.c
	base.c
	boundary.c
	check.c
	console.c
	context.c
	dep.c
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
	namespace.c
	numa.c
	pipe.c
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
    ${CMAKE_CURRENT_BINARY_DIR}/vernelwow_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vernelwow.def)

add_library(vernelwow SHARED ${SOURCE})
set_module_type(vernelwow win32dll ENTRYPOINT 0)
target_link_libraries(vernelwow wine)
add_importlibs(vernelwow kernelbasex ntdll ntdllnew normaliz ntdllbase psapi idndl advapinew advapi32 user32)
add_cd_file(TARGET vernelwow DESTINATION reactos/system32 FOR all)
