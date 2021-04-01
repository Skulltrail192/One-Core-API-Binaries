remove_definitions(-D_WIN32_WINNT=0x502 -DWINVER=0x502)
add_definitions(-D_WIN32_WINNT=0x600 -DWINVER=0x600)

spec2def(API-MS-WIN-CORE-CRT-L1-1-0.dll API-MS-WIN-CORE-CRT-L1-1-0.spec)

list(APPEND SOURCE
    API-MS-WIN-CORE-CRT-L1-1-0.rc
    ${CMAKE_CURRENT_BINARY_DIR}/API-MS-WIN-CORE-CRT-L1-1-0_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/API-MS-WIN-CORE-CRT-L1-1-0.def)

add_library(API-MS-WIN-CORE-CRT-L1-1-0 SHARED ${SOURCE})

set_module_type(API-MS-WIN-CORE-CRT-L1-1-0 win32dll ENTRYPOINT 0)
add_importlibs(API-MS-WIN-CORE-CRT-L1-1-0 msvcrt kernelbase ntdll)
add_dependencies(API-MS-WIN-CORE-CRT-L1-1-0 psdk)
add_cd_file(TARGET API-MS-WIN-CORE-CRT-L1-1-0 DESTINATION reactos/system32 FOR all)
