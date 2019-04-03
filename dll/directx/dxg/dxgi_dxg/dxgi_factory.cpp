/*
 * COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system D3D libraries
 * FILE:            dll/win32/dxgi/dxgi_factory.cpp
 * PURPOSE:         Dxgi Factory class implementation
 * PROGRAMMERS:     Skulltrail
 */

/* INCLUDES ******************************************************************/

#include <dxgi_factory.h>

  DxgiFactory::DxgiFactory(UINT Flags){
	DbgPrint("TODO\n");
  }

  DxgiFactory::~DxgiFactory() {
    
  }
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::QueryInterface(REFIID riid, void** ppvObject) {
    if (ppvObject == nullptr)
      return E_POINTER;

    *ppvObject = nullptr;
    
    if (riid == __uuidof(IUnknown)
     || riid == __uuidof(IDXGIObject)
     || riid == __uuidof(IDXGIFactory)
     || riid == __uuidof(IDXGIFactory1)
     || riid == __uuidof(IDXGIFactory2)
     || riid == __uuidof(IDXGIFactory3)
     || riid == __uuidof(IDXGIFactory4)) {
      *ppvObject = ref(this);
      return S_OK;
    }

    //Logger::warn("DxgiFactory::QueryInterface: Unknown interface query");
    //Logger::warn(str::format(riid));
    return E_NOINTERFACE;
  }  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::GetParent(REFIID riid, void** ppParent) {
    InitReturnPtr(ppParent);
    
    //Logger::warn("DxgiFactory::GetParent: Unknown interface query");
    return E_NOINTERFACE;
  }
  
  
  BOOL STDMETHODCALLTYPE DxgiFactory::IsWindowedStereoEnabled() {
    // We don't support Stereo 3D at the moment
    return FALSE;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::CreateSoftwareAdapter(
          HMODULE         Module,
          IDXGIAdapter**  ppAdapter) {
    return DXGI_ERROR_UNSUPPORTED;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::CreateSwapChain(
          IUnknown*             pDevice,
          DXGI_SWAP_CHAIN_DESC* pDesc,
          IDXGISwapChain**      ppSwapChain) {
    return DXGI_ERROR_UNSUPPORTED;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::CreateSwapChainForHwnd(
          IUnknown*             pDevice,
          HWND                  hWnd,
    const DXGI_SWAP_CHAIN_DESC1* pDesc,
    const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* pFullscreenDesc,
          IDXGIOutput*          pRestrictToOutput,
          IDXGISwapChain1**     ppSwapChain) {
    return DXGI_ERROR_UNSUPPORTED;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::CreateSwapChainForCoreWindow(
          IUnknown*             pDevice,
          IUnknown*             pWindow,
    const DXGI_SWAP_CHAIN_DESC1* pDesc,
          IDXGIOutput*          pRestrictToOutput,
          IDXGISwapChain1**     ppSwapChain) {
    InitReturnPtr(ppSwapChain);
    
    //Logger::err("DxgiFactory::CreateSwapChainForCoreWindow: Not implemented");
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::CreateSwapChainForComposition(
          IUnknown*             pDevice,
    const DXGI_SWAP_CHAIN_DESC1* pDesc,
          IDXGIOutput*          pRestrictToOutput,
          IDXGISwapChain1**     ppSwapChain) {
    InitReturnPtr(ppSwapChain);
    
    //Logger::err("DxgiFactory::CreateSwapChainForComposition: Not implemented");
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::EnumAdapters(
          UINT            Adapter,
          IDXGIAdapter**  ppAdapter) {
    return hr;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::EnumAdapters1(
          UINT            Adapter,
          IDXGIAdapter1** ppAdapter) {
    return DXGI_ERROR_UNSUPPORTED;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::EnumAdapterByLuid(
          LUID                  AdapterLuid,
          REFIID                riid,
          void**                ppvAdapter) {
    return E_NOTIMPL;
  }

  
  HRESULT STDMETHODCALLTYPE DxgiFactory::EnumWarpAdapter(
          REFIID                riid,
          void**                ppvAdapter) {
    return E_NOTIMPL;
  }


  HRESULT STDMETHODCALLTYPE DxgiFactory::GetWindowAssociation(HWND *pWindowHandle) {
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::GetSharedResourceAdapterLuid(
          HANDLE                hResource,
          LUID*                 pLuid) {
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::MakeWindowAssociation(HWND WindowHandle, UINT Flags) {
    //Logger::warn("DXGI: MakeWindowAssociation: Ignoring flags");
    return S_OK;
  }
  
  
  BOOL STDMETHODCALLTYPE DxgiFactory::IsCurrent() {
    return TRUE;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::RegisterOcclusionStatusWindow(
          HWND                  WindowHandle,
          UINT                  wMsg,
          DWORD*                pdwCookie) {
    //Logger::err("DxgiFactory::RegisterOcclusionStatusWindow: Not implemented");
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::RegisterStereoStatusEvent(
          HANDLE                hEvent,
          DWORD*                pdwCookie) {
    return E_NOTIMPL;
  }
  
  
  HRESULT STDMETHODCALLTYPE DxgiFactory::RegisterStereoStatusWindow(
          HWND                  WindowHandle,
          UINT                  wMsg,
          DWORD*                pdwCookie) {
    return E_NOTIMPL;
  }
  

  HRESULT STDMETHODCALLTYPE DxgiFactory::RegisterOcclusionStatusEvent(
          HANDLE                hEvent,
          DWORD*                pdwCookie) {
    return E_NOTIMPL;
  }
  

  void STDMETHODCALLTYPE DxgiFactory::UnregisterStereoStatus(
          DWORD                 dwCookie) {
    return E_NOTIMPL;
  }
  
  
  void STDMETHODCALLTYPE DxgiFactory::UnregisterOcclusionStatus(
          DWORD                 dwCookie) {
    //Logger::err("DxgiFactory::UnregisterOcclusionStatus: Not implemented");
  }


  UINT STDMETHODCALLTYPE DxgiFactory::GetCreationFlags() {
    return m_flags;
  }  

