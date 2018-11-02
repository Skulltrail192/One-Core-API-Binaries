
add_definitions(-D_KERNEL32_)
add_definitions(-D__WINESRC__)
include_directories(
	${REACTOS_SOURCE_DIR}/sdk/lib/rtl
	${REACTOS_SOURCE_DIR}/sdk/include/crt
	${REACTOS_SOURCE_DIR}/sdk/include/ndk
	${REACTOS_SOURCE_DIR}/sdk/include/psdk
	${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine
	${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys
	${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/win
	${REACTOS_SOURCE_DIR}/dll/wrappers/base/ntext_wrapper)	
spec2def(kernel32_wrapper.dll kernel32_wrapper.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	ahcache.c
	appcompat.c
	appmodel.c
	baseinit.c
	console.c
	cpu.c
	curdir.c
	datetime.c	
	dep.c
	dir.c
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
	powrmgmt.c
	process.c
	security.c
	sortkey.c	
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
    kernel32_wrapper.rc
    ${CMAKE_CURRENT_BINARY_DIR}/kernel32_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/kernel32_wrapper.def)
	
if(ARCH STREQUAL "i386")
    list(APPEND ASM_SOURCE
		asm/i386/thread.S
	)
elseif(ARCH STREQUAL "amd64")
    list(APPEND ASM_SOURCE
        #asm/amd64/thread.S
		
	)
endif()	
	
add_asm_files(kernel32_wrapper_ASM ${ASM_SOURCE})	
	
add_library(kernel32_wrapper SHARED ${kernel32_wrapper_ASM} ${SOURCE})

set_module_type(kernel32_wrapper win32dll ENTRYPOINT BaseDllInitialize 12)

#############################################
## HACK FOR MSVC COMPILATION WITH win32dll ##
set_subsystem(kernel32 console)
################# END  HACK #################

target_link_libraries(kernel32_wrapper wine ${PSEH_LIB})

if(ARCH STREQUAL "i386")
	add_importlibs(kernel32_wrapper kernelex ntext normaliz fileextd idndl psapi advapi32 user32 version)

elseif(ARCH STREQUAL "amd64")
	add_importlibs(kernel32_wrapper kernel32 ntext normaliz psapi fileextd advapi32 user32 version)
endif()

add_cd_file(TARGET kernel32_wrapper DESTINATION reactos/system32 FOR all)
