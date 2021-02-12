/*
Module : XTaskDialogStatic.h
Purpose: Defines the CXTaskDialogStatic class as used by CXTaskDialog.
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

#ifndef __XTASKDIALOGSTATIC_H__
#define __XTASKDIALOGSTATIC_H__

#ifndef CXTASKDIALOG_EXT_CLASS
#define CXTASKDIALOG_EXT_CLASS
#endif //#ifndef CXTASKDIALOG_EXT_CLASS


////////////////////////////// Classes ////////////////////////////////////////

//Class which draws the static controls in a specified background color and specified text color
class CXTASKDIALOG_EXT_CLASS CXTaskDialogStatic : public ATL::CWindowImpl<CXTaskDialogStatic>
{
public:
//Constructors / Destructors
  CXTaskDialogStatic() : m_clrTextColor(RGB(0, 0, 0)), 
                         m_clrBackgroundColor(RGB(255, 255, 255)),
                         m_pBrushBackground(NULL)
  {
  }
  
//Methods  
  void Init(COLORREF clrColorText, COLORREF clrColorBackground, HBRUSH* pBrushBackground)
  {
    m_clrTextColor = clrColorText;
    m_clrBackgroundColor = clrColorBackground;
    m_pBrushBackground = pBrushBackground;
  }
  
protected:
  BEGIN_MSG_MAP(CTaskDialogStatic);
    MESSAGE_HANDLER(OCM_CTLCOLORSTATIC, OnControlColorStatic)
    MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBackground)
  END_MSG_MAP()
  
  LRESULT OnControlColorStatic(UINT /*nMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    HDC hDC = reinterpret_cast<HDC>(wParam);
    SetBkColor(hDC, m_clrBackgroundColor);
    SetTextColor(hDC, m_clrTextColor);
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

    return 1;
  }

//Member variables  
  COLORREF m_clrTextColor;       //The color to draw the text in  
  COLORREF m_clrBackgroundColor; //The color of our background
  HBRUSH*  m_pBrushBackground;   //The background brush to use
};

#endif //#ifndef __XTASKDIALOGSTATIC_H__
