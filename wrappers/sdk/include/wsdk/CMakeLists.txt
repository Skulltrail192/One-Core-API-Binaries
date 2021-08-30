
add_definitions(-D_MIDL_USE_GUIDDEF_)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND SOURCE
	activation.idl
	d2d1.idl
	d2d1_1.idl
	d2d1_2.idl
	d3d10.idl 
	d3d10_1.idl 
	d3d10sdklayers.idl 
	d3d11.idl 
	d3d11_1.idl 
	d3d11_2.idl 
	d3d11_3.idl 
	d3d11_4.idl 
	d3d11sdklayers.idl 
	d3d12.idl 
	d3d12sdklayers.idl 
	d3d12shader.idl 
	d3dcommon.idl 
	dcommon.idl 
	dxgi.idl 
	dxgi1_2.idl 
	dxgi1_3.idl 
	dxgi1_4.idl 
	dxgi1_5.idl 
	dxgi1_6.idl 
	dxgicommon.idl 	
	dxgidebug.idl 	
	dxgiformat.idl	
	dxgitype.idl 	
	dxva2api.idl
	dwrite.idl
	dwrite_1.idl
	dwrite_2.idl
	dwrite_3.idl
	evr.idl 	
	evr9.idl
	gameux.idl
	hstring.idl
	inspectable.idl
	irpcss.idl
	mfmediaengine.idl 
	mfplay.idl 
	mfreadwrite.idl 
	netiodefwine.h
	opcbase.idl 	
	opcobjectmodel.idl 	
	opcparturi.idl 	
	orpc.idl 	
	msopc.idl 	
	restrictederrorinfo.idl 	
	roparameterizediid.idl 	
	rtworkq.idl
	schrpc.idl
	uianimation.idl
	uiautomationclient.idl
	uiautomationcore.idl
	uiribbon.idl
	winedxgi.idl
	wpcapi.idl
	)
	
add_iid_library(dxgi_uuids dxgi.idl dxgi1_2.idl dxgi1_3.idl dxgi1_4.idl dxgi1_5.idl dxgi1_6.idl d3d12.idl dxgi_internal.idl)	
add_iid_library(d2d1_uuids d2d1.idl d2d1_1.idl d2d1_2.idl)
add_iid_library(d3d10_uuids d3d10.idl d3d10_1.idl d3d10sdklayers.idl)
add_iid_library(d3d10_1_uuids d3d10_1.idl)
add_iid_library(d3d11_uuids d3d11.idl d3d11_1.idl d3d11_2.idl d3d11_3.idl d3d11_4.idl d3d11sdklayers.idl)
add_iid_library(d3d12_uuids d3d12.idl d3d12sdklayers.idl d3d12shader.idl)
add_iid_library(dwrite_uuids dwrite.idl dwrite_1.idl dwrite_2.idl dwrite_3.idl)	

add_idl_headers(wsdk ${SOURCE})
add_dependencies(wsdk xdk)