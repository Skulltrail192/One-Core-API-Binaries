list(APPEND SOURCE
    common.c
	entrypts.c
    hbastat.c
    io.c
    pnppower.c
    util.c)

add_library(storahci_new MODULE ${SOURCE} storahci_new.rc)

set_module_type(storahci_new kernelmodedriver ENTRYPOINT DriverEntry)
add_importlibs(storahci_new storport ntoskrnl hal)
add_cd_file(TARGET storahci_new DESTINATION reactos/system32/drivers NO_CAB FOR all)
#add_registry_inf(storahci_new.inf)
