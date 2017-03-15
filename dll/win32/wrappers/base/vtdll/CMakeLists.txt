
add_definitions(
    -D__NTDLL__
    -D_NTOSKRNL_
    -DCRTDLL)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)	
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/include/ndk)

spec2def(vtdll.dll vtdll.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	alpc.c
	crypt.c
    main.c
	ntapi.c
	sqm.c
	misc.c
	#threadpool.c
	etw/etw.c	
    etw/trace.c	####### this is a only test ########
	ldr/ldrapi.c
	ldr/ldrinit.c
	ldr/loader.c
	rtl/acl.c
	rtl/actctx.c
	rtl/avl.c	
	rtl/boundary.c
	rtl/cache.c
	rtl/cnvint.c
	rtl/context.c
	rtl/critical.c
	rtl/environ.c
	rtl/error.c
	rtl/extfeatures.c
	rtl/exception.c
	rtl/heap.c
	rtl/imagedir.c
	rtl/interlck.c
	rtl/ldrrsrc.c
	rtl/locale.c
	rtl/misc.c
	rtl/notification.c
#	rtl/message.c
	rtl/path.c
	rtl/pointer.c
	rtl/processor.c
	rtl/profiling.c
	rtl/sid.c
	rtl/sync.c		
	rtl/sysinfo.c
	rtl/time.c
	rtl/transaction.c
	rtl/threadpool.c
	rtl/unicode.c
	rtl/wer.c
	rtl/wow64.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vtdll_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vtdll.def)

add_library(vtdll SHARED ${SOURCE})

set_module_type(vtdll win32dll HOTPATCHABLE ENTRYPOINT 0)
#############################################
## HACK FOR MSVC COMPILATION WITH win32dll ##
set_subsystem(vtdll console)
################# END  HACK #################

if(MSVC)
    add_target_link_flags(vtdll "/RELEASE")
endif()

target_link_libraries(vtdll libcntpr uuid ${PSEH_LIB})

add_importlibs(vtdll ntdll ntdllnew)

add_cd_file(TARGET vtdll DESTINATION reactos/system32 FOR all)
