/*
Module : XTaskDialogCommandLink.h
Purpose: Defines the CXTaskDialogCommandLink class as used by CXTaskDialog
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

#ifndef __XTASKDIALOGCOMMANDLINK_H__
#define __XTASKDIALOGCOMMANDLINK_H__

#ifndef CXTASKDIALOG_EXT_CLASS
#define CXTASKDIALOG_EXT_CLASS
#endif //#ifndef CXTASKDIALOG_EXT_CLASS

//Pull in support for _TrackMouseEvent automatically
#pragma comment(lib, "Comctl32.lib")

#ifndef WM_CHANGEUISTATE
#define WM_CHANGEUISTATE 0x0127
#endif //#ifndef WM_CHANGEUISTATE

#ifndef WM_UPDATEUISTATE
#define WM_UPDATEUISTATE 0x0128
#endif //#ifndef WM_UPDATEUISTATE

#ifndef WM_QUERYUISTATE
#define WM_QUERYUISTATE 0x0129
#endif //#ifndef WM_QUERYUISTATE

#ifndef UISF_HIDEFOCUS
#define UISF_HIDEFOCUS 0x1
#endif //#ifndef UISF_HIDEFOCUS

#ifndef UISF_HIDEACCEL
#define UISF_HIDEACCEL 0x2
#endif //#ifndef UISF_HIDEACCEL

#ifndef DT_HIDEPREFIX
#define DT_HIDEPREFIX 0x00100000
#endif //#ifndef DT_HIDEPREFIX


////////////////////////////// Classes ////////////////////////////////////////

//Class which draws a button / check box in a transparent fashion as required on our custom Task Dialog
class CXTASKDIALOG_EXT_CLASS CXTaskDialogCommandLink : public ATL::CWindowImpl<CXTaskDialogCommandLink>
{
public:
//Constructors / Destructors
  CXTaskDialogCommandLink() : m_penHotOutline(NULL),
                              m_penNormalOutline(NULL),
                              m_bHot(FALSE),
                              m_bMouseTracking(FALSE),
                              m_hBigFont(NULL),
                              m_hSmallFont(NULL),
                              m_hShieldIcon(NULL),
                              m_hCommandLinkIcon(NULL),
                              m_hHotCommandLinkIcon(NULL),
                              m_bShieldIcon(FALSE)
  {
  }
  
  ~CXTaskDialogCommandLink()
  {
    //Delete the objects now that we are finished with them
    if (m_penFocusOutline)
      DeleteObject(m_penFocusOutline);
    if (m_penHotOutline)
      DeleteObject(m_penHotOutline);
    if (m_penNormalOutline)
      DeleteObject(m_penNormalOutline);
  }
  
//Methods  
  void SetShieldIcon(BOOL bShield)
  {
    m_bShieldIcon = bShield;
    Invalidate();
  }

  BOOL Init(HGDIOBJ hBigFont, HGDIOBJ hSmallFont, HICON hShieldIcon, HICON hCommandLinkIcon, HICON hHotCommandLinkIcon)
  {
    m_hBigFont = hBigFont;
    m_hSmallFont = hSmallFont;
    m_hShieldIcon = hShieldIcon;
    m_hCommandLinkIcon = hCommandLinkIcon;
    m_hHotCommandLinkIcon = hHotCommandLinkIcon;
  
    //Delete the objects before we try to (re)create them
    if (m_penFocusOutline)
      DeleteObject(m_penFocusOutline);
    if (m_penHotOutline)
      DeleteObject(m_penHotOutline);
    if (m_penNormalOutline)
      DeleteObject(m_penNormalOutline);

    m_penFocusOutline = CreatePen(PS_SOLID, 1, FocusOutlineColor);
    if (m_penFocusOutline == NULL)
      return FALSE;
      
    m_penNormalOutline = CreatePen(PS_SOLID, 1, NormalOutlineColor); 
    if (m_penNormalOutline == NULL)
      return FALSE;
      
    m_penHotOutline = CreatePen(PS_SOLID, 1, HotOutlineColor);
    if (m_penHotOutline == NULL)
      return FALSE;
    
    return TRUE;
  }
  
  static int CalculateCommandLinkTextWidth(int nButtonWidth, BOOL bIcon)
  {
    int nTextWidth = nButtonWidth - 4*SpacingSize;
    if (bIcon)
      nTextWidth -= (16 + 2*SpacingSize);
    return nTextWidth;  
  }

protected:
  enum
  {
    SpacingSize = 5,
    RoundRectDiameter = 6,
    BackgroundColor = RGB(255, 255, 255), //If you know a better way of getting the colors Vista's command link buttons uses, then please let me know!
    SelectedEdgeColor = RGB(240, 240, 240),
    SelectedTextColor = RGB(0, 51, 153),
    HotTextColor = RGB(7, 74, 229),
    NormalTextColor = RGB(0, 51, 153),
    FocusOutlineColor = RGB(178, 240, 255),
    NormalOutlineColor = RGB(255, 255, 255),
    HotOutlineColor = RGB(198, 198, 198),
  };

  BEGIN_MSG_MAP(CXTaskDialogCommandLink);
    MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
    MESSAGE_HANDLER(WM_MOUSELEAVE, OnMouseLeave)
    MESSAGE_HANDLER(OCM_DRAWITEM, OnDrawItem)
    MESSAGE_HANDLER(WM_UPDATEUISTATE, OnUpdateUIState)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
  END_MSG_MAP()

  LRESULT OnDrawItem(UINT /*nMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
  {
    LPDRAWITEMSTRUCT lpDrawItem = reinterpret_cast<LPDRAWITEMSTRUCT>(lParam);
    ATLASSUME(lpDrawItem != NULL);
    CRect rClient(lpDrawItem->rcItem);

    //First draw the entire client area in the background color
    SetBkColor(lpDrawItem->hDC, BackgroundColor);
    ExtTextOut(lpDrawItem->hDC, 0, 0, ETO_OPAQUE, &rClient, NULL, 0, NULL);
    
    //Draw the command link button edge
    HGDIOBJ hOldBrush = SelectObject(lpDrawItem->hDC, GetStockObject(HOLLOW_BRUSH));
    HGDIOBJ hOldPen = NULL;
    if (m_bHot)
      hOldPen = SelectObject(lpDrawItem->hDC, m_penHotOutline);
    else if (lpDrawItem->itemState & ODS_FOCUS)
      hOldPen = SelectObject(lpDrawItem->hDC, m_penFocusOutline);
    else
      hOldPen = SelectObject(lpDrawItem->hDC, m_penNormalOutline);
    RoundRect(lpDrawItem->hDC, rClient.left, rClient.top, rClient.right, rClient.bottom, RoundRectDiameter, RoundRectDiameter);

    //Emphasize the button if it is selected
    if (lpDrawItem->itemState & ODS_SELECTED)
    {
      CRect rEmphasize(rClient);
      rEmphasize.DeflateRect(1, 1);
      SetBkColor(lpDrawItem->hDC, SelectedEdgeColor);
      ExtTextOut(lpDrawItem->hDC, 0, 0, ETO_OPAQUE, &rEmphasize, NULL, 0, NULL);
    }

    //Determine the UI State, note it is safe to use WM_QUERYUISTATE even on versions of Windows which
    //do not support this message (such as Windows 98 and Windows ME), since the message is designed in 
    //such a manner as to indicate that no hiding should be done on these OS'es, so in effect you get
    //the behaviour you would expect where hiding the UI state is not implemenented!.
    LRESULT nUIState = SendMessage(WM_QUERYUISTATE);
    BOOL bHideKeyboardAccelerators = ((nUIState & UISF_HIDEACCEL) != 0);
    BOOL bHideFocusRect = ((nUIState & UISF_HIDEFOCUS) != 0);

    //Draw the focus rect if necessary
    CSize border(GetSystemMetrics(SM_CXBORDER), GetSystemMetrics(SM_CYBORDER));
    CRect rFocus(rClient);
    rFocus.DeflateRect(2*border.cx, 3*border.cy);
    if ((lpDrawItem->itemState & ODS_FOCUS) && !bHideFocusRect)
      DrawFocusRect(lpDrawItem->hDC, &rFocus);
      
    //Draw the icon if necessary
    int nX = rClient.left + 2*SpacingSize;
    HICON hIcon = reinterpret_cast<HICON>(SendMessage(BM_GETIMAGE, IMAGE_ICON, 0));
    if (hIcon)
    {
      int nCXIcon = GetSystemMetrics(SM_CXSMICON);
      int nCYIcon = GetSystemMetrics(SM_CYSMICON);
      int nY = (rClient.Height() - nCYIcon) / 2;
      DrawIconEx(lpDrawItem->hDC, nX, nY, hIcon, nCXIcon, nCYIcon, 0, NULL, DI_NORMAL);
      nX += (nCXIcon + 2*SpacingSize);
    }

    //Get the window text
    int nLength = GetWindowTextLength();
    CString sWindowText;
    GetWindowText(sWindowText.GetBuffer(nLength+1), nLength+1);
    sWindowText.ReleaseBuffer();

    //Select in the big font
    HGDIOBJ hOldFont = SelectObject(lpDrawItem->hDC, m_hBigFont);
    
    int nEOLPos = sWindowText.Find(_T("\n"));
    if (nEOLPos == -1) //There is no small text provided
    {
      //Next calculate the height of the main text and then draw it
      CRect rTempText(rClient);
      DWORD dwFlags = DT_LEFT | DT_CALCRECT | DT_SINGLELINE | DT_EXPANDTABS;
      if (bHideKeyboardAccelerators)
        dwFlags |= DT_HIDEPREFIX;
      DrawText(lpDrawItem->hDC, sWindowText, -1, &rTempText, dwFlags);
      int nY = (rClient.Height() - rTempText.Height()) / 2;
      CRect rText(nX, nY, rClient.right - 2*SpacingSize, nY+rTempText.Height());
      SetBkMode(lpDrawItem->hDC, TRANSPARENT);
      if (lpDrawItem->itemState & ODS_SELECTED)
        SetTextColor(lpDrawItem->hDC, SelectedTextColor);
      else if (m_bHot)
        SetTextColor(lpDrawItem->hDC, HotTextColor);
      else
        SetTextColor(lpDrawItem->hDC, NormalTextColor);
      dwFlags = DT_LEFT | DT_SINGLELINE | DT_EXPANDTABS;
      if (bHideKeyboardAccelerators)
        dwFlags |= DT_HIDEPREFIX;
      DrawText(lpDrawItem->hDC, sWindowText, -1, &rText, dwFlags);
    }
    else
    {
      //Break the window text into the main text and the small text for the command link
      CString sMainText(sWindowText.Left(nEOLPos));
      CString sSmallText(sWindowText.Right(sWindowText.GetLength() - nEOLPos - 1));

      //Next calculate the height of the main text and then draw it
      CRect rTempText(rClient);
      DWORD dwFlags = DT_LEFT | DT_CALCRECT | DT_SINGLELINE | DT_EXPANDTABS;
      if (bHideKeyboardAccelerators)
        dwFlags |= DT_HIDEPREFIX;
      DrawText(lpDrawItem->hDC, sMainText, -1, &rTempText, dwFlags);
      int nY = 2*SpacingSize;
      CRect rText(nX, nY, rClient.right - 2*SpacingSize, nY+rTempText.Height());
      SetBkMode(lpDrawItem->hDC, TRANSPARENT);
      if (lpDrawItem->itemState & ODS_SELECTED)
        SetTextColor(lpDrawItem->hDC, SelectedTextColor);
      else if (m_bHot)
        SetTextColor(lpDrawItem->hDC, HotTextColor);
      else
        SetTextColor(lpDrawItem->hDC, NormalTextColor);
      dwFlags = DT_LEFT | DT_SINGLELINE | DT_EXPANDTABS;
      if (bHideKeyboardAccelerators)
        dwFlags |= DT_HIDEPREFIX;
      DrawText(lpDrawItem->hDC, sMainText, -1, &rText, dwFlags);
    
      //Select in the small font
      SelectObject(lpDrawItem->hDC, m_hSmallFont);

      //Next calculate the height of the main text and then draw it
      nY += (rTempText.Height() + SpacingSize);
      rTempText = rClient;
      DrawText(lpDrawItem->hDC, sSmallText, -1, &rTempText, DT_LEFT | DT_CALCRECT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
      rText = CRect(nX, nY, rClient.right - 2*SpacingSize, nY+rTempText.Height());
      DrawText(lpDrawItem->hDC, sSmallText, -1, &rText, DT_LEFT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
    }
    
    //Restore the DC before we return
    SelectObject(lpDrawItem->hDC, hOldBrush);
    SelectObject(lpDrawItem->hDC, hOldPen);
    SelectObject(lpDrawItem->hDC, hOldFont);

    return TRUE;
  }
  
  LRESULT OnEraseBackground(UINT /*nMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    //All the painting we need to do is done in the OnDrawItem handler, so to improve 
    //performance we do not erase anything here
    return 1;
  }
 
  void UpdateIcon(BOOL bHot)
  {
    HICON hIcon = reinterpret_cast<HICON>(SendMessage(BM_GETIMAGE, IMAGE_ICON, 0));
    if (bHot)
    {
      //Update our active icon
      if (hIcon == m_hCommandLinkIcon)
        SendMessage(BM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(m_hHotCommandLinkIcon));
    }
    else
    {    
      //Update our active icon
      if (hIcon == m_hHotCommandLinkIcon)
        SendMessage(BM_SETIMAGE, IMAGE_ICON, reinterpret_cast<LPARAM>(m_hCommandLinkIcon));
    }
  }
  
  LRESULT OnMouseMove(UINT /*nMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    //Force a redraw if we are going from not hot to hot
    if (!m_bHot)
    {
      m_bHot = TRUE;
      UpdateIcon(m_bHot);
      Invalidate();
    }
  
    //Ask to be informed when the mouse leaves us
    if (!m_bMouseTracking)
    {
      TRACKMOUSEEVENT tme;
      tme.cbSize = sizeof(tme);
      tme.hwndTrack = m_hWnd;
      tme.dwFlags = TME_LEAVE;
      tme.dwHoverTime = 1;
      m_bMouseTracking = _TrackMouseEvent(&tme);
    }

    //Let the default handling proceed
    return DefWindowProc();
  }
  
  LRESULT OnMouseLeave(UINT /*nMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    //Reset mouse tracking, the hot setting and force a redraw
    m_bMouseTracking = FALSE;
    m_bHot = FALSE;
    UpdateIcon(m_bHot);
    Invalidate();
    return 0;
  }  
  
  LRESULT OnUpdateUIState(UINT /*nMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    //Force a redraw
    Invalidate();
    
    //Let the default handling proceed
    return DefWindowProc();
  }  
  
  
//Member variables  
  HPEN    m_penFocusOutline;     //The pen we use for drawing if the button is focused
  HPEN    m_penHotOutline;       //The pen we use for drawing if the button is "hot"
  HPEN    m_penNormalOutline;    //The pen we use for drawing if the button is not "hot"
  HICON   m_hShieldIcon;         //The shield icon
  HICON   m_hCommandLinkIcon;    //The command link icon
  HICON   m_hHotCommandLinkIcon; //The "hot" command link icon
  BOOL    m_bHot;                //Is the button "hot"
  BOOL    m_bMouseTracking;      //Are we tracking the mouse
  HGDIOBJ m_hBigFont;            //The font we use for the main button text
  HGDIOBJ m_hSmallFont;          //The font we use for the smaller text
  BOOL    m_bShieldIcon;         //Should we use the shield icon
};

#endif //#ifndef __XTASKDIALOGCOMMANDLINK_H__
