
add_definitions(-D_KERNEL32_)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/ntdll/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/idndl)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/kernelfull)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/normaliz)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/vtdll)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/ntdllnew)
include_directories(${REACTOS_SOURCE_DIR}/win32ss/user/ntuser)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/csr)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/csrss)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/Win)

spec2def(vernel32.dll vernel32.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	alloc.c
	base.c
	boundary.c
	check.c
	console.c
	context.c
	debug.c
	dep.c
	exception.c
	file.c
	findfile.c
	firmware.c
	handle.c
    internal.c
	loader.c
	locale.c
	main.c
	memory.c
	misc.c
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
	sysinfo.c	
	thread.c
	threadpool.c
	time.c
	transaction.c
	unimplemented.c
	wer.c
	wow64.c
	missing/fiber.c
	missing/missing.c)

add_asm_files(vernel32_asm ${ASM_SOURCE})		
add_library(vernel32 SHARED 
			${SOURCE}
			${vernel32_asm}
			 version.rc
			 ${CMAKE_CURRENT_BINARY_DIR}/vernel32_stubs.c
			${CMAKE_CURRENT_BINARY_DIR}/vernel32.def)

set_module_type(vernel32 win32dll HOTPATCHABLE ENTRYPOINT 0)
#############################################
## HACK FOR MSVC COMPILATION WITH win32dll ##
set_subsystem(vernel32 console)
################# END  HACK #################

if(RUNTIME_CHECKS)
    target_link_libraries(kernel32 RunTmChk.lib)
endif()

if(ARCH STREQUAL "amd64")
	target_link_libraries(vernel32 wine)
elseif(ARCH STREQUAL "i386")
	target_link_libraries(vernel32 wine chkstk ${PSEH_LIB})
endif()
add_importlibs(vernel32 kernelfull vtdll normaliz psapi idndl advapi32 user32 fileextd version)
add_cd_file(TARGET vernel32 DESTINATION reactos/system32 FOR all)
