
spec2def(pdh_wrapper.dll pdh_wrapper.spec)

list(APPEND SOURCE
    pdh_main.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/pdh_wrapper_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/pdh_wrapper.def)

add_library(pdh_wrapper SHARED ${SOURCE})
set_module_type(pdh_wrapper win32dll ENTRYPOINT 0)
add_importlibs(pdh_wrapper pdhbase msvcrt kernel32 ntdll)
target_link_libraries(pdh_wrapper wine)
add_cd_file(TARGET pdh_wrapper DESTINATION reactos/system32 FOR all)

spec2def(pdh_wrapper.dll pdh_wrapper.spec)


