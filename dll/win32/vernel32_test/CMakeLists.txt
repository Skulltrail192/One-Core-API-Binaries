
add_definitions(-D_KERNEL32_)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/ntdll/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/idndl)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/kernelfull)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/normaliz)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/vtdll)
include_directories(${REACTOS_SOURCE_DIR}/win32ss/user/ntuser)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/csr)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/csrss)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/Win)

spec2def(vernel32_test.dll vernel32_test.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/alloc.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/appmodel.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/barrier.c	
    ${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/base.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/boundary.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/check.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/console.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/context.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/debug.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/dep.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/file.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/exception.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/findfile.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/firmware.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/handle.c
    ${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/internal.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/loader.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/locale.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/main.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/memory.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/misc.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/namespace.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/numa.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/pipe.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/pointer.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/power.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/process.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/processor.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/recovery.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/sortkey.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/state.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/sync.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/sysinfo.c	
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/thread.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/threadpool.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/time.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/transaction.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/unimplemented.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/wer.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/wow64.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/missing/fiber.c
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32_test/missing/missing.c
)
	
if(ARCH STREQUAL "i386")
    list(APPEND ASM_SOURCE
        asm/i386/thread.S
		)
elseif(ARCH STREQUAL "amd64")
    list(APPEND ASM_SOURCE
        #asm/amd64/thread.S
		)
endif()

set(vernel32_test_SOURCE ${SOURCE})
set(vernel32_test_ASM_SOURCE ${ASM_SOURCE})

add_asm_files(vernel32_test_ASM ${ASM_SOURCE})		
add_library(vernel32_test SHARED 
			${vernel32_test_SOURCE}
			${vernel32_test_ASM}
			 version.rc
			 ${CMAKE_CURRENT_BINARY_DIR}/vernel32_test_stubs.c
			${CMAKE_CURRENT_BINARY_DIR}/vernel32_test.def)

set_module_type(vernel32_test win32dll HOTPATCHABLE ENTRYPOINT 0)
#############################################
## HACK FOR MSVC COMPILATION WITH win32dll ##
set_subsystem(vernel32_test console)
################# END  HACK #################

if(RUNTIME_CHECKS)
    target_link_libraries(kernel32 RunTmChk.lib)
endif()

if(ARCH STREQUAL "amd64")
	target_link_libraries(vernel32_test wine)
elseif(ARCH STREQUAL "i386")
	target_link_libraries(vernel32_test wine chkstk ${PSEH_LIB})
endif()
add_importlibs(vernel32_test kernelfull vtdll normaliz psapi idndl advapi32 user32 fileextd version)
add_cd_file(TARGET vernel32_test DESTINATION reactos/system32 FOR all)
