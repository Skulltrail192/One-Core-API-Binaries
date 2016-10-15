
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/atl)
spec2def(vomctl32.dll vomctl32.spec)

list(APPEND SOURCE
	dpa.c
	dsa.c
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vomctl32_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vomctl32.def)

add_library(vomctl32 SHARED ${SOURCE})
set_module_type(vomctl32 win32dll)
target_link_libraries(vomctl32 wine)
add_importlibs(vomctl32 msvcrt kernel32 ntdll comctlbase comctlnew user32 gdi32)
add_cd_file(TARGET vomctl32 DESTINATION reactos/system32 FOR all)
