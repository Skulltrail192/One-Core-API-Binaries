
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/wrappers/includes)
include_directories(${REACTOS_BINARY_DIR}/sdk/include/reactos/wine)
spec2def(tbs.dll tbs.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    tbs.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/tbs_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/tbs.def)

add_library(tbs SHARED ${SOURCE})
set_module_type(tbs win32dll)
target_link_libraries(tbs wine)
add_importlibs(tbs ole32 combase msvcrt kernelbase ntdll)
add_cd_file(TARGET tbs DESTINATION reactos/system32 FOR all)
