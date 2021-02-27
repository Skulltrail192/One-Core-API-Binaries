/*
Module : XTaskDialogButton.h
Purpose: Defines the CXTaskDialogIconStatic class as used by CXTaskDialog
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

#ifndef __XTASKDIALOGICONSTATIC_H__
#define __XTASKDIALOGICONSTATIC_H__

#ifndef CXTASKDIALOG_EXT_CLASS
#define CXTASKDIALOG_EXT_CLASS
#endif //#ifndef CXTASKDIALOG_EXT_CLASS


////////////////////////////// Classes ////////////////////////////////////////

//A class which implements the custom drawing required for the icons on our custom Task Dialog
class CXTaskDialogIconStatic : public ATL::CWindowImpl<CXTaskDialogStatic>
{
public:
//Constuctors / Destructors
  CXTaskDialogIconStatic() : m_phIcon(NULL), 
                             m_bLarge(FALSE)
  {
  }
  
//Methods
  void Init(HICON* phIcon, BOOL bLarge)
  {
    m_phIcon = phIcon;
    m_bLarge = bLarge;
  }
  
protected:
  BEGIN_MSG_MAP(CXTaskDialogIconStatic);
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
  END_MSG_MAP()
  
  LRESULT OnPaint(UINT /*nMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
  {
    //Validate our parameters
    ATLASSERT(m_phIcon != NULL);

    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(&ps);
    if (m_bLarge)
    {
      int nCX = GetSystemMetrics(SM_CXICON);
      int nCY = GetSystemMetrics(SM_CYICON);
      DrawIconEx(hdc, 0, 0, *m_phIcon, nCX, nCY, 0, NULL, DI_NORMAL);
    }
    else
    {
      int nCX = GetSystemMetrics(SM_CXSMICON);
      int nCY = GetSystemMetrics(SM_CYSMICON);
      DrawIconEx(hdc, 0, 0, *m_phIcon, nCX, nCY, 0, NULL, DI_NORMAL);
    }
    EndPaint(&ps);
    return 0;
  }

//Member variables  
  HICON* m_phIcon; //The pointer to the icon to draw
  BOOL   m_bLarge; //Should we draw it large i.e. 32*32 (as opposed to 16*16)
};

#endif //#ifndef __XTASKDIALOGICONSTATIC_H__
