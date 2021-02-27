/*
Module : XTaskDialogItem.h
Purpose: Defines the CXTaskDialogItem class as used by CXTaskDialog
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

#ifndef __XTASKDIALOGITEM_H__
#define __XTASKDIALOGITEM_H__

#ifndef CXTASKDIALOG_EXT_CLASS
#define CXTASKDIALOG_EXT_CLASS
#endif //#ifndef CXTASKDIALOG_EXT_CLASS


////////////////////////////// Classes ////////////////////////////////////////

//Simple class which represents a single control on the in memory dialog template
class CXTASKDIALOG_EXT_CLASS CXTaskDialogItem
{
public:
//Enums
  enum ControlType 
  { 
    ICON, 
    BUTTON, 
    EDITCONTROL, 
    STATIC, 
    CHECKBOX,
    PROGRESS,
    HYPERLINK,
    RADIO 
  };

//Constructors / Templates
  CXTaskDialogItem(ControlType cType) : m_ControlType(cType),
                                        m_rRect(0, 0, 0, 0)
  { 
    switch (m_ControlType)
    {
      case ICON: //deliberate fallthrough
      case STATIC:
      {
        m_sSystemClass = WC_STATICW;
        break;
      }
      case BUTTON: //deliberate fallthrough
      case CHECKBOX: //deliberate fallthrough
      case RADIO:
      {
        m_sSystemClass = WC_BUTTONW;
        break;
      }
      case EDITCONTROL:
      {
        m_sSystemClass = WC_EDITW;
        break;
      }
      case PROGRESS:
      {
        m_sSystemClass = PROGRESS_CLASSW;
        break;
      }
      case HYPERLINK:
      {
        m_sSystemClass = WC_LINK;
        break;
      }
      default:
      {
        ATLASSERT(FALSE);
        break;
      }
    }
  }

//Member variables
  DLGITEMTEMPLATE m_dlgItemTemplate;
  ControlType     m_ControlType;
  CStringW        m_sCaption;
  CStringW        m_sSystemClass;
  CRect           m_rRect;
};

#endif //#ifndef __XTASKDIALOGITEM_H__
