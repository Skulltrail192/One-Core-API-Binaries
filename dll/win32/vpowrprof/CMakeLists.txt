
spec2def(vpowrprof.dll vpowrprof.spec)

list(APPEND SOURCE
	main.c
    vpowrprof.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vpowrprof_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vpowrprof.def)

add_library(vpowrprof SHARED ${SOURCE})

set_module_type(vpowrprof win32dll ENTRYPOINT 0)
add_importlibs(vpowrprof advapi32 powrprofbase user32 comctl32 msvcrt kernel32 ntdll)
add_dependencies(vpowrprof psdk)
add_cd_file(TARGET vpowrprof DESTINATION reactos/system32 FOR all)
