include_directories(${REACTOS_SOURCE_DIR}/include/reactos/drivers/bootvid)

list(APPEND SOURCE
    main.c
    storahci.rc)

add_library(storahci SHARED ${SOURCE})

set_module_type(storahci kernelmodedriver ENTRYPOINT DriverEntry)

add_importlibs(storahci ntoskrnl hal scsiport storport)

add_cd_file(TARGET storahci DESTINATION reactos/system32/drivers NO_CAB FOR all)
