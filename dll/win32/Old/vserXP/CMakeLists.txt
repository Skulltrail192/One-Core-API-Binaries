
add_definitions(-D__WINESRC__)
include_directories(${REACTOS_SOURCE_DIR}/include/host)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/win32k)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/csr)
include_directories(${REACTOS_SOURCE_DIR}/subsystems/win32/win32k)
include_directories(${REACTOS_SOURCE_DIR}/win32ss/user/winsrv/usersrv)

spec2def(vserXP.dll vserXP.spec)

list(APPEND SOURCE
	display.c
	dwm.c
	icon.c
	magnification
    main.c
	misc.c
	process.c
	shutdown.c
	syscall.c
	touch.c
	window.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vserXP_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vserXP.def)

add_library(vserXP SHARED ${SOURCE})
set_module_type(vserXP win32dll)

target_link_libraries(vserXP wine)

add_importlibs(vserXP userbase msvcrt kernel32 advapi32 gdi32 ntdll winsrv)

add_cd_file(TARGET vserXP DESTINATION reactos/system32 FOR all)
