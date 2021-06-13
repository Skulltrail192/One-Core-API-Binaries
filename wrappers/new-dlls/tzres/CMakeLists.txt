
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/wrappers/includes)
include_directories(${REACTOS_BINARY_DIR}/sdk/include/reactos/wine)
spec2def(tzres.dll tzres.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    tzres.rc
    ${CMAKE_CURRENT_BINARY_DIR}/tzres_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/tzres.def)

add_library(tzres SHARED ${SOURCE})
set_module_type(tzres win32dll)
target_link_libraries(tzres wine)
add_importlibs(tzres ole32 combase msvcrt kernelbase ntdll)
add_cd_file(TARGET tzres DESTINATION reactos/system32 FOR all)
