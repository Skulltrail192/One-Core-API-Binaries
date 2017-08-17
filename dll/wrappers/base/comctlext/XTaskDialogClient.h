/*
Module : XTaskDialogClient.H
Purpose: Client wrapper class for the CXTaskDialog class. This client class is based on the CTaskDialog
         class as provided with WTL8
Created: PJN / 14-03-2007

Copyright (c) 2007 - 2015 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code. 

*/


/////////////////////////////// Macros / Defines //////////////////////////////

#pragma once

#ifndef __XTASKDIALOGCLIENT_H__
#define __XTASKDIALOGCLIENT_H__

#ifndef CXTASKDIALOGCLIENT_EXT_CLASS
#define CXTASKDIALOGCLIENT_EXT_CLASS
#endif //#ifndef CXTASKDIALOGCLIENT_EXT_CLASS


/////////////////////////////// Includes //////////////////////////////////////

#include "XTaskDialogDefines.h"
#ifdef XTASKDIALOGCLIENT_STATIC
#include "XTaskDialog.h"
#endif //#ifdef XTASKDIALOGCLIENT_STATIC


////////////////////////////// Classes ////////////////////////////////////////

//Class which provides a client side encapsulation of both the native Vista Task Dialog API and our emulated CXTaskDialog implementation
class CXTASKDIALOGCLIENT_EXT_CLASS CXTaskDialogClient
{
public:
//Constructors / Destructors
  CXTaskDialogClient(HWND hWndParent = NULL) : m_hWnd(NULL)
  {
    memset(&m_tc, 0, sizeof(TASKDIALOGCONFIG));
    m_tc.cbSize = sizeof(TASKDIALOGCONFIG);
    m_tc.hwndParent = hWndParent;
    m_tc.pfCallback = TaskDialogCallback;
    m_tc.lpCallbackData = reinterpret_cast<LONG_PTR>(this);
    m_tc.hInstance = _AtlBaseModule.GetResourceInstance();
  }

//Methods
  HRESULT DoModal(HWND hWndParent = ::GetActiveWindow(), int* pnButton = NULL, int* pnRadioButton = NULL, BOOL* pfVerificationFlagChecked = NULL)
  {
    //Try Vista's native implementation first
    HRESULT hr = DoModalNative(hWndParent, pnButton, pnRadioButton, pfVerificationFlagChecked);
  
    //If that failed then fall back to our emulated implementation
    if (FAILED(hr))
    {
    #ifdef XTASKDIALOGCLIENT_STATIC
      hr = DoModalEmulatedStatic(hWndParent, pnButton, pnRadioButton, pfVerificationFlagChecked);
    #else
      hr = DoModalEmulatedDLL(hWndParent, pnButton, pnRadioButton, pfVerificationFlagChecked);
    #endif
    }
  
    return hr;
  }

  HRESULT DoModalNative(HWND hWndParent = ::GetActiveWindow(), int* pnButton = NULL, int* pnRadioButton = NULL, BOOL* pfVerificationFlagChecked = NULL)
  {
    if (m_tc.hwndParent == NULL)
      m_tc.hwndParent = hWndParent;

    //Get the TaskDialogIndirect function pointer in the Common Control DLL
    typedef HRESULT (STDAPICALLTYPE *PFN_TaskDialogIndirect)(const TASKDIALOGCONFIG* pTaskConfig, int* pnButton, int* pnRadioButton, BOOL* pfVerificationFlagChecked);

    HRESULT hr = E_UNEXPECTED;
    HMODULE hCommCtrl = LoadLibrary(_T("comctl32.dll"));
    if (hCommCtrl)
    {
      PFN_TaskDialogIndirect pfnTaskDialogIndirect = reinterpret_cast<PFN_TaskDialogIndirect>(GetProcAddress(hCommCtrl, "TaskDialogIndirect"));
      if (pfnTaskDialogIndirect)
      {
        //Call the Vista native implementation of "TaskDialogIndirect"
        hr = pfnTaskDialogIndirect(&m_tc, pnButton, pnRadioButton, pfVerificationFlagChecked);
      }
      else
        hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, GetLastError());

      FreeLibrary(hCommCtrl);
    }
    else
      hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, GetLastError());    

    return hr;
  }
  
  HRESULT DoModalEmulatedDLL(HWND hWndParent = ::GetActiveWindow(), int* pnButton = NULL, int* pnRadioButton = NULL, BOOL* pfVerificationFlagChecked = NULL)
  {
    if (m_tc.hwndParent == NULL)
      m_tc.hwndParent = hWndParent;

    //Get the TaskDialogIndirect function pointer in our TaskDlg.DLL
    typedef HRESULT (STDAPICALLTYPE *PFN_TaskDialogIndirect)(const TASKDIALOGCONFIG* pTaskConfig, int* pnButton, int* pnRadioButton, BOOL* pfVerificationFlagChecked);

    HRESULT hr = E_UNEXPECTED;
    HMODULE hTaskDlg = LoadLibrary(_T("XTaskDlg.dll"));
    if (hTaskDlg)
    {
      PFN_TaskDialogIndirect pfnTaskDialogIndirect = reinterpret_cast<PFN_TaskDialogIndirect>(GetProcAddress(hTaskDlg, "TaskDialogIndirect"));
      if (pfnTaskDialogIndirect)
      {
        //Call the emulated implementation of "TaskDialogIndirect"
        hr = pfnTaskDialogIndirect(&m_tc, pnButton, pnRadioButton, pfVerificationFlagChecked);
      }
      else
        hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, GetLastError());

      ::FreeLibrary(hTaskDlg);
    }
    else
      hr = MAKE_HRESULT(SEVERITY_ERROR, FACILITY_WIN32, GetLastError());    

    return hr;
  }

#ifdef XTASKDIALOGCLIENT_STATIC
  HRESULT DoModalEmulatedStatic(HWND hWndParent = ::GetActiveWindow(), int* pnButton = NULL, int* pnRadioButton = NULL, BOOL* pfVerificationFlagChecked = NULL)
  {
    if (m_tc.hwndParent == NULL)
      m_tc.hwndParent = hWndParent;
    
    CXTaskDialog dlg;
    return dlg.TaskDialogIndirect(&m_tc, pnButton, pnRadioButton, pfVerificationFlagChecked);
  }
#endif
  
  void SetCommonButtons(TASKDIALOG_COMMON_BUTTON_FLAGS dwCommonButtons)
  {
    m_tc.dwCommonButtons = dwCommonButtons;
  }

  void SetWindowTitle(UINT nID)
  {
    m_tc.pszWindowTitle = MAKEINTRESOURCEW(nID);
  }

  void SetWindowTitle(LPCWSTR lpstrWindowTitle)
  {
    m_tc.pszWindowTitle = lpstrWindowTitle;
  }

  void SetMainIcon(HICON hIcon)
  {
    m_tc.dwFlags |= TDF_USE_HICON_MAIN;
    m_tc.hMainIcon = hIcon;
  }

  void SetMainIcon(UINT nID)
  {
    m_tc.dwFlags &= ~TDF_USE_HICON_MAIN;
    m_tc.pszMainIcon = MAKEINTRESOURCEW(nID);
  }

  void SetMainIcon(LPCWSTR lpstrMainIcon)
  {
    m_tc.dwFlags &= ~TDF_USE_HICON_MAIN;
    m_tc.pszMainIcon = lpstrMainIcon;
  }

  void SetMainInstructionText(UINT nID)
  {
    m_tc.pszMainInstruction = MAKEINTRESOURCEW(nID);
  }

  void SetMainInstructionText(LPCWSTR lpstrMainInstruction)
  {
    m_tc.pszMainInstruction = lpstrMainInstruction;
  }

  void SetContentText(UINT nID)
  {
    m_tc.pszContent = MAKEINTRESOURCEW(nID);
  }

  void SetContentText(LPCWSTR lpstrContent)
  {
    m_tc.pszContent = lpstrContent;
  }

  void SetButtons(const TASKDIALOG_BUTTON* pButtons, UINT cButtons, int nDefaultButton = 0)
  {
    m_tc.pButtons = pButtons;
    m_tc.cButtons = cButtons;
    if (nDefaultButton != 0)
      m_tc.nDefaultButton = nDefaultButton;
  }

  void SetDefaultButton(int nDefaultButton)
  {
    m_tc.nDefaultButton = nDefaultButton;
  }

  void SetRadioButtons(const TASKDIALOG_BUTTON* pRadioButtons, UINT cRadioButtons, int nDefaultRadioButton = 0)
  {
    m_tc.pRadioButtons = pRadioButtons;
    m_tc.cRadioButtons = cRadioButtons;
    if(nDefaultRadioButton != 0)
      m_tc.nDefaultRadioButton = nDefaultRadioButton;
  }

  void SetDefaultRadioButton(int nDefaultRadioButton)
  {
    m_tc.nDefaultRadioButton = nDefaultRadioButton;
  }

  void SetVerificationText(UINT nID)
  {
    m_tc.pszVerificationText = MAKEINTRESOURCEW(nID);
  }

  void SetVerificationText(LPCWSTR lpstrVerificationText)
  {
    m_tc.pszVerificationText = lpstrVerificationText;
  }

  void SetExpandedInformationText(UINT nID)
  {
    m_tc.pszExpandedInformation = MAKEINTRESOURCEW(nID);
  }

  void SetExpandedInformationText(LPCWSTR lpstrExpandedInformation)
  {
    m_tc.pszExpandedInformation = lpstrExpandedInformation;
  }

  void SetExpandedControlText(UINT nID)
  {
    m_tc.pszExpandedControlText = MAKEINTRESOURCEW(nID);
  }

  void SetExpandedControlText(LPCWSTR lpstrExpandedControlText)
  {
    m_tc.pszExpandedControlText = lpstrExpandedControlText;
  }

  void SetCollapsedControlText(UINT nID)
  {
    m_tc.pszCollapsedControlText = MAKEINTRESOURCEW(nID);
  }

  void SetCollapsedControlText(LPCWSTR lpstrCollapsedControlText)
  {
    m_tc.pszCollapsedControlText = lpstrCollapsedControlText;
  }

  void SetFooterIcon(HICON hIcon)
  {
    m_tc.dwFlags |= TDF_USE_HICON_FOOTER;
    m_tc.hFooterIcon = hIcon;
  }	

  void SetFooterIcon(UINT nID)
  {
    m_tc.dwFlags &= ~TDF_USE_HICON_FOOTER;
    m_tc.pszFooterIcon = MAKEINTRESOURCEW(nID);
  }

  void SetFooterIcon(LPCWSTR lpstrFooterIcon)
  {
    m_tc.dwFlags &= ~TDF_USE_HICON_FOOTER;
    m_tc.pszFooterIcon = lpstrFooterIcon;
  }

  void SetFooterText(UINT nID)
  {
    m_tc.pszFooter = MAKEINTRESOURCEW(nID);
  }

  void SetFooterText(LPCWSTR lpstrFooterText)
  {
    m_tc.pszFooter = lpstrFooterText;
  }

  void SetWidth(UINT cxWidth)
  {
    m_tc.cxWidth = cxWidth;
  }

  void ModifyFlags(DWORD dwRemove, DWORD dwAdd)
  {
    m_tc.dwFlags = (m_tc.dwFlags & ~dwRemove) | dwAdd;
  }

  void NavigatePage(TASKDIALOGCONFIG& tdc)
  {
    ATLASSUME(m_hWnd != NULL);

    m_tc.cbSize = sizeof(TASKDIALOGCONFIG);
    if (m_tc.hwndParent == NULL)
      tdc.hwndParent = m_tc.hwndParent;
    tdc.pfCallback = m_tc.pfCallback;
    tdc.lpCallbackData = m_tc.lpCallbackData;
    m_tc = tdc;

    SendMessage(m_hWnd, TDM_NAVIGATE_PAGE, 0, reinterpret_cast<LPARAM>(&tdc));
  }

  void NavigatePage()
  {
    ATLASSUME(m_hWnd != NULL);
    SendMessage(m_hWnd, TDM_NAVIGATE_PAGE, 0, reinterpret_cast<LPARAM>(this));
  }

  void ClickButton(int nButton)
  {
    ATLASSUME(m_hWnd != NULL);
    SendMessage(m_hWnd, TDM_CLICK_BUTTON, nButton, 0L);
  }

  void SetMarqueeProgressBar(BOOL bMarquee)
  {
    ATLASSUME(m_hWnd != NULL);
    SendMessage(m_hWnd, TDM_SET_MARQUEE_PROGRESS_BAR, bMarquee, 0L);
  }

  BOOL SetProgressBarState(int nNewState)
  {
    ATLASSUME(m_hWnd != NULL);
    return static_cast<BOOL>(SendMessage(m_hWnd, TDM_SET_PROGRESS_BAR_STATE, nNewState, 0L));
  }

  DWORD SetProgressBarRange(int nMinRange, int nMaxRange)
  {
    ATLASSUME(m_hWnd != NULL);
    return static_cast<DWORD>(SendMessage(m_hWnd, TDM_SET_PROGRESS_BAR_RANGE, 0, MAKELPARAM(nMinRange, nMaxRange)));
  }

  int SetProgressBarPos(int nNewPos)
  {
    ATLASSUME(m_hWnd != NULL);
    return static_cast<int>(SendMessage(m_hWnd, TDM_SET_PROGRESS_BAR_POS, nNewPos, 0L));
  }

  BOOL SetProgressBarMarquee(BOOL bMarquee, UINT uSpeed)
  {
    ATLASSUME(m_hWnd != NULL);
    return static_cast<BOOL>(SendMessage(m_hWnd, TDM_SET_PROGRESS_BAR_MARQUEE, bMarquee, uSpeed));
  }

  void SetElementText(TASKDIALOG_ELEMENTS element, LPCWSTR lpstrText)
  {
    ATLASSUME(m_hWnd != NULL);
    SendMessage(m_hWnd, TDM_SET_ELEMENT_TEXT, element, reinterpret_cast<LPARAM>(lpstrText));
  }

  void ClickRadioButton(int nRadioButton)
  {
    ATLASSUME(m_hWnd != NULL);
    SendMessage(m_hWnd, TDM_CLICK_RADIO_BUTTON, nRadioButton, 0L);
  }

  void EnableButton(int nButton, BOOL bEnable)
  {
    ATLASSUME(m_hWnd != NULL);
    SendMessage(m_hWnd, TDM_ENABLE_BUTTON, nButton, bEnable);
  }

  void EnableRadioButton(int nButton, BOOL bEnable)
  {
    ATLASSUME(m_hWnd != NULL);
    SendMessage(m_hWnd, TDM_ENABLE_RADIO_BUTTON, nButton, bEnable);
  }

  void ClickVerification(BOOL bCheck, BOOL bFocus)
  {
    ATLASSUME(m_hWnd != NULL);
    SendMessage(m_hWnd, TDM_CLICK_VERIFICATION, bCheck, bFocus);
  }

  void UpdateElementText(TASKDIALOG_ELEMENTS element, LPCWSTR lpstrText)
  {
    ATLASSUME(m_hWnd != NULL);
    SendMessage(m_hWnd, TDM_UPDATE_ELEMENT_TEXT, element, reinterpret_cast<LPARAM>(lpstrText));
  }

  void SetButtonElevationRequiredState(int nButton, BOOL bElevation)
  {
    ATLASSUME(m_hWnd != NULL);
    SendMessage(m_hWnd, TDM_SET_BUTTON_ELEVATION_REQUIRED_STATE, nButton, bElevation);
  }

  void UpdateIcon(TASKDIALOG_ICON_ELEMENTS element, HICON hIcon)
  {
    ATLASSUME(m_hWnd != NULL);
#ifdef _DEBUG
    if (element == TDIE_ICON_MAIN)
      ATLASSERT((m_tc.dwFlags & TDF_USE_HICON_MAIN) != 0);
    else if (element == TDIE_ICON_FOOTER)
      ATLASSERT((m_tc.dwFlags & TDF_USE_HICON_FOOTER) != 0);
#endif // _DEBUG
    SendMessage(m_hWnd, TDM_UPDATE_ICON, element, reinterpret_cast<LPARAM>(hIcon));
  }

  void UpdateIcon(TASKDIALOG_ICON_ELEMENTS element, LPCWSTR lpstrIcon)
  {
    ATLASSUME(m_hWnd != NULL);
#ifdef _DEBUG
    if(element == TDIE_ICON_MAIN)
      ATLASSERT((m_tc.dwFlags & TDF_USE_HICON_MAIN) == 0);
    else if(element == TDIE_ICON_FOOTER)
      ATLASSERT((m_tc.dwFlags & TDF_USE_HICON_FOOTER) == 0);
#endif // _DEBUG
    SendMessage(m_hWnd, TDM_UPDATE_ICON, element, reinterpret_cast<LPARAM>(lpstrIcon));
  }

  static HRESULT CALLBACK TaskDialogCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LONG_PTR lpRefData)
  {
    CXTaskDialogClient* pThis = reinterpret_cast<CXTaskDialogClient*>(lpRefData);
    ATLASSERT(pThis->m_hWnd == NULL || pThis->m_hWnd == hWnd);

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
        pThis->OnHyperlinkClicked(reinterpret_cast<LPCWSTR>(lParam));
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
        ATLTRACE(_T("Unknown notification received in CXTaskDialog::TaskDialogCallback\n"));
        break;
      }
    }

    return hr;
  }

//Virtual methods
  virtual void OnDialogConstructed()
  {
  }

  virtual void OnCreated()
  {
  }

  virtual BOOL OnButtonClicked(int /*nButton*/)
  {
    return FALSE;   //don't prevent dialog to close
  }

  virtual void OnRadioButtonClicked(int /*nRadioButton*/)
  {
  }

  virtual void OnHyperlinkClicked(LPCWSTR /*pszHREF*/)
  {
  }

  virtual void OnExpandoButtonClicked(bool /*bExpanded*/)
  {
  }

  virtual void OnVerificationClicked(bool /*bChecked*/)
  {
  }

  virtual void OnHelp()
  {
  }

  virtual BOOL OnTimer(DWORD /*dwTickCount*/)
  {
    return FALSE; //don't reset counter
  }

  virtual void OnNavigated()
  {
  }

  virtual void OnDestroyed()
  {
  }

protected:
//Member variables
  TASKDIALOGCONFIG m_tc;   //The settings we will be using to show the dialog
  HWND             m_hWnd; //The Task Dialog window
};

#endif //#ifndef __XTASKDIALOGCLIENT_H__
