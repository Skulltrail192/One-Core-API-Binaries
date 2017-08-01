set_cpp(WITH_RUNTIME)

add_definitions(-D_WIN32_WINNT=0x600)

include_directories(${REACTOS_SOURCE_DIR}/include)
include_directories(${REACTOS_SOURCE_DIR}/include/lib/atl)
include_directories(${REACTOS_SOURCE_DIR}/include/psdk)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/wrappers/wrappers_dependencies/propsys)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/wrappers/wrappers_dependencies/shellbase)
spec2def(vhell32.dll vhell32.spec)

list(APPEND SOURCE
	classes.c
	iconcache.c
    main.c
	misc.c
	path.c
	pidl.c
	shellitem.c
	shellpath.c
	shlmenu.c
	shlview_cmenu.c
	unimplemented.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vhell32_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vhell32.def)

	
add_library(vhell32 SHARED ${SOURCE})

set_module_type(vhell32 win32dll)
target_link_libraries(vhell32 wine uuid)
add_pch(shell32 main.h SOURCE)
add_importlibs(vhell32 advapi32 msvcrt user32 kernel32 ntdll shellbase shellnew shlwapi secur32 ole32 netapi32 rpcrt4 propsys comctl32 userenv gdi32 browseui)
add_dependencies(shell32 shdocvw_v1)
add_cd_file(TARGET vhell32 DESTINATION reactos/system32 FOR all)
