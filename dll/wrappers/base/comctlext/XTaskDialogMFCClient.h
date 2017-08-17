/*
Module : XTaskDialogMFCClient.H
Purpose: MFC Client wrapper class for the CXTaskDialog class.
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

#ifndef __XTASKDIALOGMFCCLIENT_H__
#define __XTASKDIALOGMFCCLIENT_H__

#ifndef CXTASKDIALOGMFCCLIENT_EXT_CLASS
#define CXTASKDIALOGMFCCLIENT_EXT_CLASS
#endif //#ifndef CXTASKDIALOGMFCCLIENT_EXT_CLASS


////////////////////////////// Includes ///////////////////////////////////////

#include "XTaskDialogDefines.h"


////////////////////////////// Classes ////////////////////////////////////////

//Class which provides a MFC integrated client side encapsulation of both the native Vista Task Dialog API and our emulated CXTaskDialog implementation
class CXTASKDIALOGMFCCLIENT_EXT_CLASS CXTaskDialogMFCClient : public CWnd
{
public:
//Constructors / Destructors
  CXTaskDialogMFCClient(CWnd* pParent = NULL);

//Methods
  HRESULT DoModal();
  virtual HRESULT DoModalNative();
  virtual HRESULT DoModalEmulatedDLL();
#ifdef XTASKDIALOGMFCCLIENT_STATIC
  HRESULT DoModalEmulatedStatic();
#endif
  int   GetButtonClicked();
  int   GetRadioButtonChecked();
  BOOL  GetVerificationFlagChecked();
  void  SetParent(CWnd* pParent);
  void  SetCommonButtons(TASKDIALOG_COMMON_BUTTON_FLAGS dwCommonButtons);
  void  SetWindowTitle(UINT nID);
  void  SetWindowTitle(LPCTSTR lpstrWindowTitle);
  void  SetMainIcon(HICON hIcon);
  void  SetMainIcon(UINT nID);
  void  SetMainIcon(LPCTSTR lpstrMainIcon);
  void  SetMainInstructionText(UINT nID, UINT nIDHelp = -1);
  void  SetMainInstructionText(LPCTSTR lpstrMainInstruction, UINT nIDHelp = 0);
  void  SetHelpID(UINT nIDHelp);
  void  SetContentText(UINT nID);
  void  SetContentText(LPCTSTR lpstrContent);
  void  SetButtons(const TASKDIALOG_BUTTON* pButtons, UINT cButtons, int nDefaultButton = 0);
  void  SetDefaultButton(int nDefaultButton);
  void  SetRadioButtons(const TASKDIALOG_BUTTON* pRadioButtons, UINT cRadioButtons, int nDefaultRadioButton = 0);
  void  SetDefaultRadioButton(int nDefaultRadioButton);
  void  SetVerificationText(UINT nID);
  void  SetVerificationText(LPCTSTR lpstrVerificationText);
  void  SetExpandedInformationText(UINT nID);
  void  SetExpandedInformationText(LPCTSTR lpstrExpandedInformation);
  void  SetExpandedControlText(UINT nID);
  void  SetExpandedControlText(LPCTSTR lpstrExpandedControlText);
  void  SetCollapsedControlText(UINT nID);
  void  SetCollapsedControlText(LPCTSTR lpstrCollapsedControlText);
  void  SetFooterIcon(HICON hIcon);
  void  SetFooterIcon(UINT nID);
  void  SetFooterIcon(LPCTSTR lpstrFooterIcon);
  void  SetFooterText(UINT nID);
  void  SetFooterText(LPCTSTR lpstrFooterText);
  void  SetWidth(UINT cxWidth);
  void  ModifyFlags(DWORD dwRemove, DWORD dwAdd);
  void  NavigatePage(TASKDIALOGCONFIG& tdc);
  void  NavigatePage();
  void  ClickButton(int nButton);
  void  SetMarqueeProgressBar(BOOL bMarquee);
  BOOL  SetProgressBarState(int nNewState);
  DWORD SetProgressBarRange(int nMinRange, int nMaxRange);
  int   SetProgressBarPos(int nNewPos);
  BOOL  SetProgressBarMarquee(BOOL bMarquee, UINT uSpeed);
  void  SetElementText(TASKDIALOG_ELEMENTS element, LPCTSTR lpstrText);
  void  ClickRadioButton(int nRadioButton);
  void  EnableButton(int nButton, BOOL bEnable);
  void  EnableRadioButton(int nButton, BOOL bEnable);
  void  ClickVerification(BOOL bCheck, BOOL bFocus);
  void  UpdateElementText(TASKDIALOG_ELEMENTS element, LPCTSTR lpstrText);
  void  SetButtonElevationRequiredState(int nButton, BOOL bElevation);
  void  UpdateIcon(TASKDIALOG_ICON_ELEMENTS element, HICON hIcon);
  void  UpdateIcon(TASKDIALOG_ICON_ELEMENTS element, LPCTSTR lpstrIcon);

//Virtual methods
  virtual void OnDialogConstructed();
  virtual void OnCreated();
  virtual BOOL OnButtonClicked(int nButton);
  virtual void OnRadioButtonClicked(int nRadioButton);
  virtual void OnHyperlinkClicked(LPCTSTR pszHREF);
  virtual void OnExpandoButtonClicked(bool bExpanded);
  virtual void OnVerificationClicked(bool bChecked);
  virtual void OnHelp();
  virtual BOOL OnTimer(DWORD dwTickCount);
  virtual void OnNavigated();
  virtual void OnDestroyed();

//Member variables
  TASKDIALOGCONFIG m_tc; //The settings we will be using to show the dialog

protected:
//typedefs
  typedef HRESULT (STDAPICALLTYPE *PFN_TaskDialogIndirect)(const TASKDIALOGCONFIG* pTaskConfig, int* pnButton, int* pnRadioButton, BOOL* pfVerificationFlagChecked);

//Static methods
  static HMODULE LoadLibraryFromApplicationDirectory(LPCTSTR lpFileName);
  static HRESULT CALLBACK TaskDialogCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LONG_PTR lpRefData);

//Standard methods
  HRESULT DoModalHelper(PFN_TaskDialogIndirect pfnTaskDialogIndirect);

//Member variables
  CStringW         m_sWindowTitle;             //The window title to use
  CStringW         m_sMainInstruction;         //The main instruction text to use
  CStringW         m_sContent;                 //The content text to use
  CStringW         m_sVerificationText;        //The verification text to use
  CStringW         m_sExpandedInformation;     //The expanded information
  CStringW         m_sExpandedControlText;     //The expanded control text
  CStringW         m_sCollapsedControlText;    //The collapsed control text
  CStringW         m_sFooterText;              //The footer text
  int              m_nButton;                  //The id of the button clicked
  int              m_nRadioButton;             //The id of the radio button selected
  BOOL             m_bVerificationFlagChecked; //Was the verification check box checked
  UINT             m_nHelpIDPrompt;            //The help prompt id
};

#endif
