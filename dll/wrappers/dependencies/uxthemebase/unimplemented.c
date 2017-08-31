/*
 * Win32 5.1 Themes
 *
 * Copyright (C) 2003 Kevin Koltzau
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
 */

#include "uxthemep.h"

typedef RECT *LPRECT;

__int32 WINAPI RegisterDefaultTheme(LPCWSTR lpData, BOOL a2)
{
	return 0;
}

HANDLE WINAPI DumpLoadedThemeToTextFile(const void *lp, LPCWSTR lpsz, int a3, int a4)
{
	return 0;
}

HANDLE WINAPI OpenThemeFileFromData(HANDLE a1, LPVOID lp)
{
	return 0;
}

HANDLE WINAPI GetThemeSysSize96(int a1, int a2)
{
	return 0;
}

HANDLE WINAPI GetThemeSysFont96(int a1, signed int a2, LPVOID lp)
{
	return 0;
}

LPHANDLE WINAPI SessionAllocate(HANDLE ProcessHandle, PVOID ProcessInformation, PVOID ReturnLength, PVOID a4, PVOID a5, PVOID a6, PVOID a7)
{
	return NULL;
}

HANDLE WINAPI SessionFree(PVOID a1)
{
	return 0;
}

HANDLE WINAPI ThemeHooksOn(PVOID a1)
{
	return 0;
}

HANDLE WINAPI ThemeHooksOff(PVOID a1)
{
	return 0;
}

HANDLE WINAPI AreThemeHooksActive(PVOID a1)
{
	return 0;
}

HANDLE WINAPI GetCurrentChangeNumber(PVOID a1)
{
	return 0;
}

HANDLE WINAPI GetNewChangeNumber(PVOID a1)
{
	return 0;
}

HANDLE WINAPI SetGlobalTheme(PVOID a1, PVOID a2)
{
	return 0;
}

HANDLE WINAPI GetGlobalTheme(PVOID a1, LPHANDLE lpTargetHandle)
{
	return 0;
}

HANDLE WINAPI LoadTheme(int a1, int a2, int a3, LPCWSTR lpString1, int a5, int a6, int a7)
{
	return 0;
}

void WINAPI InitUserTheme(HANDLE a1)
{
	;
}

HANDLE WINAPI InitUserRegistry()
{
	return 0;
}

HANDLE WINAPI ReestablishServerConnection()
{
	return 0;
}

HANDLE WINAPI RefreshThemeForTS()
{
	return 0;
}

HANDLE WINAPI ClassicGetSystemMetrics(int nIndex)
{
	return 0;
}

BOOL WINAPI ClassicSystemParametersInfoA(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni)
{
	return TRUE;
}

BOOL WINAPI ClassicSystemParametersInfoW(UINT uiAction, UINT uiParam, PVOID pvParam, UINT fWinIni)
{
	return TRUE;
}

BOOL WINAPI ClassicAdjustWindowRectEx(LPRECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle)
{
	return TRUE;
}

HANDLE WINAPI GetThemeParseErrorInfo(struct _PARSE_ERROR_INFO *lp)
{
	return 0;
}

HANDLE WINAPI CreateThemeDataFromObjects(const void *lp, const void *a2, int a3)
{
	return 0;
}

void WINAPI ServerClearStockObjects(HANDLE hFileMappingObject)
{
	;
}

void WINAPI MarkSection(HANDLE hFileMappingObject, int a2, DWORD a3)
{
	;
}

DWORD WINAPI IsThemeActiveByPolicy()
{
	return 0;
}

DWORD WINAPI ProcessLoadTheme_RunDLLW(int a1, int a2, LPWSTR hObject, int a4)
{
	return 0;
}

DWORD WINAPI ServiceClearStockObjects(int a1, HANDLE StartupContext)
{
	return 0;
}

DWORD WINAPI SetSystemVisualStyle(LPCWSTR lpLibFileName, int a2, int a3, WPARAM wParam)
{
	return 0;
}
