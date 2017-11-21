set_cpp(WITH_RUNTIME)

add_definitions(-D_WIN32_WINNT=0x600)

include_directories(${REACTOS_SOURCE_DIR}/include)
include_directories(${REACTOS_SOURCE_DIR}/include/lib/atl)
include_directories(${REACTOS_SOURCE_DIR}/include/psdk)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/wrappers/wrappers_dependencies/propsys)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/wrappers/wrappers_dependencies/shellbase)
spec2def(shlwnew.dll shlwnew.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	unimplemented.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/shlwnew_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/shlwnew.def)

	
add_library(shlwnew SHARED ${SOURCE})

set_module_type(shlwnew win32dll)
target_link_libraries(shlwnew wine uuid)
add_importlibs(shlwnew advapi32 msvcrt user32 kernel32 ntdll shellbase shlwapi secur32 ole32 netapi32 rpcrt4 propsys comctl32 gdi32 browseui)
add_dependencies(shell32 shdocvw_v1)
add_cd_file(TARGET shlwnew DESTINATION reactos/system32 FOR all)
