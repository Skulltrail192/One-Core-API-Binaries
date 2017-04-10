#ifndef _PRECOMP_H__
#define _PRECOMP_H__

#include <stdarg.h>
#include <assert.h>

#define COBJMACROS
#define WIN32_NO_STATUS
#define _INC_WINDOWS
#define COM_NO_WINDOWS_H
#define NTOS_MODE_USER

#include <windef.h>
#include <winbase.h>
#include <winreg.h>
#include <winuser.h>
#include <wincon.h>
#include <commdlg.h>
#include <ddeml.h>
#include <shlguid_undoc.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <shlobj_undoc.h>
#include <shlwapi_undoc.h>
#include <ndk/rtlfuncs.h>
#include <fmifs/fmifs.h>
#include <tchar.h>
#include <strsafe.h>
#include <atlbase.h>
#include <shresdef.h>
#include <winver.h>

#include <wine/debug.h>
#include <wine/unicode.h>

extern const GUID CLSID_AdminFolderShortcut;
extern const GUID CLSID_FontsFolderShortcut;
extern const GUID CLSID_StartMenu;
extern const GUID CLSID_MenuBandSite;
extern const GUID CLSID_OpenWith;
extern const GUID CLSID_UnixFolder;
extern const GUID CLSID_UnixDosFolder;
extern const GUID SHELL32_AdvtShortcutProduct;
extern const GUID SHELL32_AdvtShortcutComponent;

HINSTANCE shell32_hInstance;

#endif /* _PRECOMP_H__ */
