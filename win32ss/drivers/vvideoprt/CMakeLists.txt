include_directories(${REACTOS_SOURCE_DIR}/include/reactos/drivers/bootvid)

spec2def(vvideoptr.sys vvideoptr.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    vvideoptr.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vvideoptr.def)

add_library(vvideoptr SHARED ${SOURCE})

set_module_type(vvideoptr kernelmodedriver ENTRYPOINT DriverEntry)

add_importlibs(vvideoptr ntoskrnl hal videoprtbase watchdog)

add_cd_file(TARGET vvideoptr DESTINATION reactos/system32/drivers NO_CAB FOR all)
