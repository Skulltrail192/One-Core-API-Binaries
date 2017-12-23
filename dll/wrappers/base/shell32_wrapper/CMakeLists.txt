set_cpp(WITH_RUNTIME)

add_definitions(-D_WIN32_WINNT=0x600)

include_directories(${REACTOS_SOURCE_DIR}/include)
include_directories(${REACTOS_SOURCE_DIR}/include/sdk/lib/atl)
include_directories(${REACTOS_SOURCE_DIR}/include/sdk/psdk)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/dll/wrappers/dependencies/propsys)
include_directories(${REACTOS_SOURCE_DIR}/dll/wrappers/dependencies/shellbase)
spec2def(shell32_wrapper.dll shell32_wrapper.spec)

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
    ${CMAKE_CURRENT_BINARY_DIR}/shell32_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/shell32_wrapper.def)

	
add_library(shell32_wrapper SHARED ${SOURCE})

set_module_type(shell32_wrapper win32dll)
target_link_libraries(shell32_wrapper wine uuid)
add_importlibs(shell32_wrapper advapi32 msvcrt user32 kernel32 ntdll shellbase shlwapi secur32 ole32 netapi32 rpcrt4 propsys comctl32 userenv gdi32 browseui)
add_dependencies(shell32 shdocvw_v1)
add_cd_file(TARGET shell32_wrapper DESTINATION reactos/system32 FOR all)
