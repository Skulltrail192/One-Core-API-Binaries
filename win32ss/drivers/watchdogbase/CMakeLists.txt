
spec2def(watchdogbase.sys watchdogbase.spec ADD_IMPORTLIB)

add_library(watchdogbase SHARED
    watchdogbase.c
    ${CMAKE_CURRENT_BINARY_DIR}/watchdogbase.def)

set_module_type(watchdogbase kernelmodedriver)
add_importlibs(watchdogbase ntoskrnl hal)
add_cd_file(TARGET watchdogbase DESTINATION reactos/system32/drivers FOR all)
