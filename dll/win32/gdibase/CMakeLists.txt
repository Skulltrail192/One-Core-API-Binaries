

add_definitions(-DLANGPACK)

include_directories(
    include
    ${REACTOS_SOURCE_DIR}/win32ss/include)

spec2def(gdibase.dll gdibase.spec ADD_IMPORTLIB)

if(ARCH STREQUAL "i386")
    list(APPEND SOURCE
        objects/efloat.c)
endif()

list(APPEND SOURCE
    main/dllmain.c
    misc/heap.c
    misc/gdientry.c
    #misc/hacks.c
    misc/historic.c
    misc/misc.c
    misc/stubs.c
    misc/stubsa.c
    misc/stubsw.c
    misc/wingl.c
    objects/arc.c
    objects/bitmap.c
    objects/brush.c
    objects/coord.c
    objects/dc.c
    objects/eng.c
    objects/enhmfile.c
    objects/font.c
    objects/icm.c
    objects/linedda.c
    objects/metafile.c
    objects/painting.c
    objects/printdrv.c
    objects/palette.c
    objects/pen.c
    objects/region.c
    objects/text.c
    objects/utils.c
    objects/path.c
    include/precomp.h)

add_library(gdibase SHARED
    ${SOURCE}
    gdibase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/gdibase.def)

set_module_type(gdibase
    win32dll
    ENTRYPOINT DllMain 12
    UNICODE)

target_link_libraries(gdibase
    win32ksys
    dxguid
    ${PSEH_LIB})

add_importlibs(gdibase user32 advapi32 kernel32 ntdll)
add_pch(gdibase include/precomp.h SOURCE)
add_cd_file(TARGET gdibase DESTINATION reactos/system32 FOR all)

