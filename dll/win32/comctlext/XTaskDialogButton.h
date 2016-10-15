/*
Module : XTaskDialogButton.h
Purpose: Defines the CXTaskDialogButton class as used by CXTaskDialog
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

#ifndef __XTASKDIALOGBUTTON_H__
#define __XTASKDIALOGBUTTON_H__

#ifndef CXTASKDIALOG_EXT_CLASS
#define CXTASKDIALOG_EXT_CLASS
#endif //#ifndef CXTASKDIALOG_EXT_CLASS


////////////////////////////// Classes ////////////////////////////////////////

//Class which draws a button / check box in a transparent fashion as required for our custom Task Dialog
class CXTASKDIALOG_EXT_CLASS CXTaskDialogButton : public ATL::CWindowImpl<CXTaskDialogButton>
{
public:
//Constructors / Destructors
  CXTaskDialogButton() : m_clrBackgroundColor(RGB(255, 255, 255)),
                         m_pBrushBackground(NULL)
  {
  }
  
//Methods  
  void Init(COLORREF colorBackground, HBRUSH* pBrushBackground)
  {
    m_clrBackgroundColor = colorBackground;
    m_pBrushBackground = pBrushBackground;
  }

protected:
  BEGIN_MSG_MAP(CXTaskDialogButton);
    MESSAGE_HANDLER(OCM_CTLCOLORSTATIC, OnControlColorStatic)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
  END_MSG_MAP()
  
  LRESULT OnControlColorStatic(UINT /*nMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    //Validate our parameters
    ATLASSUME(m_pBrushBackground != NULL);

    HDC hDC = reinterpret_cast<HDC>(wParam);
    SetBkMode(hDC, TRANSPARENT);
    return reinterpret_cast<LRESULT>(*m_pBrushBackground);
  }
  
  LRESULT OnEraseBackground(UINT /*nMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    //Fill the entire client area with the background color
    HDC hDC = reinterpret_cast<HDC>(wParam);
    SetBkColor(hDC, m_clrBackgroundColor);
    CRect rClient;
    GetClientRect(&rClient);
    ExtTextOut(hDC, 0, 0, ETO_OPAQUE, &rClient, NULL, 0, NULL);

    return 0;
  }
  
//Member variables  
  COLORREF m_clrBackgroundColor; //The color of our background
  HBRUSH*  m_pBrushBackground;   //The background brush to use
};

#endif //#ifndef __XTASKDIALOGBUTTON_H__
