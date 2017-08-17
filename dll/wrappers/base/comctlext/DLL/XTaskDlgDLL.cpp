#define NOTASKDIALOG //This is required to ensure we do not pull in the Windows definition of TaskDialog(Indirect). Also note
                     //for this to work, precompiled headers must be turned off for this compilation unit

#include "stdafx.h"
#include "resource.h"
#include "..\XTaskDialog.h"
#include "..\XTaskDialogClient.h"

#undef _WDMDDK_

enum _LI_METRIC
{
     LIM_SMALL,
     LIM_LARGE
};
 
class CXTaskDialogDLLModule : public CAtlDllModuleT<CXTaskDialogDLLModule>
{
};

CXTaskDialogDLLModule _AtlModule;


//DLL Entry Point
extern "C" BOOL WINAPI DllMain(HINSTANCE /*hInstance*/, DWORD dwReason, LPVOID lpReserved)
{
  return _AtlModule.DllMain(dwReason, lpReserved); 
}

int reverseNumber(int n){
	int reverse = 0;
    
    while(n != 0) {
        int remainder = n%10;
        reverse = reverse*10 + remainder;
        n/=10;
    }
	return reverse;
}

//Our exported version of "TaskDialogIndirect"
extern "C" HRESULT __stdcall TaskDialogIndirect(TASKDIALOGCONFIG* pTaskConfig, int* pnButton, int* pnRadioButton, BOOL* pfVerificationFlagChecked)
{
  CXTaskDialog dlg;
  TASKDIALOG_BUTTON buttons[100];
  int trueIDs[100];
  UINT i;
  for(i=0;i<pTaskConfig->cButtons;i++){
	trueIDs[i] = pTaskConfig->pButtons[i].nButtonID;
	buttons[i].pszButtonText = pTaskConfig->pButtons[i].pszButtonText;
	buttons[i].nButtonID = i+1;	  
  }
  pTaskConfig->pButtons = buttons;
  return dlg.TaskDialogIndirect(pTaskConfig, pnButton, pnRadioButton, pfVerificationFlagChecked, trueIDs);
}

//Our exported version of "TaskDialog"
extern "C" HRESULT __stdcall TaskDialog(HWND hWndParent, HINSTANCE hInstance, PCWSTR pszWindowTitle, PCWSTR pszMainInstruction, PCWSTR pszContent, TASKDIALOG_COMMON_BUTTON_FLAGS dwCommonButtons, PCWSTR pszIcon, int* pnButton)
{
  TASKDIALOGCONFIG tc;
  memset(&tc, 0, sizeof(TASKDIALOGCONFIG));
  tc.cbSize = sizeof(TASKDIALOGCONFIG);
  tc.hwndParent = hWndParent;
  tc.hInstance = hInstance;
  tc.pszWindowTitle = pszWindowTitle;
  tc.pszMainInstruction = pszMainInstruction;
  tc.pszContent = pszContent;
  tc.dwCommonButtons = dwCommonButtons;
  tc.pszMainIcon = pszIcon;
  return TaskDialogIndirect(&tc, pnButton, NULL, NULL);
}

/***********************************************************************
 * LoadIconWithScaleDown [COMCTL32.@]
 */
extern "C" HRESULT __stdcall WINAPI LoadIconWithScaleDown(HINSTANCE hinst, const WCHAR *name, int cx, int cy, HICON *icon)
{
    *icon = (HICON)NULL;

    if (!name)
        return E_INVALIDARG;

    *icon = (HICON)LoadImageW(hinst, name, IMAGE_ICON, cx, cy,
                       (hinst || IS_INTRESOURCE(name)) ? 0 : LR_LOADFROMFILE);
    if (!*icon)
        return E_FAIL;

    return S_OK;
}

/***********************************************************************
 * LoadIconMetric [COMCTL32.@]
 */
extern "C" HRESULT __stdcall WINAPI LoadIconMetric(HINSTANCE hinst, const WCHAR *name, int size, HICON *icon)
{
    int cx, cy;
	
    if (size == LIM_SMALL)
    {
        cx = GetSystemMetrics(SM_CXSMICON);
        cy = GetSystemMetrics(SM_CYSMICON);
    }
    else if (size == LIM_LARGE)
    {
        cx = GetSystemMetrics(SM_CXICON);
        cy = GetSystemMetrics(SM_CYICON);
    }
    else
    {
        *icon = NULL;
        return E_INVALIDARG;
    }

    return LoadIconWithScaleDown(hinst, name, cx, cy, icon);
}

BOOL DIBHasAlpha(int cx, int cy, RGBQUAD* prgb)
{
    int cTotal = cx * cy;
    for (int i = 0; i < cTotal; i++)
    {
        if (prgb[i].rgbReserved != 0)
            return TRUE;
    }

    return FALSE;
}

void PreProcessDIB(int cx, int cy, RGBQUAD* pargb)
{
    int cTotal = cx * cy;
    for (int i = 0; i < cTotal; i++)
    {
        RGBQUAD* prgb = &pargb[i];
        if (prgb->rgbReserved != 0)
        {
            prgb->rgbRed      = ((prgb->rgbRed   * prgb->rgbReserved) + 128) / 255;
            prgb->rgbGreen    = ((prgb->rgbGreen * prgb->rgbReserved) + 128) / 255;
            prgb->rgbBlue     = ((prgb->rgbBlue  * prgb->rgbReserved) + 128) / 255;
        }
        else
        {
            *((DWORD*)prgb) = 0;
        }
    }
}

extern "C" 
HANDLE 
PremultiplyAlphaChannel(HANDLE hImage, RGBQUAD *pargb)
{
  HANDLE hNewImage; // edi@1
  LOGFONTW logFont; // [sp+4h] [bp-1Ch]@2
  BITMAP bitmap; // [sp+8h] [bp-18h]@3

  hNewImage = CopyImage(hImage, 0, 0, 0, 0x2000u);
  if ( hNewImage
    && GetObjectW(hNewImage, 24, &logFont)
    && &bitmap.bmPlanes
    && 32 == HIWORD(bitmap.bmWidthBytes)
    && DIBHasAlpha(bitmap.bmType, bitmap.bmWidth, pargb) )
    PreProcessDIB(bitmap.bmType, bitmap.bmWidth, pargb);
  return hNewImage;
}