
add_definitions(
    -D_NTHALDLL_
    -D_NTHAL_)
spec2def(halext.dll halext.spec)

include_directories(
    include
    ${REACTOS_SOURCE_DIR}/ntoskrnl/include)

list(APPEND SOURCE
	main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/halext_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/halext.def)

add_library(halext SHARED ${SOURCE})
set_module_type(halext kerneldll ENTRYPOINT HalInitSystem 8)
target_link_libraries(halext libcntpr)
add_dependencies(${_halname} psdk asm)
add_importlibs(halext halaacpi ntoskrnl kdcom)
add_cd_file(TARGET halext DESTINATION reactos/system32 FOR all)
