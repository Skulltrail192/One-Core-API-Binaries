
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(rpcrt4_wrapper.dll rpcrt4_wrapper.spec)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/rpcrt4_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/rpcrt4_wrapper.def)

add_library(rpcrt4_wrapper SHARED ${SOURCE})
set_module_type(rpcrt4_wrapper win32dll)
target_link_libraries(rpcrt4_wrapper wine)
add_importlibs(rpcrt4_wrapper rpcrtbase msvcrt kernel32 ntdll)
add_cd_file(TARGET rpcrt4_wrapper DESTINATION reactos/system32 FOR all)
