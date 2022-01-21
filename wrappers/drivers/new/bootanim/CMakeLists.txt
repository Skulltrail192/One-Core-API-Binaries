include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/drivers/bootvid)

spec2def(bootanim.sys bootanim.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    bootanim.c
    bootanim.rc
    ${CMAKE_CURRENT_BINARY_DIR}/bootanim.def)

add_library(bootanim SHARED ${SOURCE})

set_module_type(bootanim kernelmodedriver ENTRYPOINT DriverEntry)

add_importlibs(bootanim ntoskrnl hal bootvid)

add_cd_file(TARGET bootanim DESTINATION reactos/system32/drivers NO_CAB FOR all)
