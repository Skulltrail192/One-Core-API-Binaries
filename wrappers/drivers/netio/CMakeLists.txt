include_directories(	
	${REACTOS_SOURCE_DIR}/sdk/include/crt
	${REACTOS_SOURCE_DIR}/sdk/include/ndk
	${REACTOS_SOURCE_DIR}/sdk/include/psdk
	${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos/drivers)

spec2def(netio.sys netio.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    netio.c
    netio.rc
    ${CMAKE_CURRENT_BINARY_DIR}/netio.def)

add_library(netio SHARED ${SOURCE})

set_module_type(netio kernelmodedriver ENTRYPOINT DriverEntry)

add_importlibs(netio ntoskrnl hal)

add_cd_file(TARGET netio DESTINATION reactos/system32/drivers NO_CAB FOR all)
