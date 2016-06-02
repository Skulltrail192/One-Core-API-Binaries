
add_definitions(-D_vdvapiXP_)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/lib/rtl)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/ntdllnew)
include_directories(${REACTOS_SOURCE_DIR}/dll/win32/kernel32/include)

spec2def(vdvapiXP.dll vdvapiXP.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    main.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vdvapiXP_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vdvapiXP.def)

add_library(vdvapiXP SHARED ${SOURCE})
set_module_type(vdvapiXP win32dll ENTRYPOINT 0)

target_link_libraries(vdvapiXP wine)

add_importlibs(vdvapiXP msvcrt kernel32 ntdll advapibase advapinew ntdllnew)

add_cd_file(TARGET vdvapiXP DESTINATION reactos/system32 FOR all)
