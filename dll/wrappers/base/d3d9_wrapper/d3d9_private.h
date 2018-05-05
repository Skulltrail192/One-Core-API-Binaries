#include <rtl.h>
#include <winsvc.h>
#include <evntprov.h>
#include <ntsecapi.h>
#include <ndk/cmfuncs.h>
#include <wine/config.h>
#include <d3d9.h>
#include <ddraw.h>

/* IDirect3D9: IUnknown implementation */
static HRESULT WINAPI IDirect3D9ExImpl_QueryInterface(LPDIRECT3D9EX iface, REFIID riid, LPVOID* ppvObject);

static ULONG WINAPI IDirect3D9ExImpl_AddRef(LPDIRECT3D9EX iface);

static ULONG WINAPI IDirect3D9ExImpl_Release(LPDIRECT3D9EX iface);

static UINT WINAPI IDirect3D9ExImpl_GetAdapterModeCountEx(IDirect3D9Ex *iface,
        UINT adapter, const D3DDISPLAYMODEFILTER *filter);

static HRESULT WINAPI IDirect3D9ExImpl_RegisterSoftwareDevice(IDirect3D9Ex *iface, void *init_function);

static UINT WINAPI IDirect3D9ExImpl_GetAdapterModeCount(IDirect3D9Ex * iface, UINT Adapter, D3DFORMAT Format);

static HRESULT WINAPI IDirect3D9ExImpl_GetAdapterIdentifier(IDirect3D9Ex *iface, UINT adapter,
        DWORD flags, D3DADAPTER_IDENTIFIER9 *identifier);
		
static UINT WINAPI IDirect3D9ExImpl_GetAdapterModeCount(IDirect3D9Ex * iface, UINT Adapter, D3DFORMAT Format);	

static HRESULT WINAPI IDirect3D9ExImpl_EnumAdapterModes(IDirect3D9Ex * iface, UINT Adapter, D3DFORMAT Format,
                                                      UINT Mode, D3DDISPLAYMODE* pMode);

static HRESULT WINAPI IDirect3D9ExImpl_GetAdapterDisplayMode(IDirect3D9Ex * iface, UINT Adapter, D3DDISPLAYMODE* pMode);	

static HRESULT WINAPI IDirect3D9ExImpl_CheckDeviceType(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType,
                                                     D3DFORMAT DisplayFormat, D3DFORMAT BackBufferFormat, BOOL Windowed);

static HRESULT WINAPI IDirect3D9ExImpl_CheckDeviceFormat(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType,
                                                       D3DFORMAT AdapterFormat, DWORD Usage, D3DRESOURCETYPE RType,
                                                       D3DFORMAT CheckFormat);

static HRESULT WINAPI IDirect3D9ExImpl_CheckDeviceMultiSampleType(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType,
                                                                D3DFORMAT SurfaceFormat, BOOL Windowed,
                                                                D3DMULTISAMPLE_TYPE MultiSampleType, DWORD* pQualityLevels);

static HRESULT WINAPI IDirect3D9ExImpl_CheckDepthStencilMatch(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType,
                                                            D3DFORMAT AdapterFormat, D3DFORMAT RenderTargetFormat,
                                                            D3DFORMAT DepthStencilFormat);

static HRESULT WINAPI IDirect3D9ExImpl_CheckDeviceFormatConversion(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType,
                                                                 D3DFORMAT SourceFormat, D3DFORMAT TargetFormat);

static HRESULT WINAPI IDirect3D9ExImpl_GetDeviceCaps(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType, D3DCAPS9* pCaps);

static HMONITOR WINAPI IDirect3D9ExImpl_GetAdapterMonitor(IDirect3D9Ex * iface, UINT Adapter);

static HRESULT WINAPI IDirect3D9ExImpl_CreateDevice(IDirect3D9Ex * iface, UINT Adapter, D3DDEVTYPE DeviceType,
                                                  HWND hFocusWindow, DWORD BehaviourFlags,
                                                  D3DPRESENT_PARAMETERS* pPresentationParameters,
                                                  struct IDirect3DDevice9** ppReturnedDeviceInterface);	

static HRESULT WINAPI IDirect3D9ExImpl_GetAdapterDisplayModeEx(IDirect3D9Ex *iface,
        UINT adapter, D3DDISPLAYMODEEX *mode, D3DDISPLAYROTATION *rotation);

static HRESULT WINAPI IDirect3D9ExImpl_EnumAdapterModesEx(IDirect3D9Ex *iface,
        UINT adapter, const D3DDISPLAYMODEFILTER *filter, UINT mode_idx, D3DDISPLAYMODEEX *mode);		
		
static UINT WINAPI IDirect3D9ExImpl_GetAdapterCount(IDirect3D9Ex * iface);		

static HRESULT WINAPI IDirect3D9ExImpl_CreateDeviceEx(IDirect3D9Ex *iface,
        UINT adapter, D3DDEVTYPE device_type, HWND focus_window, DWORD flags,
        D3DPRESENT_PARAMETERS *parameters, D3DDISPLAYMODEEX *mode, IDirect3DDevice9Ex **device);
		
WINUSERAPI BOOL WINAPI EnumDisplaySettingsA(LPCSTR,DWORD,LPDEVMODEA);		
WINUSERAPI BOOL WINAPI EnumDisplaySettingsExA(LPCSTR,DWORD,LPDEVMODEA,DWORD);		

extern struct IDirect3D9ExVtbl Direct3D9_Vtbl;

typedef struct _DIRECT3D9_INT
{
    IDirect3D9Ex IDirect3D9Ex_iface;
    LONG refcount;
	CRITICAL_SECTION d3d9_cs;
	struct IDirect3D9ExVtbl *lpVtbl; /* LPDIRECTD3D9 function table */
	UINT SDKVersion;
    //Direct3D9DisplayAdapterInfo DisplayAdapters[D3D9_INT_MAX_NUM_ADAPTERS];	
    //struct wined3d *wined3d;
    //BOOL extended;
}DIRECT3D9_INT, *LPDIRECT3D9_INT;

static HRESULT WINAPI IDirect3D9ExImpl_GetAdapterLUID(IDirect3D9Ex *iface, UINT adapter, LUID *luid);