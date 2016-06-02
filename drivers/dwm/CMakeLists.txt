include_directories(${REACTOS_SOURCE_DIR}/include/reactos/drivers/bootvid)

spec2def(dwm.sys dwm.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    dwm.c
    dwm.rc
    ${CMAKE_CURRENT_BINARY_DIR}/dwm.def)

add_library(dwm SHARED ${SOURCE})

set_module_type(dwm kernelmodedriver ENTRYPOINT DriverEntry)

add_importlibs(dwm ntoskrnl hal bootvid win32k)

add_cd_file(TARGET dwm DESTINATION reactos/system32/drivers NO_CAB FOR all)
