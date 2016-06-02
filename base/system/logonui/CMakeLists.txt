
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys)


list(APPEND SOURCE
)

add_executable(logonui WIN32 ${SOURCE} logonui.rc)
target_link_libraries(logonui nt ${PSEH_LIB} smlib)
set_module_type(logonui nativecui)
add_importlibs(logonui ntdll)
add_pch(logonui logonui.h SOURCE)
add_cd_file(TARGET logonui DESTINATION reactos/system32 FOR all)
