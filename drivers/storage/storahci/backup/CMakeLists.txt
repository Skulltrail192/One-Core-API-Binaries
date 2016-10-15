add_definitions(-DDEBUG)

list(APPEND SOURCE
    storahci.c)

add_library(storahci SHARED ${SOURCE} storahci.rc)

set_module_type(storahci kernelmodedriver ENTRYPOINT DriverEntry)
add_importlibs(storahci scsiport ntoskrnl hal)
add_cd_file(TARGET storahci DESTINATION reactos/system32/drivers NO_CAB FOR all)
