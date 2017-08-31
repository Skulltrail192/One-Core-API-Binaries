
include_directories(${REACTOS_SOURCE_DIR}/ntoskrnl/include)

add_definitions(-D_VIDEOPORT_)

spec2def(videoprtbase.sys videoprtbase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    agp.c
    child.c
    ddc.c
    dispatch.c
    dma.c
    event.c
    funclist.c
    int10.c
    interrupt.c
    registry.c
    resource.c
    services.c
    spinlock.c
    stubs.c
    timer.c
    videoprt.c
    videoprt.h)

add_library(videoprtbase SHARED
    ${SOURCE}
    guid.c
    videoprtbase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/videoprtbase.def)

set_module_type(videoprtbase kernelmodedriver)
add_importlibs(videoprtbase ntoskrnl hal)

add_pch(videoprtbase videoprt.h SOURCE)
add_cd_file(TARGET videoprtbase DESTINATION reactos/system32/drivers FOR all)

