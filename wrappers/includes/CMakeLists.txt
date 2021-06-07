
add_definitions(-D_MIDL_USE_GUIDDEF_)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND SOURCE
	dxva2api.idl
	evr9.idl
	mfidl.idl
	mftransform.idl
	)

add_idl_headers(wsdk ${SOURCE})
add_dependencies(wsdk xdk)