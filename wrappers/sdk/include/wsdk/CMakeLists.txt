
add_definitions(-D_MIDL_USE_GUIDDEF_)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND SOURCE
	activation.idl
	d3d10.idl 
	d3d10_1.idl 
	d3d10sdklayers.idl 
	d3d11sdklayers.idl 
	d3d11.idl 
	dxgi.idl 
	dxgicommon.idl 	
	dxgiformat.idl	
	dxgitype.idl 	
	dxva2api.idl
	evr.idl 	
	evr9.idl
	gameux.idl
	hstring.idl
	inspectable.idl
	irpcss.idl
	mfmediaengine.idl 
	mfplay.idl 
	mfreadwrite.idl 
	orpc.idl 	
	restrictederrorinfo.idl 	
	roparameterizediid.idl 	
	rtworkq.idl
	schrpc.idl
	uiautomationclient.idl
	uiautomationcore.idl
	uiribbon.idl
	wpcapi.idl
	)

add_idl_headers(wsdk ${SOURCE})
add_dependencies(wsdk xdk)