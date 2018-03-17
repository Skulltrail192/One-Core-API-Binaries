
add_library(dxguid dxguid-mingw.c)
add_library(dx10guid dx10guid.c)
add_library(dx9guid dx9guid.c)
add_library(dx8guid dx8guid.c)

add_dependencies(dxguid psdk)
add_dependencies(dx10guid psdk d3d_idl_headers)
