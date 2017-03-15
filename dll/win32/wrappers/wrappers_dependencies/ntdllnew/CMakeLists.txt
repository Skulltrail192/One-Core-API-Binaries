
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)

spec2def(ntdllnew.dll ntdllnew.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
	unimplemented.c
	ntdllnew.h
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ntdllnew_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ntdllnew.def)

add_library(ntdllnew SHARED ${SOURCE})
set_module_type(ntdllnew win32dll)
target_link_libraries(ntdllnew wine)
add_importlibs(ntdllnew advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET ntdllnew DESTINATION reactos/system32 FOR all)
