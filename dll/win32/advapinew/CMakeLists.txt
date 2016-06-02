
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)

spec2def(advapinew.dll advapinew.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
	unimplemented.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/advapinew_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/advapinew.def)

add_library(advapinew SHARED ${SOURCE})
set_module_type(advapinew win32dll)
target_link_libraries(advapinew wine)
add_importlibs(advapinew advapi32 msvcrt kernel32 ntdll)
add_cd_file(TARGET advapinew DESTINATION reactos/system32 FOR all)
