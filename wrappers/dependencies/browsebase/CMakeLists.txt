
set_cpp(WITH_RUNTIME)

remove_definitions(-D_WIN32_WINNT=0x502)
add_definitions(-D_WIN32_WINNT=0x600)

include_directories(${REACTOS_SOURCE_DIR}/sdk/lib/atl)
include_directories(${REACTOS_SOURCE_DIR}/sdk/include/reactos)

spec2def(browsebase.dll browsebase.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    aclmulti.cpp
    addressband.cpp
    addresseditbox.cpp
    bandproxy.cpp
    bandsite.cpp
    bandsitemenu.cpp
    basebar.cpp
    basebarsite.cpp
    brandband.cpp
    browseui.cpp
    browseuiord.cpp
    commonbrowser.cpp
    globalfoldersettings.cpp
    internettoolbar.cpp
    regtreeoptions.cpp
    shellbrowser.cpp
    toolsband.cpp
    travellog.cpp
    utility.cpp
    precomp.h)

add_library(browsebase SHARED
    ${SOURCE}
    dllinstall.c
    browsebase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/browsebase.def)

set_module_type(browsebase win32dll UNICODE)

target_link_libraries(browsebase    
    uuid
    wine)

add_importlibs(browsebase
    shlwapi
    shell32
    comctl32
    gdi32
    ole32
    oleaut32
    user32
    advapi32
    msvcrt
    kernel32
    ntdll)

add_pch(browsebase precomp.h SOURCE)
add_cd_file(TARGET browsebase DESTINATION reactos/system32 FOR all)

if(NOT MSVC)
    target_compile_options(browseui PRIVATE "-Wno-unused-but-set-variable")
    # Binutils linker bug
    if(LTCG)
        add_target_link_flags(browseui "-Wl,--allow-multiple-definition")
    endif()
endif()

add_custom_command(TARGET browsebase POST_BUILD 
  COMMAND "${CMAKE_COMMAND}" -E copy 
     "$<TARGET_FILE:browseui>"
     "$<TARGET_FILE_DIR:filebrowser>/$<TARGET_FILE_NAME:browseui>" 
  COMMENT "Copying to output directory")
