
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/win)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/wrappers/base/vtdll)
spec2def(VERNEL32.dll VERNEL32.spec)

list(APPEND SOURCE
	ahcache.c
	appcompat.c
	datetime.c
	dllmain.c
	error.c
	filehops.c
	findfile.c
	fileopcr.c	
	mem.c
	namepipe.c
	process.c
	support.c	
	synch.c
	thread.c
	tick.c
	winnls/ansi.c
    VERNEL32.rc
    ${CMAKE_CURRENT_BINARY_DIR}/VERNEL32_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/VERNEL32.def)
	
if(ARCH STREQUAL "i386")
    list(APPEND ASM_SOURCE
        asm/i386/thread.S)
elseif(ARCH STREQUAL "amd64")
    list(APPEND ASM_SOURCE
        asm/amd64/thread.S)
endif()

set(VERNEL32_ASM_SOURCE ${ASM_SOURCE})

add_asm_files(VERNEL32_ASM ${ASM_SOURCE})	

add_library(VERNEL32 SHARED ${VERNEL32_ASM} ${SOURCE})

set_module_type(VERNEL32 win32dll ENTRYPOINT BaseDllInitialize 12)

add_importlibs(VERNEL32 kernelfull vtdll)

add_dependencies(VERNEL32 psdk)

#For __except_handler3 support
target_link_libraries(VERNEL32 wine chkstk ${PSEH_LIB})

if(RUNTIME_CHECKS)
    target_link_libraries(VERNEL32 RunTmChk.lib)
endif()

add_cd_file(TARGET VERNEL32 DESTINATION reactos/system32 FOR all)
