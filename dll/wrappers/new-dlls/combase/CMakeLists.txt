
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
spec2def(combase.dll combase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
	roapi.c
	string.c
    combase.rc
	${CMAKE_CURRENT_BINARY_DIR}/combase_stubs.c
	${CMAKE_CURRENT_BINARY_DIR}/combase.def
	)

add_library(combase SHARED ${SOURCE})

set_module_type(combase win32dll UNICODE)
target_link_libraries(combase uuid activation_uuids wine ${PSEH_LIB})
add_delay_importlibs(combase winmm uxtheme)
add_importlibs(combase user32 gdi32 advapi32 msvcrt kernel32 ole32 ntdll)
add_cd_file(TARGET combase DESTINATION reactos/system32 FOR all)
