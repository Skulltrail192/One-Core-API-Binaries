
include_directories(
    ${REACTOS_SOURCE_DIR}/sdk/include/reactos/drivers/fltmgr
    includes)

spec2def(fltmgrbase.sys fltmgrbase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    Context.c
    Dispatch.c
    Filter.c
    Interface.c
    Lib.c
    Messaging.c
    Misc.c
    Object.c
    Registry.c
    Volume.c
    fltmgr.h)

add_library(fltmgrbase MODULE
    ${SOURCE}
    fltmgrbase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/fltmgrbase.def)

set_module_type(fltmgrbase kernelmodedriver)
target_link_libraries(fltmgrbase ${PSEH_LIB})
add_importlibs(fltmgrbase ntoskrnl hal)
target_compile_definitions(fltmgrbase PRIVATE NTDDI_VERSION=NTDDI_WS03SP1)
add_pch(fltmgrbase fltmgr.h SOURCE)
add_cd_file(TARGET fltmgrbase DESTINATION reactos/system32/drivers NO_CAB FOR all)
