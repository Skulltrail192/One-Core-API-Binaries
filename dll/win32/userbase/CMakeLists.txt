
spec2def(userbase.dll userbase.spec ADD_IMPORTLIB)

include_directories(
    ${REACTOS_SOURCE_DIR}/include/reactos/subsys
    include
    ${REACTOS_SOURCE_DIR}/win32ss/include)

list(APPEND SOURCE
    controls/appswitch.c
    controls/button.c
    controls/combo.c
    controls/edit.c
    controls/icontitle.c
    controls/listbox.c
    controls/regcontrol.c
    controls/scrollbar.c
    controls/static.c
    misc/dde.c
    misc/ddeclient.c
    misc/ddeserver.c
    misc/desktop.c
    misc/display.c
    misc/dllmain.c
    misc/exit.c
    misc/exticon.c
    misc/imm.c
    misc/misc.c
    misc/object.c
    misc/resources.c
    misc/rtlstr.c
    misc/stubs.c
    misc/timer.c
    misc/usrapihk.c
    misc/winhelp.c
    misc/winsta.c
    windows/accel.c
    windows/caret.c
    windows/class.c
    windows/clipboard.c
#   windows/cursoricon.c
    windows/dc.c
    windows/defwnd.c
    windows/dialog.c
    windows/draw.c
    windows/font.c
    windows/hook.c
    windows/input.c
    windows/mdi.c
    windows/menu.c
    windows/messagebox.c
    windows/message.c
    windows/nonclient.c
    windows/paint.c
    windows/prop.c
    windows/rect.c
    windows/spy.c
    windows/text.c
    windows/window.c
    windows/winpos.c
    ${CMAKE_CURRENT_BINARY_DIR}/userbase_stubs.c
    include/user32.h)

if(USE_NEW_CURSORICON)
    list(APPEND SOURCE windows/cursoricon_new.c)
else()
    list(APPEND SOURCE windows/cursoricon.c)
endif()

add_library(userbase SHARED
    ${SOURCE}
    userbase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/userbase.def)

set_module_type(userbase win32dll ENTRYPOINT DllMain 12 UNICODE HOTPATCHABLE)
target_link_libraries(userbase user32_wsprintf wine win32ksys ${PSEH_LIB})

if(MSVC)
    # for __ftol2_sse, float to int cast helper
    target_link_libraries(userbase msvcrtex)
endif()

add_delay_importlibs(userbase imm32 usp10)
add_importlibs(userbase gdi32 advapi32 kernel32 ntdll)
add_pch(userbase include/userbase.h SOURCE)
add_cd_file(TARGET userbase DESTINATION reactos/system32 FOR all)
