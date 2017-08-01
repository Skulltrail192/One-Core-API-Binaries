set_cpp(WITH_RUNTIME)

add_definitions(-D_WIN32_WINNT=0x600)

include_directories(${REACTOS_SOURCE_DIR}/include)
include_directories(${REACTOS_SOURCE_DIR}/include/lib/atl)
include_directories(${REACTOS_SOURCE_DIR}/include/psdk)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/wrappers/wrappers_dependencies/propsys)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/wrappers/wrappers_dependencies/shellbase)
spec2def(shellnew.dll shellnew.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	unimplemented.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/shellnew_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/shellnew.def)

	
add_library(shellnew SHARED ${SOURCE})

set_module_type(shellnew win32dll)
target_link_libraries(shellnew wine uuid)
add_pch(shell32 main.h SOURCE)
add_importlibs(shellnew advapi32 msvcrt user32 kernel32 ntdll shellbase shlwapi secur32 ole32 netapi32 rpcrt4 propsys comctl32 gdi32 browseui)
add_dependencies(shell32 shdocvw_v1)
add_cd_file(TARGET shellnew DESTINATION reactos/system32 FOR all)
