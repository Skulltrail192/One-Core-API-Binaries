
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)

spec2def(gameux.dll gameux.spec)

list(APPEND SOURCE
    factory.c
	gameexplorer.c
	gamestatistics.c
	main.c
    ${CMAKE_CURRENT_BINARY_DIR}/gameux_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/gameux.def)

add_library(gameux SHARED ${SOURCE} version.rc)
set_module_type(gameux win32dll)
target_link_libraries(gameux wine)
add_importlibs(gameux uuid shell32 oleaut32 ole32 user32 advapi32 shlwapi)
add_cd_file(TARGET gameux DESTINATION reactos/system32 FOR all)
