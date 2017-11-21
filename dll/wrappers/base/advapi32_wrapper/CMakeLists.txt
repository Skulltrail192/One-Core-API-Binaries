
add_definitions(-D_ADVAPI32_)

remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/wrappers/ntdllnew)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/kernel32/include)

spec2def(advapi32_wrapper.dll advapi32_wrapper.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/advapi32_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/advapi32_wrapper.def)

add_library(advapi32_wrapper SHARED ${SOURCE})
set_module_type(advapi32_wrapper win32dll ENTRYPOINT 0)

target_link_libraries(advapi32_wrapper wine)

add_importlibs(advapi32_wrapper advapinew advapibase advapilong ntext wevtapibase)

add_cd_file(TARGET advapi32_wrapper DESTINATION reactos/system32 FOR all)
