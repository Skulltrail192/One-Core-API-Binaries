
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)

add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
spec2def(vpowrprof.dll vpowrprof.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
	vpowrprof.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vpowrprof_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vpowrprof.def)

add_library(vpowrprof SHARED ${SOURCE})
set_module_type(vpowrprof win32dll UNICODE)
target_link_libraries(vpowrprof wine)
add_importlibs(vpowrprof advapi32 powrprofbase user32 comctl32 msvcrt kernel32 ntdll)
add_cd_file(TARGET vpowrprof DESTINATION reactos/system32 FOR all)
