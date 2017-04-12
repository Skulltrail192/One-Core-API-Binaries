
spec2def(vdh.dll vdh.spec)

list(APPEND SOURCE
    pdh_main.c
	vdh.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vdh_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vdh.def)

add_library(vdh SHARED ${SOURCE})
set_module_type(vdh win32dll ENTRYPOINT 0)
add_importlibs(vdh pdhbase msvcrt kernel32 ntdll)
target_link_libraries(vdh wine)
add_cd_file(TARGET vdh DESTINATION reactos/system32 FOR all)

spec2def(vpowrprof.dll vpowrprof.spec)


