
include_directories(BEFORE ${REACTOS_SOURCE_DIR}/sdk/include/reactos/wine)
include_directories(${REACTOS_SOURCE_DIR}/sdk/lib/crt/include)
spec2def(ucrtbase.dll ucrtbase.spec ADD_IMPORTLIB)

add_definitions(
    -DUSE_MSVCRT_PREFIX
    -D_MSVCRT_
    -D_MSVCRT_LIB_
    -D_MT
    -D_CTYPE_DISABLE_MACROS
    -D_NO_INLINING
    -DCRTDLL
    -D__MINGW_IMPORT="")

list(APPEND SOURCE
	main.c
    ucrtbase.rc
    ${CMAKE_CURRENT_BINARY_DIR}/ucrtbase_stubs.c
    ${CMAKE_CURRENT_BINARY_DIR}/ucrtbase.def)

add_library(ucrtbase SHARED ${SOURCE})

set_module_type(ucrtbase win32dll ENTRYPOINT 0)
add_dependencies(ucrtbase psdk)
target_link_libraries(ucrtbase wine ${PSEH_LIB})

if(MSVC)
    # export of deleting destructor "name"
    add_target_link_flags(msvcrt "/ignore:4102")
    add_target_link_flags(libmsvcrt "/ignore:4102")
endif()

add_importlibs(ucrtbase kernel32 msvcrt ntdll)
add_cd_file(TARGET ucrtbase DESTINATION reactos/system32 FOR all)
