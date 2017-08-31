
spec2def(powrprofbase.dll powrprofbase.spec ADD_IMPORTLIB)


add_library(powrprofbase SHARED
    powrprofbase.c
    powrprofbase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/powrprofbase.def)

set_module_type(powrprofbase win32dll UNICODE)

target_link_libraries(powrprofbase wine)

add_importlibs(powrprofbase advapi32 user32 comctl32 msvcrt kernel32 ntdll)

add_cd_file(TARGET powrprofbase DESTINATION reactos/system32 FOR all)
