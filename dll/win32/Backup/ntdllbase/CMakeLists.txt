
add_definitions(
    -D__NTDLL__
    -D_NTOSKRNL_
    -DCRTDLL)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)

spec2def(ntdllbase.dll ntdllbase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	avl.c
	crypt.c
	error.c
	etw.c
	info.c
	ldrinit.c
	locale.c
    main.c
	ntapi.c
	path.c
	pointer.c
	rtl.c
	sqm.c
	sync.c
	synctemp.c
	transaction.c
	wow64.c
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

target_link_libraries(ntdllbase libcntpr ${PSEH_LIB})

add_importlibs(ntdllbase ntdll ntdllnew)

add_cd_file(TARGET ntdllbase DESTINATION reactos/system32 FOR all)
