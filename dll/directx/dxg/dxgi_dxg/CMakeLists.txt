
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)

set_cpp(WITH_RUNTIME WITH_EXCEPTIONS)
spec2def(dxgi_dxg.dll dxgi.spec ADD_IMPORTLIB)

list(APPEND SOURCE
    dxgi_main.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/dxgi_dxg.def)

if(MSVC)
    add_compile_flags("/Zc:wchar_t-")
endif()

add_library(dxgi_dxg SHARED ${SOURCE})
set_module_type(dxgi_dxg win32dll UNICODE)
add_importlibs(dxgi_dxg oleaut32 msvcrt kernel32 ntdll)
add_cd_file(TARGET dxgi_dxg DESTINATION reactos/system32/wbem FOR all)