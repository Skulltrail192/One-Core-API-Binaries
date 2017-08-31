
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/idl)
#add_rpc_files(server ${REACTOS_SOURCE_DIR}/include/reactos/idl/desksrv.idl)
spec2def(desksrv.dll desksrv.spec ADD_IMPORTLIB)

add_library(desksrv SHARED
    main.c
    desksrv.rc
    ${CMAKE_CURRENT_BINARY_DIR}/desksrv.def)

set_module_type(desksrv win32dll UNICODE)
target_link_libraries(desksrv wine)
add_importlibs(desksrv advapi32 rpcrt4 msvcrt kernel32 ntdll)
add_cd_file(TARGET desksrv DESTINATION reactos/system32 FOR all)
