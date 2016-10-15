
spec2def(vpowrprof.dll vpowrprof.spec ADD_IMPORTLIB)


add_library(vpowrprof SHARED
    main.c
    vpowrprof.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vpowrprof.def)

set_module_type(vpowrprof win32dll UNICODE)

target_link_libraries(vpowrprof wine)

add_importlibs(vpowrprof advapi32 powrprofbase user32 comctl32 msvcrt kernel32 ntdll)

add_cd_file(TARGET vpowrprof DESTINATION reactos/system32 FOR all)
