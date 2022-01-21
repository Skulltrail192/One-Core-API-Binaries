
include_directories(
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/drivers/fltmgr
    includes)

spec2def(fltmgr_wrapper.sys fltmgr_wrapper.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	main.c
    fltmgr.h)

add_library(fltmgr_wrapper MODULE
    ${SOURCE}
    fltmgr_wrapper.rc
    ${CMAKE_CURRENT_BINARY_DIR}/fltmgr_wrapper.def)

set_module_type(fltmgr_wrapper kernelmodedriver)
target_link_libraries(fltmgr_wrapper ${PSEH_LIB})
add_importlibs(fltmgr_wrapper ntoskrnl hal fltmgrbase)
target_compile_definitions(fltmgr_wrapper PRIVATE NTDDI_VERSION=NTDDI_WS03SP1)
add_pch(fltmgr_wrapper fltmgr.h SOURCE)
add_cd_file(TARGET fltmgr_wrapper DESTINATION reactos/system32/drivers NO_CAB FOR all)
