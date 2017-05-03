
spec2def(ntkrnlext.dll ntkrnlext.spec ADD_IMPORTLIB)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)

list(APPEND SOURCE
	etw.c
	io.c
    ntkrnlext.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ntkrnlext_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ntkrnlext.def)

add_library(ntkrnlext SHARED ${SOURCE})

# set_module_type(ntkrnlext win32dll ENTRYPOINT 0)
set_subsystem(ntkrnlext native)
set_image_base(ntkrnlext 0x00010000)
add_importlibs(ntkrnlext ntoskrnl)
add_dependencies(ntkrnlext psdk)
add_cd_file(TARGET ntkrnlext DESTINATION reactos/system32 FOR all)
