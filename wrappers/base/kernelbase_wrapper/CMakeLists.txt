
add_definitions(-D_KERNEL32_)
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(
	${REACTOS_SOURCE_DIR}/sdk/lib/rtl
	${REACTOS_SOURCE_DIR}/sdk/include/crt
	${REACTOS_SOURCE_DIR}/sdk/include/ndk
	${REACTOS_SOURCE_DIR}/sdk/include/psdk
	${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine
	${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys
	${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/win
	${REACTOS_SOURCE_DIR}/wrappers/sdk/include/wsdk)	
spec2def(kernelbase.dll kernelbase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	ahcache.c
	appcompat.c
	appmodel.c
	audit.c
	baseinit.c
	console.c
	cpu.c
	curdir.c
	datetime.c	
	dep.c
	dir.c
	dllatom.c
	dllmain.c
	error.c
	filehops.c
	fileinfo.c
	filemap.c
	filemisc.c
	fileopcr.c	
	filefind.c
	handle.c 
	heap.c
    locale.c
	mem.c
	module.c
	namepipe.c
	nls.c
	numa.c
	number.c
	pathmisc.c
	perf.c
	powrmgmt.c
	process.c
	psapi.c
	reg.c
	security.c
	sortkey.c	
	string.c
	support.c
	sxs.c
	syblink.c	
	synch.c
	sysinfo.c
	thread.c
	tick.c
	time.c	
	wer.c
	winlinks.c
	utils.c
	vdm.c
	winnls/ansi.c
    version.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/kernelbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/kernelbase.def)
	
if(ARCH STREQUAL "i386")
    list(APPEND ASM_SOURCE
		asm/i386/thunk.S
	)
elseif(ARCH STREQUAL "amd64")
    list(APPEND ASM_SOURCE
        #asm/amd64/thread.S
		
	)
endif()	
	
add_asm_files(kernelbase_ASM ${ASM_SOURCE})	
	
add_library(kernelbase SHARED ${kernelbase_ASM} ${SOURCE})

set_module_type(kernelbase win32dll ENTRYPOINT BaseDllInitialize 12)

#############################################
## HACK FOR MSVC COMPILATION WITH win32dll ##
set_subsystem(kernel32 console)
################# END  HACK #################

target_link_libraries(kernelbase wine ${PSEH_LIB})

if(ARCH STREQUAL "i386")
	add_importlibs(kernelbase kernelex ntext)

elseif(ARCH STREQUAL "amd64")
	add_importlibs(kernelbase kernel32 ntext)
endif()

add_delay_importlibs(kernelbase shlwapi idndl)

add_cd_file(TARGET kernelbase DESTINATION reactos/system32 FOR all)
