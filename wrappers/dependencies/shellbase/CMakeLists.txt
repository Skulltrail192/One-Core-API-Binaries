PROJECT(SHELL)

set_cpp(WITH_RUNTIME)
spec2def(shellbase.dll shellbase.spec ADD_IMPORTLIB)

if(NOT MSVC)
    # HACK: this should be enabled globally!
    add_compile_flags_language("-std=c++11" "CXX")
endif()

remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)

add_definitions(
    -D_SHELL32_
    -D_WINE
    -D_ATL_NO_EXCEPTIONS)

include_directories(
    ${REACTOS_SOURCE_DIR}/sdk/lib/atl
    ${REACTOS_SOURCE_DIR})

list(APPEND SOURCE
    CIDLDataObj.cpp
    CQueryAssociations.cpp
    debughlp.cpp
    dialogs/dialogs.cpp
    dialogs/drive.cpp
    dialogs/drvdefext.cpp
    dialogs/folder_options.cpp
    dialogs/filedefext.cpp
    dialogs/fprop.cpp
    dialogs/recycler_prop.cpp
    CDropTargetHelper.cpp
    CEnumIDListBase.cpp
    CExtractIcon.cpp
    folders.cpp
    iconcache.cpp
    shell32.cpp
    CShellItem.cpp
    CShellLink.cpp
    CFolderOptions.cpp
    folders/CDesktopFolder.cpp
    folders/CFSFolder.cpp
    folders/CDrivesFolder.cpp
    folders/CMyDocsFolder.cpp
    folders/CPrinterFolder.cpp
    folders/CAdminToolsFolder.cpp
    folders/CNetFolder.cpp
    folders/CFontsFolder.cpp
    folders/CControlPanelFolder.cpp
    folders/CRecycleBin.cpp
    folders/CRegFolder.cpp
    droptargets/CexeDropHandler.cpp
    droptargets/CFSDropTarget.cpp
    shlexec.cpp
    shlfileop.cpp
    shlfolder.cpp
    CFileSysBindData.cpp
    CDefView.cpp
    CDefViewDual.cpp
    CDefViewBckgrndMenu.cpp
    stubs.cpp
    systray.cpp
    CDefaultContextMenu.cpp
    COpenWithMenu.cpp
    CNewMenu.cpp
    CShellDispatch.cpp
    CFolder.cpp
    CFolderItems.cpp
    CFolderItemVerbs.cpp
	Unimplemented.cpp
    precomp.h)

file(GLOB_RECURSE shell32_rc_deps res/*.*)
list(APPEND shell32_rc_deps ${CMAKE_CURRENT_BINARY_DIR}/shell32_shldisp.tlb)
add_rc_deps(shellbase.rc ${shell32_rc_deps})

add_library(shellbase SHARED
    ${SOURCE}
    wine/brsfolder.c
    wine/changenotify.c
    wine/classes.c
    wine/clipboard.c
    wine/control.c
    wine/pidl.c
    wine/shell32_main.c
    wine/shellole.c
    wine/shellord.c
    wine/shellpath.c
    wine/shellreg.c
    wine/shellstring.c
    wine/shlmenu.c
    wine/shpolicy.c
    vista.c
    shellbase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/shellbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/shellbase.def)


add_typelib(shell32_shldisp.idl)
set_source_files_properties(shellbase.rc PROPERTIES OBJECT_DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/shell32_shldisp.tlb)

set_module_type(shellbase win32dll UNICODE)
target_link_libraries(shellbase shellmenu shelldesktop wine uuid recyclebin)
add_delay_importlibs(shellbase powrprof shdocvw devmgr winspool winmm mpr uxtheme ole32 oleaut32 userenv browseui version fmifs)
add_importlibs(shellbase advapi32 gdi32 user32 comctl32 comdlg32 shlwapi msvcrt kernel32 ntdll)
add_dependencies(shellbase stdole2) # shell32_shldisp.tlb needs stdole2.tlb
add_pch(shellbase precomp.h SOURCE)
add_cd_file(TARGET shellbase DESTINATION reactos/system32 FOR all)
