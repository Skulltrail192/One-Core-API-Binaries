
add_definitions(-D_KERNEL32_)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/include/crt)
include_directories(${REACTOS_SOURCE_DIR}/include/ndk)
include_directories(${REACTOS_SOURCE_DIR}/include/psdk)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/win)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/wrappers/base/vtdll)
spec2def(vernel32.dll vernel32.spec)

list(APPEND SOURCE
	ahcache.c
	appcompat.c
	datetime.c
	dep.c
	dllmain.c
	error.c
	filehops.c
	fileinfo.c
	fileopcr.c	
	findfile.c
    locale.c
	mem.c
	module.c
	namepipe.c
	process.c
	sortkey.c	
	support.c
	syblink.c
	synch.c
	sysinfo.c
	thread.c
	tick.c
	wer.c
	utils.c
	winnls/ansi.c
    vernel32.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vernel32_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vernel32.def)
	
if(ARCH STREQUAL "i386")
    list(APPEND ASM_SOURCE
		asm/i386/thread.S
	)
elseif(ARCH STREQUAL "amd64")
    list(APPEND ASM_SOURCE
        # asm/amd64/thread.S
	)
endif()

set(vernel32_ASM_SOURCE ${ASM_SOURCE})

add_asm_files(vernel32_ASM ${ASM_SOURCE})	

add_library(vernel32 SHARED ${vernel32_ASM} ${SOURCE})

set_module_type(vernel32 win32dll ENTRYPOINT BaseDllInitialize 12)

add_importlibs(vernel32 kernelfull vtdll normaliz)

add_dependencies(vernel32 psdk)

#For __except_handler3 support
target_link_libraries(vernel32 wine chkstk ${PSEH_LIB})

if(RUNTIME_CHECKS)
    target_link_libraries(vernel32 RunTmChk.lib)
endif()

add_cd_file(TARGET vernel32 DESTINATION reactos/system32 FOR all)
