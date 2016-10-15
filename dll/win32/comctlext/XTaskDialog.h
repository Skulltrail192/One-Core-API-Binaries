/*
Module : XTaskDialog.H
Purpose: Defines the interface for a set of classes which implements an emulation of the Vista Task Dialog API. 
         The code is designed to support an emulated version of Task Dialogs on Windows 98, ME, Windows 2000, Windows XP, 
         Windows 2003 or later.
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

#ifndef __XTASKDIALOG_H__
#define __XTASKDIALOG_H__

#ifndef CXTASKDIALOG_EXT_CLASS
#define CXTASKDIALOG_EXT_CLASS
#endif //#ifndef CXTASKDIALOG_EXT_CLASS


/////////////////////////////// Includes //////////////////////////////////////

#ifndef __ATLWIN_H__
#pragma message("CXTaskDialog requires ATL support to implement its functionality. If your project is MFC only, then you need to update it to include ATL support")
#include <atlwin.h>
#endif //#ifndef __ATLWIN_H__
#ifndef __ATLSTR_H__
#pragma message("To avoid this message, please put atlstr.h in your pre compiled header (normally stdafx.h)")
#include <atlstr.h>
#endif //#ifndef __ATLSTR_H__
#ifndef __ATLTYPES_H__
#pragma message("To avoid this message, please put atltypes.h in your pre compiled header (normally stdafx.h)")
#include <atltypes.h>
#endif //#ifndef __ATLTYPES_H__
#include "XTaskDialogDefines.h"
#include "XTaskDialogItem.h"
#include "XTaskDialogStatic.h"
#include "XTaskDialogButton.h"
#include "XTaskDialogIconStatic.h"
#include "XTaskDialogCommandLink.h"

const UINT WM_XTASKDIALOG_HANDLE_NAVIGATE_PAGE = RegisterWindowMessage(_T("3FE02142-FD2F-4216-86CA-21C995083CBF"));


////////////////////////////// Classes ////////////////////////////////////////

//Simple class used by CXTaskDialog for handling the built in buttons which a Task Dialog can have
class CXTaskDialogButtonDetails
{
public:
  CString m_sCaption;            //The caption for the button
  int     m_nTaskDialogConfigID; //The id of the button as it is defined in the dwCommonButtons field in TASKDIALOGCONFIG
  int     m_nDialogID;           //The id of the button as it is on the dialog
};

//Class which emulates the Vista TaskDialogIndirect API
class CXTASKDIALOG_EXT_CLASS CXTaskDialog : ATL::CDialogImpl<CXTaskDialog>
{
public:
//Constructors / Destructors
  CXTaskDialog();
  virtual ~CXTaskDialog();

//Methods
  //Our emulated version of Vista Common Controls TaskDialogIndirect function	
  HRESULT TaskDialogIndirect(const TASKDIALOGCONFIG* pTaskConfig, int* pnButton, int* pnRadioButton, BOOL* pfVerificationFlagChecked, int *array);
  
//Accessors / Mutators
  void SetDoExpandoAnimation(BOOL bExpandoAnimation) { m_bDoExpandoAnimation = bExpandoAnimation; };
  BOOL GetDoExpandoAnimation() const { return m_bDoExpandoAnimation; };

protected:
//Enums
  enum
  {
    MainIconVerticalOffset         = 14,   //The vertical offset of the main icon from the top of the client area
    ButtonSpacing                  = 8,    //The horizontal spacing between buttons
    RadioButtonVerticalSpacingSize = 5,    //The verication spacing between radio buttons
    HorizontalSpacingOnButton      = 16,   //The horizontal spacing we use for text captions on buttons
    VerticalSpacingOnButton        = 5,    //The vertical spacing we use for text captions on buttons
    InternalSpacingForCheckBox     = 18,   //Additional horizontal spacing we use for Check Box width
    ProgressControlHeight          = 14,   //The height of the progress control
    SpacingSize                    = 5,    //The minimum distance between dialog controls
    InitialMarqueeUpdates          = 20,   //The default milliseconds interval to use when the progress control is in marquee mode
    HorizontalStaticSpace          = 0,    //Additional horizontal space to allocate to statics
    HorizontalExpandoSize          = 20,   //The horizontal size of the Expando button
    VerticalExpandoSize            = 20,   //The vertical size of the Expando button
  };

//Static methods
  static HRESULT CALLBACK TaskDialogCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LONG_PTR lpRefData);

//Non static methods
  HRESULT Display(int* pnButton);
  HRESULT Layout(CRect& rDialog);
  HRESULT AddItem(CXTaskDialogItem::ControlType cType,UINT nID, const CRect* pRect,	LPCTSTR pszCaption, short nXDialogBaseUnit, short nYDialogBaseUnit, DWORD dwAdditionalStyles = 0, DWORD dwStylesToRemove = 0);
  int&	  GetButtonCount() { return m_nButton; };
  HRESULT LoadString(HINSTANCE hInstance, CString& sString, PCWSTR pszID);
  HRESULT LoadMainIcon();
  HRESULT LoadFooterIcon();
  void    CalculateButtonMetrics(HDC hDC, const CString& sCaption, int nMaxWidth, int& nButtonWidth, int& nButtonHeight);
  void    CalculateButtonMetrics(HDC hDC, int nMaxWidth, int& nButtonWidth, int& nButtonHeight, int& nMaxButtonID, int& nButtons, int& nExpandoButtonWidth);
  void    CalculateCommandLinkMetrics(HDC hDC, HGDIOBJ hBigFont, HGDIOBJ hSmallFont, const CString& sCaption, int nTextWidth, int& nButtonHeight);
  void    SetChildControlFont(int nID, BOOL bLargeFont);
  void    HandleExpando();
  void    PlaySound();
  void    HideChildControl(int nID);
  void    ShowChildControl(int nID);
  void    MoveChildControl(int nID, int nVerticalMovement);
  CString CreateDisplayableText(const CString& sText);
  virtual HRESULT CreateGDIResources();
  virtual HRESULT LoadStringResources();
  virtual HRESULT LoadIconResources();
  void    DestroyChild(int nDlgItem);
  void    DestroyChildren();
  void    FreeResources();
  void    DestroyTimers();

//Member variables
  int                               m_nButton;                         //current button no.
  DLGTEMPLATE                       m_dlgTempl;                        //message box dialog template
  CString                           m_sWindowTitle;                    //The window title
  UINT_PTR                          m_nTimerID;                        //Our custom timer
  UINT_PTR                          m_nExpandoTimerID;                 //The timer we use for doing the expando expansion / collapsed animation
  const TASKDIALOGCONFIG*           m_pTaskConfig;                     //The struct we have been called with
  FILETIME                          m_LastTickCount;                   //The start of the tick count we pass in TDN_TIMER notifications
  HFONT                             m_hFont;                           //Font for all the other text   
  BOOL                              m_bCancelButtonPresent;            //Have we added a standard Cancel button
  int                               m_nIDDefaultButton;                //The ID of the default button
  int                               m_nYDivider1;                      //The Y offset where we draw the first white section
  int                               m_nFinalYDivider1;                 //Value of m_nYDivider1 after expando animation
  int                               m_nYDivider2;                      //The Y offset where we draw the footer white section
  int                               m_nFinalYDivider2;                 //Value of m_nYDivider2 after expando animation
  int                               m_nYDivider3;                      //The Y offset where we draw the TDF_EXPAND_FOOTER_AREA divider
  int                               m_nFinalYDivider3;                 //Value of m_nYDivider3 after expando animation
  COLORREF                          m_colorTopBackground;              //The background color for the top of the Task Dialog
  COLORREF                          m_colorStandardText;               //The text color for standard sized text
  COLORREF                          m_colorDividerBackground;          //the background color for the divider section
  COLORREF                          m_colorDivider;                    //The color we use for the divider section
  HBRUSH                            m_brushTopBackground;              //The brush we use for the top background
  HBRUSH                            m_brushDividerBackground;          //The brush we use for the divider section background
  int                               m_nMaxButtonID;                    //The maximum button id numeric value
  CSimpleArray<CXTaskDialogItem*>   m_dlgItems;                        //The array of the control items we will be creating in the task dialog
  HICON                             m_hMainIcon;                       //The main icon
  int                               m_nMainIconID;                     //The ID of the main icon
  CString                           m_sMainInstruction;                //The main instruction text
  CXTaskDialogIconStatic            m_ctrlMainIcon;                    //The control for the main icon
  HFONT                             m_hMainInstructionFont;            //font for the main instruction text
  int                               m_nMainInstructionID;              //The ctrl ID for the main instruction text
  CXTaskDialogStatic                m_ctrlMainInstructionText;         //The control for the main instruction text
  COLORREF                          m_colorMainInstruction;            //The color to use for the main instruction text
  int                               m_nProgressID;                     //The ctrl ID for the progress control
  HICON                             m_hFooterIcon;                     //The footer icon
  int                               m_nFooterIconID;                   //The ID of the footer icon
  int                               m_nFooterID;                       //The ID of the footer
  CXTaskDialogStatic                m_ctrlFooterText;                  //The control for the footer text  
  CString                           m_sFooter;                         //The footer text
  CString                           m_sDisplayableFooter;              //The footer text with any hyperlinks removed (used for text metrics)
  CXTaskDialogIconStatic            m_ctrlFooterIcon;                  //The control for the footer icon
  int                               m_nContentID;                      //The control ID for the content text
  CString                           m_sContent;                        //The content text
  CString                           m_sDisplayableContent;             //The context text with any hyperlinks removed (used for text metrics)
  CXTaskDialogStatic                m_ctrlContentText;                 //The control for the content text
  int                               m_nVerificationCheckBoxID;         //The ctrl ID for the verification check box
  CString                           m_sVerificationText;               //The caption to use for the verification checkbox
  CXTaskDialogButton                m_ctrlVerificationCheckBox;        //The control for the verification check box
  BOOL                              m_bVerificationFlagChecked;        //Used to return the state of the verification checkbox
  int                               m_nExpandedTextID;                 //The control ID for the expanded text
  int                               m_nExpandoButtonID;                //The control ID for the expanded button
  BOOL                              m_bExpandedExpanded;               //Is the expando button in the "Expanded" state
  CXTaskDialogStatic                m_ctrlExpandedText;                //The control for the expanded text
  CString                           m_sExpanded;                       //The expanded text
  CString                           m_sDisplayableExpanded;            //The expanded text with any hyperlinks removed (used for text metrics)
  CString                           m_sDefaultExpandoCollapsedCaption; //The default text to use for the expando button when it is "collapsed"
  CString                           m_sDefaultExpandoExpandedCaption;  //The default text to use for the expando button when it is "expanded"
  CString                           m_sExpandoCollapsedCaption;        //The actual text to use for the expando button when it is "collapsed"
  CString                           m_sExpandoExpandedCaption;         //The actual text to use for the expando button when it is "expanded"
  int                               m_nRadioButtonChecked;             //Used to return which radio button was selected
  CSimpleArray<CXTaskDialogButton*> m_ctrlsRadio;                      //The array of subclassed radio buttons
  CSimpleArray<CXTaskDialogCommandLink*> m_ctrlsCommandLinks;          //The array of subclassed command link buttons
  int                               m_nFinalExpandoVerticalMovement;   //How much the expando animation needs to increase the window height by
  int                               m_nCurrentExpandoVerticalMovement; //The current increase in window height for the expando animation
  HICON                             m_hCommandLink;                    //The icon we use on the command link buttons
  HICON                             m_hHotCommandLink;                 //The icon we use on "hot" command link buttons
  HICON                             m_hBigInformation;                 //The 32*32 icon we use for TD_INFORMATION_ICON
  HICON                             m_hSmallInformation;               //The 16*16 icon we use for TD_INFORMATION_ICON
  HICON                             m_hBigWarning;                     //The 32*32 icon we use for TD_WARNING_ICON
  HICON                             m_hSmallWarning;                   //The 16*16 icon we use for TD_WARNING_ICON
  HICON                             m_hBigError;                       //The 32*32 icon we use for TD_ERROR_ICON
  HICON                             m_hSmallError;                     //The 16*16 icon we use for TD_ERROR_ICON
  HICON                             m_hBigShield;                      //The 32*32 icon we use on buttons for the UAC shield
  HICON                             m_hSmallShield;                    //The 16*16 icon we use on buttons for the UAC shield
  BOOL                              m_bMainIconIsInteral;              //Is the main icon one of our internal icons (Used in OnInitDialog)
  CXTaskDialogButtonDetails         m_CommandButtons[6];               //The command buttons array
  BOOL                              m_bDoExpandoAnimation;             //Should expando animations be done

  BEGIN_MSG_MAP(CXTaskDialog)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_HELP, OnHelp)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    MESSAGE_HANDLER(WM_COMMAND, OnCommand)
    MESSAGE_HANDLER(WM_TIMER, OnTimer)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
    MESSAGE_HANDLER(TDM_UPDATE_ICON, OnUpdateIcon)
    MESSAGE_HANDLER(TDM_ENABLE_BUTTON, OnEnableButton)
    MESSAGE_HANDLER(TDM_ENABLE_RADIO_BUTTON, OnEnableRadioButton)
    MESSAGE_HANDLER(TDM_CLICK_BUTTON, OnClickButton)
    MESSAGE_HANDLER(TDM_CLICK_VERIFICATION, OnClickVerification)
    MESSAGE_HANDLER(TDM_CLICK_RADIO_BUTTON, OnClickRadioButton)
    MESSAGE_HANDLER(TDM_SET_PROGRESS_BAR_RANGE, OnSetProgressBarRange)
    MESSAGE_HANDLER(TDM_SET_MARQUEE_PROGRESS_BAR, OnSetMarqueeProgressBar)
    MESSAGE_HANDLER(TDM_SET_PROGRESS_BAR_POS, OnSetProgressBarPos)
    MESSAGE_HANDLER(TDM_SET_PROGRESS_BAR_STATE, OnSetProgressBarState)
    MESSAGE_HANDLER(TDM_SET_ELEMENT_TEXT, OnSetElementText)
    MESSAGE_HANDLER(TDM_UPDATE_ELEMENT_TEXT, OnUpdateElementText)
    MESSAGE_HANDLER(TDM_SET_BUTTON_ELEVATION_REQUIRED_STATE, OnSetButtonElevationRequiredState)
    MESSAGE_HANDLER(TDM_NAVIGATE_PAGE, OnNavigatePage)
    MESSAGE_HANDLER(WM_XTASKDIALOG_HANDLE_NAVIGATE_PAGE, OnHandleNavigatePage)
    NOTIFY_CODE_HANDLER(NM_CLICK, OnClick)
    REFLECT_NOTIFICATIONS()
  END_MSG_MAP()
 
  UINT ConvertToRealId(UINT id); 
  LRESULT OnInitDialog(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnHelp(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnCreate(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnDestroy(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnCommand(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnEraseBackground(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnTimer(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnUpdateIcon(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnEnableButton(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnClickButton(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSetProgressBarRange(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSetMarqueeProgressBar(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSetProgressBarPos(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSetProgressBarState(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSetElementText(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnUpdateElementText(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnClickRadioButton(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnClickVerification(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnEnableRadioButton(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnSetButtonElevationRequiredState(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnNavigatePage(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
  LRESULT OnClick(int nID, LPNMHDR pnmh, BOOL& bHandled);
  LRESULT OnHandleNavigatePage(UINT nMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

#endif //#ifndef __XTASKDIALOG_H__
