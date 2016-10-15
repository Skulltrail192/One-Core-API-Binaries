
spec2def(storport.sys storport.spec ADD_IMPORTLIB)

add_library(storport SHARED 
			storport.c 
			storport.rc 
			${CMAKE_CURRENT_BINARY_DIR}/storport.def)

set_module_type(storport kernelmodedriver)
add_importlibs(storport ntoskrnl hal)

add_cd_file(TARGET storport DESTINATION reactos/system32/drivers NO_CAB FOR all)
