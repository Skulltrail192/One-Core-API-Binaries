
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/include/psdk)
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/include/reactos/wine)

list(APPEND SOURCE
    main.c
    manifest.c
    wusa.rc)

add_executable(wusa ${SOURCE})

set_module_type(wusa win32cui UNICODE)
target_link_libraries(wusa wine msxml2 uuid)
add_importlibs(wusa cabiext shlwapi ole32 oleaut32 msvcrt advapi32 kernel32 ntdll)
add_cd_file(TARGET wusa DESTINATION reactos/system32 FOR all)