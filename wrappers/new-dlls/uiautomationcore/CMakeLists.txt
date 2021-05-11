
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/dxsdk)

spec2def(uiautomationcore.dll uiautomationcore.spec)

list(APPEND SOURCE
    uia_main.c
    ${CMAKE_CURRENT_BINARY_DIR}/uiautomationcore_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/uiautomationcore.def)
	
add_library(uiautomationcore SHARED ${SOURCE} uiautomationcore.rc)
#add_dependencies(uiautomationcore uiautomationcore_idlheader stdole2 dxsdk)
set_module_type(uiautomationcore win32dll)
target_link_libraries(uiautomationcore wine uuid)
add_importlibs(uiautomationcore shell32 oleaut32 ole32 user32 advapi32 shlwapi ntdll kernel32 msvcrt)
add_cd_file(TARGET uiautomationcore DESTINATION reactos/system32 FOR all)
