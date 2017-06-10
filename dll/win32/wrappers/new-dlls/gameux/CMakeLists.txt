
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
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
target_link_libraries(gameux wine uuid)
add_importlibs(gameux shell32 oleaut32 ole32 user32 advapi32 shlwapi ntdll kernel32 msvcrt)
add_cd_file(TARGET gameux DESTINATION reactos/system32 FOR all)
