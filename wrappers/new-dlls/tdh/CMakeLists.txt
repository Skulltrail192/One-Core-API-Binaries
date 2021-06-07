
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/wrappers/includes)
include_directories(${REACTOS_BINARY_DIR}/sdk/include/reactos/wine)
spec2def(tdh.dll tdh.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    tdh_main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/tdh_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/tdh.def)

add_library(tdh SHARED ${SOURCE})
set_module_type(tdh win32dll)
target_link_libraries(tdh wine)
add_importlibs(tdh ole32 combase msvcrt kernelbase ntdll)
add_cd_file(TARGET tdh DESTINATION reactos/system32 FOR all)
