
add_definitions(-D_KERNEL32_)
remove_definitions(-D_WIN32_WINNT=0x502 -DWINVER=0x502)
add_definitions(-D_WIN32_WINNT=0x600 -DWINVER=0x600)

include_directories(${REACTOS_SOURCE_DIR}/sdk/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/crt)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/ndk)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/psdk)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/subsys/win)
include_directories(${REACTOS_SOURCE_DIR}/dll/wrappers/base/ntext_wrapper)

spec2def(kernel32_wrapper.dll kernel32_wrapper.spec)

list(APPEND SOURCE
	# ahcache.c
	# appcompat.c
	appmodel.c
	# baseinit.c
	# console.c
	# # datetime.c	
	# dep.c
	dllmain.c
	# error.c
	# filehops.c
	# fileinfo.c
	# fileopcr.c	
	# findfile.c
	# handle.c 
    # # locale.c
	# mem.c
	# namepipe.c
	# # nls.c
	# #number.c
	# pathmisc.c
	# powrmgmt.c
	# process.c
	# security.c
	# sortkey.c	
	# support.c
	# syblink.c	
	# # synch.c
	# sysinfo.c
	# thread.c
	# tick.c
	# time.c	
	# wer.c
	# utils.c
	# winnls/ansi.c	
    # GetFileInformationByHandleEx.c
    # GetTickCount64.c
    # InitOnceExecuteOnce.c
    # sync.c
    ${CMAKE_CURRENT_BINARY_DIR}/kernel32_wrapper.def)
	
if(ARCH STREQUAL "i386")
    list(APPEND ASM_SOURCE
		asm/i386/thread.S
	)
elseif(ARCH STREQUAL "amd64")
    list(APPEND ASM_SOURCE
        # asm/amd64/thread.S
	)
endif()	

set(kernel32_wrapper_ASM_SOURCE ${ASM_SOURCE})

add_asm_files(kernel32_wrapper_ASM ${ASM_SOURCE})	

add_library(kernel32_wrapper SHARED ${kernel32_wrapper_ASM} ${SOURCE})

set_module_type(kernel32_wrapper win32dll ENTRYPOINT BaseDllInitialize 12)

#############################################
## HACK FOR MSVC COMPILATION WITH win32dll ##
set_subsystem(kernel32_wrapper console)
################# END  HACK #################

# add_importlibs(kernel32_wrapper kernelfull ntext ntdll ntdll_vista)
if(ARCH STREQUAL "i386")
	add_importlibs(kernel32_wrapper kernelfull ntext ntdll_vista normaliz psapi advapi32 user32 version)

elseif(ARCH STREQUAL "amd64")
	add_importlibs(kernel32_wrapper kernel32 ntext normaliz psapi advapi32 user32 version)
endif()

add_dependencies(kernel32_wrapper psdk errcodes asm)

#For __except_handler3 support
target_link_libraries(kernel32_wrapper wine chkstk ${PSEH_LIB})

if(RUNTIME_CHECKS)
    target_link_libraries(kernel32_wrapper RunTmChk.lib)
endif()

add_cd_file(TARGET kernel32_wrapper DESTINATION reactos/system32 FOR all)
