
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
spec2def(psapi_new.dll psapi_new.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    psapi.c
    psapi_new.rc
    ${CMAKE_CURRENT_BINARY_DIR}/psapi_new.def)

add_library(psapi_new MODULE ${SOURCE})
set_module_type(psapi_new win32dll)
target_link_libraries(psapi_new ${PSEH_LIB})
add_importlibs(psapi_new msvcrt kernel32 ntdll)
add_cd_file(TARGET psapi_new DESTINATION reactos/system32 FOR all)
