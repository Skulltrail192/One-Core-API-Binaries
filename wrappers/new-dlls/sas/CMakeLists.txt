
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/wrappers/includes)
include_directories(${REACTOS_BINARY_DIR}/sdk/include/reactos/wine)
spec2def(sas.dll sas.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/sas_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/sas.def)

add_library(sas SHARED ${SOURCE})
set_module_type(sas win32dll)
target_link_libraries(sas wine)
add_importlibs(sas ole32 combase msvcrt kernelbase ntdll)
add_cd_file(TARGET sas DESTINATION reactos/system32 FOR all)
