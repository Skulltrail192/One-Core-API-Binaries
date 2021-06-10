
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)

spec2def(sppc.dll sppc.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    sppc.c
	version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/sppc_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/sppc.def)
	
add_library(sppc SHARED ${SOURCE})
set_module_type(sppc win32dll)
target_link_libraries(sppc wine uuid)
add_importlibs(sppc shell32 gdi32 oleaut32 ole32 user32 advapi32 shlwapi ntdll kernel32 msvcrt)
add_cd_file(TARGET sppc DESTINATION reactos/system32 FOR all)
