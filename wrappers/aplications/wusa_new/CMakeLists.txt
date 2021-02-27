
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/include/psdk)
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/include/reactos/wine)

list(APPEND SOURCE
    main.c
    manifest.c
    wusa_new.rc)

add_executable(wusa_new ${SOURCE})

set_module_type(wusa_new win32cui UNICODE)
target_link_libraries(wusa_new wine uuid)
add_importlibs(wusa_new cabinet shlwapi ole32 oleaut32 advapi32 ntdll)
add_cd_file(TARGET wusa_new DESTINATION reactos/system32 FOR all)