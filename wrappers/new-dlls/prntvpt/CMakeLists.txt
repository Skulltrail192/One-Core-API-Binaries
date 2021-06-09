
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

spec2def(prntvpt.dll prntvpt.spec)

list(APPEND SOURCE
    dllmain.c
    main.c
    ticket.c
    ${CMAKE_CURRENT_BINARY_DIR}/prntvpt_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/prntvpt.def)

add_library(prntvpt SHARED ${SOURCE} version.rc)
set_module_type(prntvpt win32dll)
target_link_libraries(prntvpt wine)
add_importlibs(prntvpt winspool msvcrt ole32 oleaut32 kernel32 ntdll)
add_cd_file(TARGET prntvpt DESTINATION reactos/system32 FOR all)
