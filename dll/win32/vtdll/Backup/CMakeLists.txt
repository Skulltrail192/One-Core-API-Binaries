
add_definitions(
    -D__NTDLL__
    -D_NTOSKRNL_
    -DCRTDLL)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)	
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/include/ndk)

spec2def(ntdllbase.dll ntdllbase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	crypt.c
    main.c
	ntapi.c
	sqm.c
	synctemp.c
	etw/etw.c	
    etw/trace.c	####### this is a only test ########
	ldr/ldrinit.c
	rtl/acl.c
	rtl/actctx.c
	rtl/avl.c	
	rtl/cnvint.c
	rtl/critical.c
	rtl/error.c
	rtl/exception.c
	rtl/heap.c
	rtl/imagedir.c
	rtl/interlck.c
	rtl/ldrrsrc.c
	rtl/locale.c
	rtl/message.c
	rtl/path.c
	rtl/pointer.c
	rtl/processor.c
	rtl/sid.c
	rtl/sync.c		
	rtl/sysinfo.c
	rtl/transaction.c	
	rtl/unicode.c
	rtl/wer.c
	rtl/wow64.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ntdllbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ntdllbase.def)

add_library(ntdllbase SHARED ${SOURCE})

set_module_type(ntdllbase win32dll HOTPATCHABLE ENTRYPOINT 0)
#############################################
## HACK FOR MSVC COMPILATION WITH win32dll ##
set_subsystem(ntdllbase console)
################# END  HACK #################

if(MSVC)
    add_target_link_flags(ntdllbase "/RELEASE")
endif()

target_link_libraries(ntdllbase libcntpr uuid ${PSEH_LIB})

add_importlibs(ntdllbase ntdll ntdllnew)

add_cd_file(TARGET ntdllbase DESTINATION reactos/system32 FOR all)
