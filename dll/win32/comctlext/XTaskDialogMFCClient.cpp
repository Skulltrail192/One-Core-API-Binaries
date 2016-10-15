/*
Module : XTaskDialogMFCClient.cpp
Purpose: MFC Client wrapper class for the CXTaskDialog class.
Created: PJN / 14-03-2007
History: PJN / 27-09-2015 1. Updated copyright details.

Copyright (c) 2007 - 2015 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


/////////////////////////////// Includes //////////////////////////////////////

#include "stdafx.h"
#include "XTaskDialogMFCClient.h"
#ifdef XTASKDIALOGMFCCLIENT_STATIC
#include "XTaskDialog.h"
#endif //#ifdef XTASKDIALOGMFCCLIENT_STATIC


/////////////////////////////// Macros / Defines //////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif //#ifdef _DEBUG

//Simple class to allow access to CWinApp::m_dwPromptContext
class CXTaskDialogMFCClientPublicWinApp : public CWinApp
{
public:
  DWORD* GetPromptContextAddress() 
  { 
    return &m_dwPromptContext; 
  }
};

#ifndef WM_HELPPROMPTADDR
#define WM_HELPPROMPTADDR	0x0376
#endif //#ifndef WM_HELPPROMPTADDR

#ifndef HID_BASE_PROMPT
#define HID_BASE_PROMPT 0x00030000UL
#endif //#ifndef HID_BASE_PROMPT


////////////////////////////// Implementation /////////////////////////////////

CXTaskDialogMFCClient::CXTaskDialogMFCClient(CWnd* pParent) : m_nButton(0),
                                                              m_nRadioButton(0),
                                                              m_bVerificationFlagChecked(FALSE),
                                                              m_nHelpIDPrompt(0)
{
  memset(&m_tc, 0, sizeof(TASKDIALOGCONFIG));
  m_tc.cbSize = sizeof(TASKDIALOGCONFIG);
  m_tc.pfCallback = TaskDialogCallback;
  m_tc.lpCallbackData = reinterpret_cast<LONG_PTR>(this);
  m_tc.hInstance = AfxGetResourceHandle();
  SetParent(pParent);
}
  
HRESULT CXTaskDialogMFCClient::DoModal()
{
  //Try Vista's native implementation first
  HRESULT hr = DoModalNative();

  //If that failed then fall back to our emulated implementation
  if (FAILED(hr))
  {
  #ifdef XTASKDIALOGMFCCLIENT_STATIC
    hr = DoModalEmulatedStatic();
  #else
    hr = DoModalEmulatedDLL();
  #endif //#ifdef XTASKDIALOGMFCCLIENT_STATIC
  }

  return hr;
}

HMODULE CXTaskDialogMFCClient::LoadLibraryFromApplicationDirectory(LPCTSTR lpFileName)
{
  //Get the Application diretory
  TCHAR szFullPath[_MAX_PATH];
  szFullPath[0] = _T('\0');
  if (GetModuleFileName(NULL, szFullPath, _countof(szFullPath)) == 0)
    return NULL;

  //Form the new path
  TCHAR szDrive[_MAX_DRIVE];
  szDrive[0] = _T('\0');
  TCHAR szDir[_MAX_DIR];
  szDir[0] = _T('\0');
  _tsplitpath_s(szFullPath, szDrive, sizeof(szDrive)/sizeof(TCHAR), szDir, sizeof(szDir)/sizeof(TCHAR), NULL, 0, NULL, 0);
   TCHAR szFname[_MAX_FNAME];
   szFname[0] = _T('\0');
   TCHAR szExt[_MAX_EXT];
   szExt[0] = _T('\0');
  _tsplitpath_s(lpFileName, NULL, 0, NULL, 0, szFname, sizeof(szFname)/sizeof(TCHAR), szExt, sizeof(szExt)/sizeof(TCHAR));
  _tmakepath_s(szFullPath, sizeof(szFullPath)/sizeof(TCHAR), szDrive, szDir, szFname, szExt);

  //Delegate to LoadLibrary    
  return LoadLibrary(szFullPath);
}

HRESULT CXTaskDialogMFCClient::DoModalNative()
{
  //Get the TaskDialogIndirect function pointer in the Common Control DLL
  HRESULT hr = E_UNEXPECTED;
  HMODULE hCommCtrl = LoadLibrary(_T("comctl32.dll")); //Note using LoadLibrary here without a fully qualified path is ok because we should be using a manifest to specify a dependency on Common Ctrl v6.
  if (hCommCtrl)
  {
    PFN_TaskDialogIndirect pfnTaskDialogIndirect = reinterpret_cast<PFN_TaskDialogIndirect>(GetProcAddress(hCommCtrl, "TaskDialogIndirect"));
    if (pfnTaskDialogIndirect)
      hr = DoModalHelper(pfnTaskDialogIndirect);
    else
      hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, GetLastError());

    FreeLibrary(hCommCtrl);
  }
  else
    hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, GetLastError());    

  return hr;
}

HRESULT CXTaskDialogMFCClient::DoModalEmulatedDLL()
{
  //Get the TaskDialogIndirect function pointer in our dll
  HRESULT hr = E_UNEXPECTED;
  HMODULE hTaskDlg = LoadLibraryFromApplicationDirectory(_T("XTaskDlg.dll"));
  if (hTaskDlg)
  {
    PFN_TaskDialogIndirect pfnTaskDialogIndirect = reinterpret_cast<PFN_TaskDialogIndirect>(GetProcAddress(hTaskDlg, "TaskDialogIndirect"));
    if (pfnTaskDialogIndirect)
      hr = DoModalHelper(pfnTaskDialogIndirect);
    else
      hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, GetLastError());

    FreeLibrary(hTaskDlg);
  }
  else
    hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, ::GetLastError());    

  return hr;
}

#ifdef XTASKDIALOGMFCCLIENT_STATIC
HRESULT CXTaskDialogMFCClient::DoModalEmulatedStatic()
{
  return DoModalHelper(NULL);
}
#endif //#ifdef XTASKDIALOGMFCCLIENT_STATIC

int CXTaskDialogMFCClient::GetButtonClicked()
{
  return m_nButton;
}

int CXTaskDialogMFCClient::GetRadioButtonChecked()
{
  return m_nRadioButton;
}

BOOL CXTaskDialogMFCClient::GetVerificationFlagChecked()
{
  return m_bVerificationFlagChecked;
}

void CXTaskDialogMFCClient::SetParent(CWnd* pParent)
{
  if (pParent != NULL)
    m_tc.hwndParent = pParent->GetSafeHwnd();
  else
    m_tc.hwndParent = NULL;
}

void CXTaskDialogMFCClient::SetCommonButtons(TASKDIALOG_COMMON_BUTTON_FLAGS dwCommonButtons)
{
  m_tc.dwCommonButtons = dwCommonButtons;
}

void CXTaskDialogMFCClient::SetWindowTitle(UINT nID)
{
  m_tc.pszWindowTitle = MAKEINTRESOURCEW(nID);
}

void CXTaskDialogMFCClient::SetWindowTitle(LPCTSTR lpstrWindowTitle)
{
  m_sWindowTitle = lpstrWindowTitle;
  m_tc.pszWindowTitle = m_sWindowTitle;
}

void CXTaskDialogMFCClient::SetMainIcon(HICON hIcon)
{
  m_tc.dwFlags |= TDF_USE_HICON_MAIN;
  m_tc.hMainIcon = hIcon;
}

void CXTaskDialogMFCClient::SetMainIcon(UINT nID)
{
  m_tc.dwFlags &= ~TDF_USE_HICON_MAIN;
  m_tc.pszMainIcon = MAKEINTRESOURCEW(nID);
}

void CXTaskDialogMFCClient::SetMainIcon(LPCTSTR lpstrMainIcon)
{
  m_tc.dwFlags &= ~TDF_USE_HICON_MAIN;
  m_tc.pszMainIcon = MAKEINTRESOURCEW(reinterpret_cast<ULONG_PTR>(lpstrMainIcon));
}

void CXTaskDialogMFCClient::SetMainInstructionText(UINT nID, UINT nIDHelp)
{
  m_tc.pszMainInstruction = MAKEINTRESOURCEW(nID);
  
  if (nIDHelp == static_cast<UINT>(-1))
    m_nHelpIDPrompt = nID;
  else
    m_nHelpIDPrompt = nIDHelp;
}

void CXTaskDialogMFCClient::SetMainInstructionText(LPCTSTR lpstrMainInstruction, UINT nIDHelp)
{
  m_sMainInstruction = lpstrMainInstruction;
  m_tc.pszMainInstruction = m_sMainInstruction;
  m_nHelpIDPrompt = nIDHelp;
}

void CXTaskDialogMFCClient::SetHelpID(UINT nIDHelp)
{
  m_nHelpIDPrompt = nIDHelp;
}

void CXTaskDialogMFCClient::SetContentText(UINT nID)
{
  m_tc.pszContent = MAKEINTRESOURCEW(nID);
}

void CXTaskDialogMFCClient::SetContentText(LPCTSTR lpstrContent)
{
  m_sContent = lpstrContent;
  m_tc.pszContent = m_sContent;
}

void CXTaskDialogMFCClient::SetButtons(const TASKDIALOG_BUTTON* pButtons, UINT cButtons, int nDefaultButton)
{
  m_tc.pButtons = pButtons;
  m_tc.cButtons = cButtons;
  if (nDefaultButton != 0)
    m_tc.nDefaultButton = nDefaultButton;
}

void CXTaskDialogMFCClient::SetDefaultButton(int nDefaultButton)
{
  m_tc.nDefaultButton = nDefaultButton;
}

void CXTaskDialogMFCClient::SetRadioButtons(const TASKDIALOG_BUTTON* pRadioButtons, UINT cRadioButtons, int nDefaultRadioButton)
{
  m_tc.pRadioButtons = pRadioButtons;
  m_tc.cRadioButtons = cRadioButtons;
  if(nDefaultRadioButton != 0)
    m_tc.nDefaultRadioButton = nDefaultRadioButton;
}

void CXTaskDialogMFCClient::SetDefaultRadioButton(int nDefaultRadioButton)
{
  m_tc.nDefaultRadioButton = nDefaultRadioButton;
}

void CXTaskDialogMFCClient::SetVerificationText(UINT nID)
{
  m_tc.pszVerificationText = MAKEINTRESOURCEW(nID);
}

void CXTaskDialogMFCClient::SetVerificationText(LPCTSTR lpstrVerificationText)
{
  m_sVerificationText = lpstrVerificationText;
  m_tc.pszVerificationText = m_sVerificationText;
}

void CXTaskDialogMFCClient::SetExpandedInformationText(UINT nID)
{
  m_tc.pszExpandedInformation = MAKEINTRESOURCEW(nID);
}

void CXTaskDialogMFCClient::SetExpandedInformationText(LPCTSTR lpstrExpandedInformation)
{
  m_sExpandedInformation = lpstrExpandedInformation;
  m_tc.pszExpandedInformation = m_sExpandedInformation;
}

void CXTaskDialogMFCClient::SetExpandedControlText(UINT nID)
{
  m_tc.pszExpandedControlText = MAKEINTRESOURCEW(nID);
}

void CXTaskDialogMFCClient::SetExpandedControlText(LPCTSTR lpstrExpandedControlText)
{
  m_sExpandedControlText = lpstrExpandedControlText;
  m_tc.pszExpandedControlText = m_sExpandedControlText;
}

void CXTaskDialogMFCClient::SetCollapsedControlText(UINT nID)
{
  m_tc.pszCollapsedControlText = MAKEINTRESOURCEW(nID);
}

void CXTaskDialogMFCClient::SetCollapsedControlText(LPCTSTR lpstrCollapsedControlText)
{
  m_sCollapsedControlText = lpstrCollapsedControlText;
  m_tc.pszCollapsedControlText = m_sCollapsedControlText;
}

void CXTaskDialogMFCClient::SetFooterIcon(HICON hIcon)
{
  m_tc.dwFlags |= TDF_USE_HICON_FOOTER;
  m_tc.hFooterIcon = hIcon;
}	

void CXTaskDialogMFCClient::SetFooterIcon(UINT nID)
{
  m_tc.dwFlags &= ~TDF_USE_HICON_FOOTER;
  m_tc.pszFooterIcon = MAKEINTRESOURCEW(nID);
}

void CXTaskDialogMFCClient::SetFooterIcon(LPCTSTR lpstrFooterIcon)
{
  m_tc.dwFlags &= ~TDF_USE_HICON_FOOTER;
  m_tc.pszFooterIcon = MAKEINTRESOURCEW(reinterpret_cast<ULONG_PTR>(lpstrFooterIcon));
}

void CXTaskDialogMFCClient::SetFooterText(UINT nID)
{
  m_tc.pszFooter = MAKEINTRESOURCEW(nID);
}

void CXTaskDialogMFCClient::SetFooterText(LPCTSTR lpstrFooterText)
{
  m_sFooterText = lpstrFooterText;
  m_tc.pszFooter = m_sFooterText;
}

void CXTaskDialogMFCClient::SetWidth(UINT cxWidth)
{
  m_tc.cxWidth = cxWidth;
}

void CXTaskDialogMFCClient::ModifyFlags(DWORD dwRemove, DWORD dwAdd)
{
  m_tc.dwFlags = (m_tc.dwFlags & ~dwRemove) | dwAdd;
}

void CXTaskDialogMFCClient::NavigatePage(TASKDIALOGCONFIG& tdc)
{
  m_tc.cbSize = sizeof(TASKDIALOGCONFIG);
  if (m_tc.hwndParent == NULL)
    tdc.hwndParent = m_tc.hwndParent;
  tdc.pfCallback = m_tc.pfCallback;
  tdc.lpCallbackData = m_tc.lpCallbackData;
  m_tc = tdc;

  SendMessage(TDM_NAVIGATE_PAGE, 0, reinterpret_cast<LPARAM>(&tdc));
}

void CXTaskDialogMFCClient::NavigatePage()
{
  SendMessage(TDM_NAVIGATE_PAGE, 0, reinterpret_cast<LPARAM>(this));
}

void CXTaskDialogMFCClient::ClickButton(int nButton)
{
  SendMessage(TDM_CLICK_BUTTON, nButton, 0L);
}

void CXTaskDialogMFCClient::SetMarqueeProgressBar(BOOL bMarquee)
{
  SendMessage(TDM_SET_MARQUEE_PROGRESS_BAR, bMarquee, 0L);
}

BOOL CXTaskDialogMFCClient::SetProgressBarState(int nNewState)
{
  return static_cast<BOOL>(SendMessage(TDM_SET_PROGRESS_BAR_STATE, nNewState, 0L));
}

DWORD CXTaskDialogMFCClient::SetProgressBarRange(int nMinRange, int nMaxRange)
{
  return static_cast<DWORD>(SendMessage(TDM_SET_PROGRESS_BAR_RANGE, 0, MAKELPARAM(nMinRange, nMaxRange)));
}

int CXTaskDialogMFCClient::SetProgressBarPos(int nNewPos)
{
  return static_cast<int>(SendMessage(TDM_SET_PROGRESS_BAR_POS, nNewPos, 0L));
}

BOOL CXTaskDialogMFCClient::SetProgressBarMarquee(BOOL bMarquee, UINT uSpeed)
{
  return static_cast<BOOL>(SendMessage(TDM_SET_PROGRESS_BAR_MARQUEE, bMarquee, uSpeed));
}

void CXTaskDialogMFCClient::SetElementText(TASKDIALOG_ELEMENTS element, LPCTSTR lpstrText)
{
  CStringW sText(lpstrText);
  SendMessage(TDM_SET_ELEMENT_TEXT, element, reinterpret_cast<LPARAM>(sText.operator LPCWSTR()));
}

void CXTaskDialogMFCClient::ClickRadioButton(int nRadioButton)
{
  SendMessage(TDM_CLICK_RADIO_BUTTON, nRadioButton, 0L);
}

void CXTaskDialogMFCClient::EnableButton(int nButton, BOOL bEnable)
{
  SendMessage(TDM_ENABLE_BUTTON, nButton, bEnable);
}

void CXTaskDialogMFCClient::EnableRadioButton(int nButton, BOOL bEnable)
{
  SendMessage(TDM_ENABLE_RADIO_BUTTON, nButton, bEnable);
}

void CXTaskDialogMFCClient::ClickVerification(BOOL bCheck, BOOL bFocus)
{
  SendMessage(TDM_CLICK_VERIFICATION, bCheck, bFocus);
}

void CXTaskDialogMFCClient::UpdateElementText(TASKDIALOG_ELEMENTS element, LPCTSTR lpstrText)
{
  CStringW sText(lpstrText);
  SendMessage(TDM_UPDATE_ELEMENT_TEXT, element, reinterpret_cast<LPARAM>(sText.operator LPCWSTR()));
}

void CXTaskDialogMFCClient::SetButtonElevationRequiredState(int nButton, BOOL bElevation)
{
  SendMessage(TDM_SET_BUTTON_ELEVATION_REQUIRED_STATE, nButton, bElevation);
}

void CXTaskDialogMFCClient::UpdateIcon(TASKDIALOG_ICON_ELEMENTS element, HICON hIcon)
{
#ifdef _DEBUG
  if (element == TDIE_ICON_MAIN)
    ASSERT((m_tc.dwFlags & TDF_USE_HICON_MAIN) != 0);
  else if (element == TDIE_ICON_FOOTER)
    ASSERT((m_tc.dwFlags & TDF_USE_HICON_FOOTER) != 0);
#endif //#ifdef _DEBUG
  SendMessage(TDM_UPDATE_ICON, element, reinterpret_cast<LPARAM>(hIcon));
}

void CXTaskDialogMFCClient::UpdateIcon(TASKDIALOG_ICON_ELEMENTS element, LPCTSTR lpstrIcon)
{
#ifdef _DEBUG
  if(element == TDIE_ICON_MAIN)
    ASSERT((m_tc.dwFlags & TDF_USE_HICON_MAIN) == 0);
  else if(element == TDIE_ICON_FOOTER)
    ASSERT((m_tc.dwFlags & TDF_USE_HICON_FOOTER) == 0);
#endif //#ifdef _DEBUG
  SendMessage(TDM_UPDATE_ICON, element, reinterpret_cast<LPARAM>(lpstrIcon));
}

HRESULT CALLBACK CXTaskDialogMFCClient::TaskDialogCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LONG_PTR lpRefData)
{
  CXTaskDialogMFCClient* pThis = reinterpret_cast<CXTaskDialogMFCClient*>(lpRefData);
  ASSERT(pThis->m_hWnd == NULL || pThis->m_hWnd == hWnd);

  //What will be the return value from this function
  HRESULT hr = S_OK;

  switch (uMsg)
  {
    case TDN_DIALOG_CONSTRUCTED:
    {
      pThis->m_hWnd = hWnd;
      pThis->OnDialogConstructed();
      break;
    }
    case TDN_CREATED:
    {
      pThis->OnCreated();
      break;
    }
    case TDN_BUTTON_CLICKED:
    {
      hr = pThis->OnButtonClicked(static_cast<int>(wParam)) ? S_FALSE : S_OK;
      break;
    }
    case TDN_RADIO_BUTTON_CLICKED:
    {
      pThis->OnRadioButtonClicked(static_cast<int>(wParam));
      break;
    }
    case TDN_HYPERLINK_CLICKED:
    {
      CString sHREF(reinterpret_cast<LPCWSTR>(lParam));
      pThis->OnHyperlinkClicked(sHREF);
      break;
    }
    case TDN_EXPANDO_BUTTON_CLICKED:
    {
      pThis->OnExpandoButtonClicked(wParam != 0);
      break;
    }
    case TDN_VERIFICATION_CLICKED:
    {
      pThis->OnVerificationClicked(wParam != 0);
      break;
    }
    case TDN_HELP:
    {
      pThis->OnHelp();
      break;
    }
    case TDN_TIMER:
    {
      hr = pThis->OnTimer(static_cast<DWORD>(wParam)) ? S_FALSE : S_OK;
      break;
    }
    case TDN_NAVIGATED:
    {
      pThis->OnNavigated();
      break;
    }
    case TDN_DESTROYED:
    {
      pThis->OnDestroyed();
      pThis->m_hWnd = NULL;
      break;
    }
    default:
    {
      TRACE(_T("Unknown notification received in CXTaskDialog::TaskDialogCallback\n"));
      break;
    }
  }

  return hr;
}

void CXTaskDialogMFCClient::OnDialogConstructed()
{
}

void CXTaskDialogMFCClient::OnCreated()
{
}

BOOL CXTaskDialogMFCClient::OnButtonClicked(int /*nButton*/)
{
  return FALSE; //don't prevent dialog to close
}

void CXTaskDialogMFCClient::OnRadioButtonClicked(int /*nRadioButton*/)
{
}

void CXTaskDialogMFCClient::OnHyperlinkClicked(LPCTSTR /*pszHREF*/)
{
}

void CXTaskDialogMFCClient::OnExpandoButtonClicked(bool /*bExpanded*/)
{
}

void CXTaskDialogMFCClient::OnVerificationClicked(bool /*bChecked*/)
{
}

void CXTaskDialogMFCClient::OnHelp()
{
  //By default send the owner a WM_HELP message
  HELPINFO hi;
  memset(&hi, 0, sizeof(hi));
  hi.cbSize = sizeof(hi);
  hi.iContextType = HELPINFO_WINDOW;
  hi.hItemHandle = GetSafeHwnd();
  GetCursorPos(&hi.MousePos);
  ::SendMessage(m_tc.hwndParent, WM_HELP, 0, reinterpret_cast<LPARAM>(&hi));
}

BOOL CXTaskDialogMFCClient::OnTimer(DWORD /*dwTickCount*/)
{
  return FALSE; //don't reset counter
}

void CXTaskDialogMFCClient::OnNavigated()
{
}

void CXTaskDialogMFCClient::OnDestroyed()
{
}

HRESULT CXTaskDialogMFCClient::DoModalHelper(PFN_TaskDialogIndirect pfnTaskDialogIndirect)
{
  //What will be the return value from this function (assume the worst)
  HRESULT hr = E_UNEXPECTED;
  
  CXTaskDialogMFCClientPublicWinApp* pApp = static_cast<CXTaskDialogMFCClientPublicWinApp*>(AfxGetApp());
  AFXASSUME(pApp != NULL);
  
  //disable windows for modal dialog
  pApp->EnableModeless(FALSE);
  HWND hWndTop;
  HWND hWnd = CWnd::GetSafeOwner_(m_tc.hwndParent, &hWndTop);

  //re-enable the parent window, so that focus is restored 
  //correctly when the dialog is dismissed.
  if (hWnd != hWndTop)
    ::EnableWindow(hWnd, TRUE);

  //set help context if possible
  DWORD* pdwContext = NULL;

  DWORD dwWndPid = 0;
  GetWindowThreadProcessId(hWnd, &dwWndPid);

  if (hWnd != NULL && dwWndPid == GetCurrentProcessId())
  {
    //use app-level context or frame level context
    LRESULT lResult = ::SendMessage(hWnd, WM_HELPPROMPTADDR, 0, 0);
    if (lResult != 0)
      pdwContext = reinterpret_cast<DWORD*>(lResult);
  }
  //for backward compatibility use app context if possible
  if (pdwContext == NULL)
    pdwContext = pApp->GetPromptContextAddress();

  DWORD dwOldPromptContext = 0;
  if (pdwContext != NULL)
  {
    //save old prompt context for restoration later
    dwOldPromptContext = *pdwContext;
    if (m_nHelpIDPrompt != 0)
      *pdwContext = HID_BASE_PROMPT + m_nHelpIDPrompt;
  }

  if (hWnd)
    m_tc.hwndParent = hWnd;
  else
    m_tc.hwndParent = NULL;

#ifdef XTASKDIALOGMFCCLIENT_STATIC
  if (pfnTaskDialogIndirect == NULL)
  {
    CXTaskDialog dlg;
    hr = dlg.TaskDialogIndirect(&m_tc, &m_nButton, &m_nRadioButton, &m_bVerificationFlagChecked);
  }
  else
#endif //#ifdef XTASKDIALOGMFCCLIENT_STATIC
  {
    ASSERT(pfnTaskDialogIndirect != NULL);
    hr = pfnTaskDialogIndirect(&m_tc, &m_nButton, &m_nRadioButton, &m_bVerificationFlagChecked);
  }

  //restore prompt context if possible
  if (pdwContext != NULL)
    *pdwContext = dwOldPromptContext;

  //re-enable windows
  if (hWndTop != NULL)
    ::EnableWindow(hWndTop, TRUE);
  pApp->EnableModeless(TRUE);

  return hr;
}
