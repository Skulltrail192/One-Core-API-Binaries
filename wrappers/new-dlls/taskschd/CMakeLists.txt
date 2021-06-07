
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_BINARY_DIR}/sdk/include/psdk)
include_directories(${REACTOS_BINARY_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(taskschd.dll taskschd.spec)

add_idl_headers(taskschd_idlheader schrpc.idl taskschd_tlb.idl)	
add_rpc_files(client schrpc.idl)

list(APPEND SOURCE
    folder.c
    folder_collection.c
	main.c
    regtask.c
    task.c
    taskschd.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/taskschd_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/taskschd.def
	${CMAKE_CURRENT_BINARY_DIR}/schrpc_c.c)

add_library(taskschd SHARED ${SOURCE})
add_dependencies(taskschd mf_idlheader)
set_module_type(taskschd win32dll)
target_link_libraries(taskschd wine uuid mfuuid)
add_importlibs(taskschd oleaut32 ole32 advapi32 xmllite rpcrt4 msvcrt kernelbase ntdll)

add_cd_file(TARGET taskschd DESTINATION reactos/system32 FOR all)
