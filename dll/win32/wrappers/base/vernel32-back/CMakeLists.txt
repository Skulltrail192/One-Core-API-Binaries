
add_definitions(-D_KERNEL32_)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/ntdll/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/idndl)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/kernelfull)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/normaliz)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/vtdll)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/nlsdl)
include_directories(${REACTOS_SOURCE_DIR}/win32ss/user/ntuser)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/csr)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/csrss)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/Win)

spec2def(vernel32.dll vernel32.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	${REACTOS_SOURCE_DIR}/dll/win32/vernel32/support.c
)
	
if(ARCH STREQUAL "i386")
    list(APPEND ASM_SOURCE
        asm/i386/thread.S
		)
elseif(ARCH STREQUAL "amd64")
    list(APPEND ASM_SOURCE
        asm/amd64/thread.S
		)
endif()

set(VERNEL32_SOURCE ${SOURCE})
set(VERNEL32_ASM_SOURCE ${ASM_SOURCE})

add_asm_files(VERNEL32_ASM ${ASM_SOURCE})		
add_library(vernel32 SHARED 
			${VERNEL32_SOURCE}
			${VERNEL32_ASM}
			 vernel32.rc
			 ${CMAKE_CURRENT_BINARY_DIR}/vernel32_stubs.c
			${CMAKE_CURRENT_BINARY_DIR}/vernel32.def)

set_module_type(vernel32 win32dll HOTPATCHABLE ENTRYPOINT 0)
#############################################
## HACK FOR MSVC COMPILATION WITH win32dll ##
set_subsystem(vernel32 console)
################# END  HACK #################

if(RUNTIME_CHECKS)
    target_link_libraries(kernel32 RunTmChk.lib)
endif()

if(ARCH STREQUAL "amd64")
	target_link_libraries(vernel32 wine)
elseif(ARCH STREQUAL "i386")
	target_link_libraries(vernel32 wine chkstk ${PSEH_LIB})
endif()
add_importlibs(vernel32 kernelfull vtdll)
add_cd_file(TARGET vernel32 DESTINATION reactos/system32 FOR all)
