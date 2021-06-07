
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_BINARY_DIR}/sdk/include/psdk)
include_directories(${REACTOS_BINARY_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(mfreadwrite.dll mfreadwrite.spec)

list(APPEND SOURCE
    main.c
    reader.c
    writer.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/mfreadwrite_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/mfreadwrite.def)
	
add_idl_headers(mfreadwrite_idlheader mf_classes.idl)	

add_library(mfreadwrite SHARED ${SOURCE})
add_dependencies(mfreadwrite mf_idlheader)
set_module_type(mfreadwrite win32dll)
target_link_libraries(mfreadwrite wine uuid mfuuid)
add_importlibs(mfreadwrite mfplat ole32 msvcrt kernelbase ntdll)
add_cd_file(TARGET mfreadwrite DESTINATION reactos/system32 FOR all)
