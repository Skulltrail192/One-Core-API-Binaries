
add_definitions(-D__WINESRC__)
remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x602)	
include_directories(${REACTOS_SOURCE_DIR}/include/host)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/win32k)
include_directories(${REACTOS_SOURCE_DIR}/include/reactos/subsys/csr)
include_directories(${REACTOS_SOURCE_DIR}/subsystems/win32/win32k)
include_directories(${REACTOS_SOURCE_DIR}/win32ss/user/winsrv/usersrv)
include_directories(${REACTOS_SOURCE_DIR}/win32ss/include)

spec2def(vser32.dll vser32.spec)

list(APPEND SOURCE
	dib.c
	display.c
	dwm.c
	icon.c
	magnification
    main.c
	misc.c
	paint.c
	process.c
	shutdown.c
	syscall.c
	touch.c
	window.c
    version.rc
    ${CMAKE_CURRENT_BINARY_DIR}/vser32_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/vser32.def)

add_library(vser32 SHARED ${SOURCE})
set_module_type(vser32 win32dll ENTRYPOINT 0)

target_link_libraries(vser32 wine)

add_importlibs(vser32 userbase kernel32 gdi32 ntdll advapi32 winsrv)

add_cd_file(TARGET vser32 DESTINATION reactos/system32 FOR all)
