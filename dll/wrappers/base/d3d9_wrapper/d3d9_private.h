#include <rtl.h>
#include <winsvc.h>
#include <evntprov.h>
#include <ntsecapi.h>
#include <ndk/cmfuncs.h>
#include <wine/config.h>
#include <d3d9.h>
#include <ddraw.h>

/* IDirect3D9: IUnknown implementation */
static HRESULT WINAPI IDirect3D9Impl_QueryInterface(LPDIRECT3D9 iface, REFIID riid, LPVOID* ppvObject);

static ULONG WINAPI IDirect3D9Impl_AddRef(LPDIRECT3D9 iface);

static ULONG WINAPI IDirect3D9Impl_Release(LPDIRECT3D9 iface);