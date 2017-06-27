/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 */

#define _USER32_
#define OEMRESOURCE
#define NTOS_MODE_USER 
#define WIN32_NO_STATUS
#define _WMI_SOURCE_
#include <windows.h>
#include <accctrl.h>
#include <aclapi.h>
#include <wmistr.h>
#include <evntrace.h>
#include <winsafer.h>
#include <sddl.h>
#define NTOS_MODE_USER
#include <ndk/cmfuncs.h>
#include <ndk/exfuncs.h>
#include <ndk/iofuncs.h>
#include <ndk/kefuncs.h>
#include <ndk/obfuncs.h>
#include <ndk/psfuncs.h>
#include <ndk/rtlfuncs.h>
#include <ndk/setypes.h>
#include <ndk/sefuncs.h>
#include <evntprov.h>
#include <csr.h>
#include <strsafe.h>
#include <wine/debug.h>
#include <csrsrv.h>
#include <api.h>
#include <ntuser.h>
#include <winuser.h>

#define WH_MIN   (-1)
#define WH_MAX   14
#define WH_MINHOOK   WH_MIN
#define WH_MAXHOOK   WH_MAX
#define NB_HOOKS (WH_MAXHOOK-WH_MINHOOK+1)

typedef void *(WINAPI *RegisterCallback)(SIZE_T sizeOne, SIZE_T sizeTwo, int flags);

static RegisterCallback gpICSProc = NULL;

static BOOL gfdDPIProcess = FALSE;

static PVOID W32UserProbeAddress = 0;

static HDC ghdcBits2 = 0;

BITMAPINFOHEADER globalInfoHeader;

static int gfSystemInitialized = 0;

static const WCHAR string = 0;

static int InitDst[32];

static int gpidLogon = 0;

static PVOID grpdeskLogon = 0;

LIST_ENTRY gpqForeground;

BITMAPINFO *ScaleProc;

BITMAPINFO *gpsi;

BITMAPINFOHEADER pbmih;

RTL_CRITICAL_SECTION CriticalSectionObject;

PRTL_CRITICAL_SECTION gcsHdc;

LPCRITICAL_SECTION lpCriticalSection;

HANDLE pUserHeap;

typedef struct tagMAGCOLOREFFECT;

struct tagMAGCOLOREFFECT *pEffectGlobal;

typedef HANDLE HPOWERNOTIFY;

typedef HANDLE HTOUCHINPUT;

typedef HANDLE HGESTUREINFO; 

typedef struct _PROCESSINFO *PPROCESSINFO;

typedef struct _D3DMATRIX {
    union {
        struct {
            float        _11, _12, _13, _14;
            float        _21, _22, _23, _24;
            float        _31, _32, _33, _34;
            float        _41, _42, _43, _44;

        };
        float m[4][4];
    };
} D3DMATRIX;

typedef enum _ORIENTATION_PREFERENCE { 
  ORIENTATION_PREFERENCE_NONE               = 0x0,
  ORIENTATION_PREFERENCE_LANDSCAPE          = 0x1,
  
    ORIENTATION_PREFERENCE_PORTRAIT      = 0x2,
  ORIENTATION_PREFERENCE_LANDSCAPE_FLIPPED  = 0x4,
  ORIENTATION_PREFERENCE_PORTRAIT_FLIPPED   = 0x8
} ORIENTATION_PREFERENCE;

ORIENTATION_PREFERENCE Orientation;

typedef enum tagFEEDBACK_TYPE { 
  FEEDBACK_TOUCH_CONTACTVISUALIZATION  = 1,
  FEEDBACK_PEN_BARRELVISUALIZATION     = 2,
  FEEDBACK_PEN_TAP                     = 3,
  FEEDBACK_PEN_DOUBLETAP               = 4,
  FEEDBACK_PEN_PRESSANDHOLD            = 5,
  FEEDBACK_PEN_RIGHTTAP                = 6,
  FEEDBACK_TOUCH_TAP                   = 7,
  FEEDBACK_TOUCH_DOUBLETAP             = 8,
  FEEDBACK_TOUCH_PRESSANDHOLD          = 9,
  FEEDBACK_TOUCH_RIGHTTAP              = 10,
  FEEDBACK_GESTURE_PRESSANDTAP         = 11,
  FEEDBACK_MAX                         = 0xFFFFFFFF
} FEEDBACK_TYPE ;

typedef enum tagPOINTER_INPUT_TYPE { 
  PT_POINTER  = 0x00000001,
  PT_TOUCH    = 0x00000002,
  PT_PEN      = 0x00000003,
  PT_MOUSE    = 0x00000004
} POINTER_INPUT_TYPE;

typedef struct tagMAGCOLOREFFECT {
  float transform[5][5];
} MAGCOLOREFFECT, *PMAGCOLOREFFECT;

typedef struct _TOUCHINPUT {
  LONG      x;
  LONG      y;
  HANDLE    hSource;
  DWORD     dwID;
  DWORD     dwFlags;
  DWORD     dwMask;
  DWORD     dwTime;
  ULONG_PTR dwExtraInfo;
  DWORD     cxContact;
  DWORD     cyContact;
} TOUCHINPUT, *PTOUCHINPUT;

typedef struct _GESTURECONFIG {
  DWORD dwID;
  DWORD dwWant;
  DWORD dwBlock;
} GESTURECONFIG, *PGESTURECONFIG;

typedef struct _GESTUREINFO {
  UINT      cbSize;
  DWORD     dwFlags;
  DWORD     dwID;
  HWND      hwndTarget;
  POINTS    ptsLocation;
  DWORD     dwInstanceID;
  DWORD     dwSequenceID;
  ULONGLONG ullArguments;
  UINT      cbExtraArgs;
} GESTUREINFO, *PGESTUREINFO;

typedef enum tagINPUT_MESSAGE_DEVICE_TYPE { 
  IMDT_UNAVAILABLE  = 0, // 0x0
  IMDT_KEYBOARD     = 1, // 0x1
  IMDT_MOUSE        = 2, // 0x2
  IMDT_TOUCH        = 4, // 0x4
  IMDT_PEN          = 8 // 0x8
} INPUT_MESSAGE_DEVICE_TYPE;

typedef enum tagINPUT_MESSAGE_ORIGIN_ID { 
  IMO_UNAVAILABLE  = 0x00000000,
  IMO_HARDWARE     = 0x00000001,
  IMO_INJECTED     = 0x00000002,
  IMO_SYSTEM       = 0x00000004
} INPUT_MESSAGE_ORIGIN_ID;

typedef struct tagINPUT_MESSAGE_SOURCE {
  INPUT_MESSAGE_DEVICE_TYPE deviceType;
  INPUT_MESSAGE_ORIGIN_ID   originId;
} INPUT_MESSAGE_SOURCE;

typedef struct tagCHANGEFILTERSTRUCT {
  DWORD cbSize;
  DWORD ExtStatus;
} CHANGEFILTERSTRUCT, *PCHANGEFILTERSTRUCT;

typedef enum  { 
  DISPLAYCONFIG_MODE_INFO_TYPE_SOURCE        = 1,
  DISPLAYCONFIG_MODE_INFO_TYPE_TARGET        = 2,
  DISPLAYCONFIG_MODE_INFO_TYPE_FORCE_UINT32  = 0xFFFFFFFF
} DISPLAYCONFIG_MODE_INFO_TYPE;

typedef struct DISPLAYCONFIG_RATIONAL {
  UINT32 Numerator;
  UINT32 Denominator;
} DISPLAYCONFIG_RATIONAL;

typedef struct DISPLAYCONFIG_2DREGION {
  UINT32 cx;
  UINT32 cy;
} DISPLAYCONFIG_2DREGION;

typedef enum  { 
  DISPLAYCONFIG_SCANLINE_ORDERING_UNSPECIFIED                 = 0,
  DISPLAYCONFIG_SCANLINE_ORDERING_PROGRESSIVE                 = 1,
  DISPLAYCONFIG_SCANLINE_ORDERING_INTERLACED                  = 2,
  DISPLAYCONFIG_SCANLINE_ORDERING_INTERLACED_UPPERFIELDFIRST  = DISPLAYCONFIG_SCANLINE_ORDERING_INTERLACED,
  DISPLAYCONFIG_SCANLINE_ORDERING_INTERLACED_LOWERFIELDFIRST  = 3,
  DISPLAYCONFIG_SCANLINE_ORDERING_FORCE_UINT32                = 0xFFFFFFFF
} DISPLAYCONFIG_SCANLINE_ORDERING;

typedef struct DISPLAYCONFIG_VIDEO_SIGNAL_INFO {
  UINT64                          pixelRate;
  DISPLAYCONFIG_RATIONAL          hSyncFreq;
  DISPLAYCONFIG_RATIONAL          vSyncFreq;
  DISPLAYCONFIG_2DREGION          activeSize;
  DISPLAYCONFIG_2DREGION          totalSize;
  union {
    struct {
      UINT32 videoStandard  :16;
      UINT32 vSyncFreqDivider  :6;
      UINT32 reserved  :10;
    } AdditionalSignalInfo;
    UINT32 videoStandard;
  };
  DISPLAYCONFIG_SCANLINE_ORDERING scanLineOrdering;
} DISPLAYCONFIG_VIDEO_SIGNAL_INFO;

typedef struct DISPLAYCONFIG_TARGET_MODE {
  DISPLAYCONFIG_VIDEO_SIGNAL_INFO targetVideoSignalInfo;
} DISPLAYCONFIG_TARGET_MODE;

typedef enum  { 
  DISPLAYCONFIG_TOPOLOGY_INTERNAL      = 0x00000001,
  DISPLAYCONFIG_TOPOLOGY_CLONE         = 0x00000002,
  DISPLAYCONFIG_TOPOLOGY_EXTEND        = 0x00000004,
  DISPLAYCONFIG_TOPOLOGY_EXTERNAL      = 0x00000008,
  DISPLAYCONFIG_TOPOLOGY_FORCE_UINT32  = 0xFFFFFFFF
} DISPLAYCONFIG_TOPOLOGY_ID;

// typedef struct _DESKTOPINFO
 // {
     // PVOID pvDesktopBase;
     // PVOID pvDesktopLimit;
     // struct _WND *spwnd;
     // DWORD fsHooks;
     // LIST_ENTRY aphkStart[NB_HOOKS];
 
     // HWND hTaskManWindow;
     // HWND hProgmanWindow;
     // HWND hShellWindow;
     // struct _WND * spwndShell;
 
     // PPROCESSINFO ppiShellProcess;
 
     // union
     // {
         // UINT Dummy;
         // struct
         // {
             // UINT LastInputWasKbd : 1;
         // };
     // };
 
     // WCHAR szDesktopName[1];
 // } DESKTOPINFO, *PDESKTOPINFO;

typedef enum  { 
  DISPLAYCONFIG_PIXELFORMAT_8BPP          = 1,
  DISPLAYCONFIG_PIXELFORMAT_16BPP         = 2,
  DISPLAYCONFIG_PIXELFORMAT_24BPP         = 3,
  DISPLAYCONFIG_PIXELFORMAT_32BPP         = 4,
  DISPLAYCONFIG_PIXELFORMAT_NONGDI        = 5,
  DISPLAYCONFIG_PIXELFORMAT_FORCE_UINT32  = 0xffffffff
} DISPLAYCONFIG_PIXELFORMAT;

typedef struct DISPLAYCONFIG_SOURCE_MODE {
  UINT32                    width;
  UINT32                    height;
  DISPLAYCONFIG_PIXELFORMAT pixelFormat;
  POINTL                    position;
} DISPLAYCONFIG_SOURCE_MODE;

typedef struct DISPLAYCONFIG_MODE_INFO {
  DISPLAYCONFIG_MODE_INFO_TYPE infoType;
  UINT32                       id;
  LUID                         adapterId;
  union {
    DISPLAYCONFIG_TARGET_MODE targetMode;
    DISPLAYCONFIG_SOURCE_MODE sourceMode;
  };
} DISPLAYCONFIG_MODE_INFO;

typedef enum  { 
  DISPLAYCONFIG_SCALING_IDENTITY                = 1,
  DISPLAYCONFIG_SCALING_CENTERED                = 2,
  DISPLAYCONFIG_SCALING_STRETCHED               = 3,
  DISPLAYCONFIG_SCALING_ASPECTRATIOCENTEREDMAX  = 4,
  DISPLAYCONFIG_SCALING_CUSTOM                  = 5,
  DISPLAYCONFIG_SCALING_PREFERRED               = 128,
  DISPLAYCONFIG_SCALING_FORCE_UINT32            = 0xFFFFFFFF
} DISPLAYCONFIG_SCALING;

typedef enum  { 
  DISPLAYCONFIG_ROTATION_IDENTITY      = 1,
  DISPLAYCONFIG_ROTATION_ROTATE90      = 2,
  DISPLAYCONFIG_ROTATION_ROTATE180     = 3,
  DISPLAYCONFIG_ROTATION_ROTATE270     = 4,
  DISPLAYCONFIG_ROTATION_FORCE_UINT32  = 0xFFFFFFFF
} DISPLAYCONFIG_ROTATION;

typedef enum  { 
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_OTHER                 = -1,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_HD15                  = 0,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_SVIDEO                = 1,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_COMPOSITE_VIDEO       = 2,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_COMPONENT_VIDEO       = 3,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_DVI                   = 4,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_HDMI                  = 5,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_LVDS                  = 6,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_D_JPN                 = 8,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_SDI                   = 9,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_DISPLAYPORT_EXTERNAL  = 10,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_DISPLAYPORT_EMBEDDED  = 11,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_UDI_EXTERNAL          = 12,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_UDI_EMBEDDED          = 13,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_SDTVDONGLE            = 14,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_MIRACAST              = 15,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_INTERNAL              = 0x80000000,
  DISPLAYCONFIG_OUTPUT_TECHNOLOGY_FORCE_UINT32          = 0xFFFFFFFF
} DISPLAYCONFIG_VIDEO_OUTPUT_TECHNOLOGY;

typedef struct DISPLAYCONFIG_PATH_TARGET_INFO {
  LUID                                  adapterId;
  UINT32                                id;
  UINT32                                modeInfoIdx;
  DISPLAYCONFIG_VIDEO_OUTPUT_TECHNOLOGY outputTechnology;
  DISPLAYCONFIG_ROTATION                rotation;
  DISPLAYCONFIG_SCALING                 scaling;
  DISPLAYCONFIG_RATIONAL                refreshRate;
  DISPLAYCONFIG_SCANLINE_ORDERING       scanLineOrdering;
  BOOL                                  targetAvailable;
  UINT32                                statusFlags;
} DISPLAYCONFIG_PATH_TARGET_INFO;

typedef struct DISPLAYCONFIG_PATH_SOURCE_INFO {
  LUID   adapterId;
  UINT32 id;
  UINT32 modeInfoIdx;
  UINT32 statusFlags;
} DISPLAYCONFIG_PATH_SOURCE_INFO;

typedef struct DISPLAYCONFIG_PATH_INFO {
  DISPLAYCONFIG_PATH_SOURCE_INFO sourceInfo;
  DISPLAYCONFIG_PATH_TARGET_INFO targetInfo;
  UINT32                         flags;
} DISPLAYCONFIG_PATH_INFO;

typedef enum  { 
  DISPLAYCONFIG_DEVICE_INFO_GET_SOURCE_NAME            = 1,
  DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_NAME            = 2,
  DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_PREFERRED_MODE  = 3,
  DISPLAYCONFIG_DEVICE_INFO_GET_ADAPTER_NAME           = 4,
  DISPLAYCONFIG_DEVICE_INFO_SET_TARGET_PERSISTENCE     = 5,
  DISPLAYCONFIG_DEVICE_INFO_GET_TARGET_BASE_TYPE       = 6,
  DISPLAYCONFIG_DEVICE_INFO_FORCE_UINT32               = 0xFFFFFFFF
} DISPLAYCONFIG_DEVICE_INFO_TYPE;

typedef struct DISPLAYCONFIG_DEVICE_INFO_HEADER {
  DISPLAYCONFIG_DEVICE_INFO_TYPE type;
  UINT32                         size;
  LUID                           adapterId;
  UINT32                         id;
} DISPLAYCONFIG_DEVICE_INFO_HEADER;

typedef struct tagTOUCH_HIT_TESTING_INPUT {
  UINT32 pointerId;
  POINT  point;
  RECT   boundingBox;
  RECT   nonOccludedBoundingBox;
  UINT32 orientation;
} TOUCH_HIT_TESTING_INPUT, *PTOUCH_HIT_TESTING_INPUT;

typedef struct tagTOUCH_HIT_TESTING_PROXIMITY_EVALUATION {
  UINT16 score;
  POINT  adjustedPoint;
} TOUCH_HIT_TESTING_PROXIMITY_EVALUATION, *PTOUCH_HIT_TESTING_PROXIMITY_EVALUATION;

typedef struct _SCOPETABLE_ENTRY {
    int EnclosingLevel;                 /* lexical level of enclosing scope */
    int (__cdecl *Filter)(void);        /* NULL for a termination handler */
    void (__cdecl *SpecificHandler)(void); /* xcpt or termination handler */
}SCOPETABLE_ENTRY,*PSCOPETABLE_ENTRY;

typedef struct _EH3_EXCEPTION_REGISTRATION {
    struct _EH3_EXCEPTION_REGISTRATION *Next;
    PVOID ExceptionHandler;
    PSCOPETABLE_ENTRY ScopeTable;
    DWORD TryLevel;
} EH3_EXCEPTION_REGISTRATION, *PEH3_EXCEPTION_REGISTRATION;

typedef struct _CPPEH_RECORD{
	struct _EH3_EXCEPTION_REGISTRATION registration;
}CPPEH_RECORD, *PCPPEH_RECORD;

typedef struct _WINCOMPATTRDATA
{
    DWORD attribute; // the attribute to query, see below
    PVOID pData; // buffer to store the result
    ULONG dataSize; // size of the pData buffer
}WINCOMPATTRDATA, *PWINCOMPATTRDATA;

BOOL 
WINAPI 
GetCurrentInputMessageSource(
	INPUT_MESSAGE_DEVICE_TYPE *this, 
	BOOL otherParamter
);

#define POINTER_DEVICE_PRODUCT_STRING_MAX 520

typedef enum POINTER_DEVICE_TYPE { 
  POINTER_DEVICE_TYPE_INTEGRATED_PEN  = 0x00000001,
  POINTER_DEVICE_TYPE_EXTERNAL_PEN    = 0x00000002,
  POINTER_DEVICE_TYPE_TOUCH           = 0x00000003,
  POINTER_DEVICE_TYPE_TOUCH_PAD       = 0x00000004,
  POINTER_DEVICE_TYPE_MAX             = 0xFFFFFFFF
} POINTER_DEVICE_TYPE;

typedef struct tagPOINTER_DEVICE_INFO {
  DWORD               displayOrientation;
  HANDLE              device;
  POINTER_DEVICE_TYPE pointerDeviceType;
  HMONITOR            monitor;
  ULONG               startingCursorId;
  USHORT              maxActiveContacts;
  WCHAR               productString[POINTER_DEVICE_PRODUCT_STRING_MAX];
} POINTER_DEVICE_INFO;