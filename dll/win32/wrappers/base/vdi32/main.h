#include <wine/config.h>

#include <ntstatus.h>
#define WIN32_NO_STATUS

#include <wine/debug.h>

#include <winbase.h>
#include <ntsecapi.h>
#include <bcrypt.h>
#include <wingdi.h>
#include <math.h>
#include <d3dkmthk.h>
#include "wingdi.h"
#include <limits.h>
#include <winuser.h>


// Purpose: Video present source unique identification number descriptor type
//

typedef UINT D3DDDI_VIDEO_PRESENT_SOURCE_ID;

typedef HANDLE D3DKMT_HANDLE;

typedef struct _D3DKMT_CLOSEADAPTER {
  D3DKMT_HANDLE hAdapter;
} D3DKMT_CLOSEADAPTER;

typedef struct _D3DKMT_OPENADAPTERFROMDEVICENAME {
  PCWSTR        pDeviceName;
  D3DKMT_HANDLE hAdapter;
  LUID          AdapterLuid;
} D3DKMT_OPENADAPTERFROMDEVICENAME;

typedef struct _D3DKMT_CHECKMONITORPOWERSTATE {
  D3DKMT_HANDLE                  hAdapter;
  D3DDDI_VIDEO_PRESENT_SOURCE_ID VidPnSourceId;
} D3DKMT_CHECKMONITORPOWERSTATE;

typedef struct _D3DKMDT_VIDEO_PRESENT_SOURCE {
  D3DDDI_VIDEO_PRESENT_SOURCE_ID Id;
  DWORD                          dwReserved;
} D3DKMDT_VIDEO_PRESENT_SOURCE;

typedef struct gdi_physdev
{
     const struct gdi_dc_funcs *funcs;
     struct gdi_physdev        *next;
     HDC                        hdc;
} *PHYSDEV;

/* Undocumented structure filled in by GetFontRealizationInfo */
struct font_realization_info
{
    DWORD size;        /* could be 16 or 24 */
    DWORD flags;       /* 1 for bitmap fonts, 3 for scalable fonts */
    DWORD cache_num;   /* keeps incrementing - num of fonts that have been created allowing for caching?? */
    DWORD instance_id; /* identifies a realized font instance */
    DWORD unk;         /* unknown */
    WORD  face_index;  /* face index in case of font collections */
    WORD  simulations; /* 0 bit - bold simulation, 1 bit - oblique simulation */
};