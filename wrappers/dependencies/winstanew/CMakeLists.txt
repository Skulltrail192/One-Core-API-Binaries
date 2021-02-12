
spec2def(winstanew.dll winstanew.spec ADD_IMPORTLIB)

include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

list(APPEND SOURCE    
    main.c
	unimplemented.c
    winstabase.h)

add_library(winstanew SHARED
    ${SOURCE}
    winstanew.rc
    ${CMAKE_CURRENT_BINARY_DIR}/winstanew.def)

set_module_type(winstanew win32dll)
target_link_libraries(winstanew wine)
add_importlibs(winstanew msvcrt kernel32 ntdll)

add_cd_file(TARGET winstanew DESTINATION reactos/system32 FOR all)
