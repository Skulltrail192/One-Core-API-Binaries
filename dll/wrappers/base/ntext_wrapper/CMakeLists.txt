
add_definitions(
    -D__NTDLL__
    -D_NTOSKRNL_
    -DCRTDLL)

include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/ndk)

spec2def(ntext.dll ntext.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	alpc.c
	crypt.c
    main.c
	ntapi.c
	sqm.c
	misc.c
	etw/etw.c	
    etw/trace.c	####### this is a only test ########
	ldr/ldrapi.c
	ldr/ldrinit.c
	ldr/loader.c
	rtl/acl.c
	rtl/actctx.c
	rtl/atom.c
	rtl/avl.c	
	rtl/boundary.c
	rtl/cache.c
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
	rtl/nls.c
	rtl/notification.c
	#rtl/newsrw.c
	# rtl/message.c
	rtl/path.c
	rtl/pointer.c
	rtl/processor.c
	rtl/profiling.c
	#rtl/pushlock.c
	rtl/sid.c
	#rtl/reactos.c		
	rtl/synch.c		
	rtl/sysinfo.c
	rtl/time.c
	rtl/transaction.c
	rtl/threadpool.c
	rtl/unicode.c
	rtl/wer.c
	rtl/wow64.c
	#rtl/wine.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ntext_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ntext.def)

add_library(ntext SHARED ${SOURCE})

set_module_type(ntext win32dll ENTRYPOINT LdrInitialize 12)
# #############################################
# ## HACK FOR MSVC COMPILATION WITH win32dll ##
# set_subsystem(ntext console)
# ################# END  HACK #################

# if(MSVC)
    # add_target_link_flags(ntext "/RELEASE")
# endif()

target_link_libraries(ntext libcntpr uuid ${PSEH_LIB})

add_importlibs(ntext ntdll)

add_cd_file(TARGET ntext DESTINATION reactos/system32 FOR all)
