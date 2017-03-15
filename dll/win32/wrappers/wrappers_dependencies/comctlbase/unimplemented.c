/*
 * Common controls functions
 *
 * Copyright 1997 Dimitrie O. Paun
 * Copyright 1998,2000 Eric Kohl
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA
 *
 * NOTES
 * 
 * This code was audited for completeness against the documented features
 * of Comctl32.dll version 6.0 on Oct. 21, 2002, by Christian Neumair.
 *
 * Unless otherwise noted, we believe this code to be complete, as per
 * the specification mentioned above.
 * If you discover missing features, or bugs, please note them below.
 *
 * TODO
 *   -- implement GetMUILanguage + InitMUILanguage
 *   -- finish NOTES for MenuHelp, GetEffectiveClientRect and GetStatusTextW
 *   -- FIXMEs + BUGS (search for them)
 *
 * Control Classes
 *   -- ICC_ANIMATE_CLASS
 *   -- ICC_BAR_CLASSES
 *   -- ICC_COOL_CLASSES
 *   -- ICC_DATE_CLASSES
 *   -- ICC_HOTKEY_CLASS
 *   -- ICC_INTERNET_CLASSES
 *   -- ICC_LINK_CLASS
 *   -- ICC_LISTVIEW_CLASSES
 *   -- ICC_NATIVEFNTCTL_CLASS
 *   -- ICC_PAGESCROLLER_CLASS
 *   -- ICC_PROGRESS_CLASS
 *   -- ICC_STANDARD_CLASSES (not yet implemented)
 *   -- ICC_TAB_CLASSES
 *   -- ICC_TREEVIEW_CLASSES
 *   -- ICC_UPDOWN_CLASS
 *   -- ICC_USEREX_CLASSES
 *   -- ICC_WIN95_CLASSES
 */

#include "comctl32.h"

#define NO_SHLWAPI_STREAM
#include <shlwapi.h>

WINCOMMCTRLAPI HRESULT WINAPI ImageList_ReadEx(_In_ DWORD dwFlags,
											   _In_ LPSTREAM pstm, 
											   _In_ REFIID riid, 
											   _Outptr_ PVOID *ppv)
{
	return S_OK;
}

WINCOMMCTRLAPI HRESULT WINAPI ImageList_WriteEx(_In_ HIMAGELIST himl, 
												  _In_ DWORD dwFlags, 
												  _In_ LPSTREAM pstm)
{
	return S_OK;	
}