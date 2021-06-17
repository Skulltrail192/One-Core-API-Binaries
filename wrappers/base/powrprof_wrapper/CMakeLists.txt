
spec2def(powrprof_wrapper.dll powrprof_wrapper.spec)

list(APPEND SOURCE
	main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/powrprof_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/powrprof_wrapper.def)

add_library(powrprof_wrapper SHARED ${SOURCE})

set_module_type(powrprof_wrapper win32dll ENTRYPOINT 0)
add_importlibs(powrprof_wrapper advapi32 powrprofbase user32 comctl32 msvcrt kernel32 ntdll)
add_dependencies(powrprof_wrapper psdk)
add_cd_file(TARGET powrprof_wrapper DESTINATION reactos/system32 FOR all)
