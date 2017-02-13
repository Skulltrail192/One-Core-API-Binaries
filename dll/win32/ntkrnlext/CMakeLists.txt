
spec2def(ntkrnlext.dll ntkrnlext.spec)

list(APPEND SOURCE
    ntkrnlext.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ntkrnlext_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ntkrnlext.def)

add_library(ntkrnlext SHARED ${SOURCE})

set_module_type(ntkrnlext win32dll ENTRYPOINT 0)
add_importlibs(ntkrnlext ntoskrnl)
add_dependencies(ntkrnlext psdk)
add_cd_file(TARGET ntkrnlext DESTINATION reactos/system32 FOR all)
