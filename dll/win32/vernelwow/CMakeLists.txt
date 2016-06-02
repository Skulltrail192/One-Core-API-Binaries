
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

spec2def(vernelwow.dll vernelwow.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	../vernel32/alloc.c
	../vernel32/base.c
	../vernel32/boundary.c
	../vernel32/check.c
	../vernel32/console.c
	../vernel32/context.c
	../vernel32/dep.c
	../vernel32/file.c
	../vernel32/firmware.c
	../vernel32/handle.c
    ../vernel32/internal.c
	../vernel32/loader.c
	../vernel32/locale.c
	../vernel32/main.c
	../vernel32/memory.c
	../vernel32/misc.c
	../vernel32/namespace.c
	../vernel32/numa.c
	../vernel32/pipe.c
	../vernel32/pointer.c
	../vernel32/power.c
	../vernel32/process.c
	../vernel32/processor.c
	../vernel32/recovery.c
	../vernel32/sortkey.c
	../vernel32/state.c
	../vernel32/sync.c
	../vernel32/sysinfo.c	
	../vernel32/thread.c
	../vernel32/threadpool.c
	../vernel32/time.c
	../vernel32/transaction.c
	../vernel32/unimplemented.c
	../vernel32/wer.c
	../vernel32/wow64.c
	../vernel32/missing/fiber.c
	../vernel32/missing/missing.c	
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vernelwow_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vernelwow.def)

add_library(vernelwow 
			SHARED 
			${SOURCE})

set_module_type(vernelwow win32dll HOTPATCHABLE ENTRYPOINT 0)
#############################################
## HACK FOR MSVC COMPILATION WITH win32dll ##
set_subsystem(vernelwow console)
################# END  HACK #################

if(RUNTIME_CHECKS)
    target_link_libraries(kernel32 RunTmChk.lib)
endif()

if(ARCH STREQUAL "amd64")
	target_link_libraries(vernelwow wine)
elseif(ARCH STREQUAL "i386")
	target_link_libraries(vernelwow wine chkstk ${PSEH_LIB})
endif()
add_importlibs(vernelwow kernelfull vtdll ntdllnew normaliz psapi idndl advapi32 user32 version)
add_cd_file(TARGET vernelwow DESTINATION reactos/system32 FOR all)
