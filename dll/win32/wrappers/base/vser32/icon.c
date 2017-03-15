/*
 * Copyright 2009 Henri Verbeet for CodeWeavers
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
 */

#include <main.h>

/*
HBITMAP WINAPI CreateScreenBitmap(LONG first, LONG cy, COLORREF thrird, int four, COLORREF pvBits, int uneeded)
{
  HBITMAP localBitmap; // eax@18
  HBITMAP otherBitmap; // ebx@1
  HDC localHdc; // eax@2
  WORD witdth; // ax@20
  LPVOID pv; // [sp+8h] [bp-1Ch]@23
  DWORD compose = 0; // [sp+14h] [bp-10h]@23
  UINT16 other = 0; // [sp+18h] [bp-Ch]@23
  HDC hdc; // [sp+20h] [bp-4h]@3

  otherBitmap = 0;
  if ( gfSystemInitialized )
    localHdc = GetDC(0);
  else
    localHdc = CreateDCW(L"DISPLAY", &string, 0, 0);
  hdc = localHdc;
  if ( localHdc )
  {
    if ( four != 1 || thrird != 1 )
    {
      if ( thrird && thrird != LOBYTE(gpsi[88].bmiHeader.biHeight) || four && four != gpsi[88].bmiHeader.biHeight )
      {
        witdth = four * thrird;
        if ( !(four * thrird) )
          witdth = LOWORD(gpsi[88].bmiHeader.biWidth);
        globalInfoHeader.biBitCount = witdth;
        globalInfoHeader.biWidth = first;
        globalInfoHeader.biHeight = cy;
        localBitmap = CreateDIBitmap(hdc, &globalInfoHeader, 2u, 0, (const BITMAPINFO *)&globalInfoHeader, 0);
      }
      else
      {
        localBitmap = CreateCompatibleBitmap(hdc, first, cy);
      }
      otherBitmap = localBitmap;
    }
    else
    {
      pbmih.biWidth = first;
      pbmih.biHeight = cy;
      otherBitmap = CreateDIBitmap(hdc, &pbmih, 2u, 0, (const BITMAPINFO *)&pbmih, 0);
      uneeded = 1;
    }
    if ( gfSystemInitialized )
      ReleaseDC(0, hdc);
    else
      DeleteDC(hdc);
    if ( otherBitmap && pvBits )
    {
      GetObjectW(otherBitmap, 24, &pv);
      SetBitmapBits(otherBitmap, cy * compose * other, (const void *)pvBits);
    }
  }
  return otherBitmap;
}

BOOL WINAPI ScaleDIB(BITMAPINFOHEADER *infoHeader, const void *secondParameter, BITMAPINFO *bitmapInfo, LONG *unknown)
{
  BITMAPINFOHEADER *localInfoHeader; // ecx@1
  BITMAPINFO *localBitmapInfo; // eax@2
  unsigned int six; // edx@3
  LONG seven; // esi@4
  int eigth; // esi@4
  int nine; // edi@4
  int ten; // edx@4
  LONG eleven; // edx@5
  LONG twelven; // ecx@6
  LONG numberOne; // ebx@7
  LONG numberTwo; // eax@7
  BOOL result; // eax@8
  BOOLEAN *compare; // [sp+Ch] [bp-28h]@3
  BITMAPINFOHEADER *otherInfoHeader; // [sp+10h] [bp-24h]@4
  LONG Width; // [sp+14h] [bp-20h]@4
  int difference; // [sp+14h] [bp-20h]@7
  LONG otherSeven; // [sp+18h] [bp-1Ch]@4
  LONG biHeight; // [sp+3Ch] [bp+8h]@4
  int otherDifference; // [sp+3Ch] [bp+8h]@7
  LONG hum; // [sp+44h] [bp+10h]@4

  localInfoHeader = infoHeader;
  if ( infoHeader->biCompression
    || (localBitmapInfo = bitmapInfo, bitmapInfo->bmiHeader.biCompression)
    || (six = (unsigned int)bitmapInfo->bmiHeader.biBitCount >> 3,
        compare = (BOOLEAN *)((unsigned int)ScaleProc * (six + 4 * ((unsigned int)infoHeader->biBitCount >> 3))),
        !compare) )
  {
    result = 0;
  }
  else
  {
    Width = infoHeader->biWidth;
    hum = infoHeader->biHeight;
    seven = localBitmapInfo->bmiHeader.biWidth;
    otherSeven = seven;
    biHeight = localBitmapInfo->bmiHeader.biHeight;
    eigth = ((localBitmapInfo->bmiHeader.biBitCount * seven + 31) >> 3) & 0xFFFFFFFC;
    nine = ((localInfoHeader->biBitCount * Width + 31) >> 3) & 0xFFFFFFFC;
    localBitmapInfo->bmiHeader.biSizeImage = biHeight * eigth;
    otherInfoHeader = localInfoHeader;
    ten = InitDst[six];
    if ( ten )
      ((void (__stdcall *)(BITMAPINFO *))ten)(localBitmapInfo);
    eleven = otherSeven;
    if ( Width != 2 * otherSeven || (twelven = biHeight, hum != 2 * biHeight) )
    {
      difference = ((Width << 16) - 65536) / (otherSeven - 1);
      twelven = biHeight;
      otherDifference = ((hum << 16) - 65536) / (biHeight - 1);
      numberTwo = 0;
      numberOne = 0;
      eleven = otherSeven;
    }
    else
    {
      difference = 131072;
      otherDifference = 131072;
      numberOne = 32768;
      numberTwo = 32768;
    }    
    result = 1;
  }
  return result;
}

int WINAPI SmartStretchDIBits(HDC hdc, int xDest, int yDest, int DestWidth, int DestHeight, int xSrc, int ySrc, int SrcWidth, int SrcHeight, const void *lpBits, BITMAPINFOHEADER *lpbmi, UINT iUsage, DWORD rop)
{
  int caps; // ebx@12
  int other; // ebx@12
  WORD operation; // ecx@14
  WORD compose; // ecx@15
  BITMAPINFO *bitmapInfo; // eax@16
  DWORD Height; // esi@17
  WORD bitCount; // ax@6
  RGBQUAD prgbq; // [sp+Ch] [bp-24h]@1
  int resp; // [sp+10h] [bp-20h]@9
  BITMAPINFOHEADER *Address; // [sp+14h] [bp-1Ch]@1
  CPPEH_RECORD ms_exc; // [sp+18h] [bp-18h]@9

  Address = 0;
  if ( GetDIBColorTable(hdc, 0, 1u, &prgbq) != 1 && (DestWidth != SrcWidth || DestHeight != SrcHeight) )
  {
    if ( DestWidth >= SrcWidth / 2 )
    {
      if ( DestHeight >= SrcHeight / 2 )
      {
        if ( !lpbmi->biCompression )
        {
          bitCount = lpbmi->biBitCount;
          if ( bitCount == 4 || bitCount == 8 || bitCount == 24 )
          {
            caps = GetDeviceCaps(hdc, 14);
            other = (unsigned int)(GetDeviceCaps(hdc, 12) * caps) > 8 ? 24 : 8;
            if ( DestWidth > (signed)0xFFFFFFFFu / other
              || !DestHeight
              || (operation = (((unsigned int)(DestWidth * other) >> 3) + 3) & 0xFFFFFFFC,
                  operation > 0xFFFFFFFFu / DestHeight)
              || (compose = DestHeight * operation + 1064, compose < 0x428) )
              return 0;
            bitmapInfo = (BITMAPINFO *)RtlAllocateHeap(pUserHeap, 0, compose);
            Address = (BITMAPINFOHEADER *)bitmapInfo;
            if ( bitmapInfo )
            {
              ms_exc.registration.TryLevel = 0;
              memcpy(bitmapInfo, lpbmi, 0x28u);
              bitmapInfo->bmiHeader.biWidth = DestWidth;
              bitmapInfo->bmiHeader.biHeight = DestHeight;
              bitmapInfo->bmiHeader.biBitCount = other;
              Height = (DWORD)&bitmapInfo[24].bmiHeader.biHeight;
              if ( ScaleDIB(lpbmi, lpBits, bitmapInfo, &bitmapInfo[24].bmiHeader.biHeight) )
              {
                lpbmi = Address;
                lpBits = (const void *)Height;
                SrcWidth = DestWidth;
                SrcHeight = DestHeight;
              }
              ms_exc.registration.TryLevel = -1;
            }
          }
        }
      }
    }
  }
  ms_exc.registration.TryLevel = 1;
  resp = StretchDIBits(
           hdc,
           xDest,
           yDest,
           DestWidth,
           DestHeight,
           xSrc,
           ySrc,
           SrcWidth,
           SrcHeight,
           lpBits,
           (const BITMAPINFO *)lpbmi,
           iUsage,
           rop);
  ms_exc.registration.TryLevel = -1;
  if ( Address )
    RtlFreeHeap(pUserHeap, 0, Address);
  return resp;
}

HBITMAP WINAPI Convert1BppToMonoBitmap(HDC hdc, HBITMAP ho)
{
  HBITMAP localBitmap; // esi@1
  HDC localHdc; // ebx@1
  LPVOID pv; // [sp+Ch] [bp-24h]@2
  int nWidth = 0; // [sp+10h] [bp-20h]@2
  int cy = 0; // [sp+14h] [bp-1Ch]@2
  HGDIOBJ localobject; // [sp+24h] [bp-Ch]@3
  HGDIOBJ otherObject; // [sp+28h] [bp-8h]@3
  HBITMAP otherBitmap; // [sp+2Ch] [bp-4h]@2

  localBitmap = ho;
  localHdc = CreateCompatibleDC(hdc);
  if ( localHdc )
  {
    GetObjectW(ho, 24, &pv);
    otherBitmap = CreateBitmap(nWidth, cy, 1u, 1u, 0);
    if ( otherBitmap )
    {
      otherObject = SelectObject(hdc, ho);
      localobject = SelectObject(localHdc, otherBitmap);
      BitBlt(localHdc, 0, 0, nWidth, cy, hdc, 0, 0, 0xCC0020u);
      SelectObject(hdc, otherObject);
      SelectObject(localHdc, localobject);
      localBitmap = otherBitmap;
      DeleteObject(ho);
    }
    DeleteDC(localHdc);
  }
  return localBitmap;
}

HGDIOBJ WINAPI BitmapFromDIB(LONG DestWidth, LONG cy, COLORREF color, UINT16 number, int mode, int SrcWidth, int SrcHeight, PVOID lpBits, BITMAPINFOHEADER *pbmi, HPALETTE hPal)
{
  COLORREF otherReference; // edi@1
  BITMAPINFOHEADER *header; // esi@5
  LONG lowWitdh; // edi@7
  LONG width; // ebx@7
  int receiveCompression; // eax@7
  HBITMAP bitmap; // eax@10
  int resp; // eax@18
  PVOID ppvBits; // [sp+Ch] [bp-30h]@28
  DWORD compression; // [sp+10h] [bp-2Ch]@7
  int uneeded; // [sp+14h] [bp-28h]@1
  BOOL verification; // [sp+18h] [bp-24h]@19
  int other; // [sp+1Ch] [bp-20h]@4
  HGDIOBJ hbm; // [sp+20h] [bp-1Ch]@1
  CPPEH_RECORD ms_exc; // [sp+24h] [bp-18h]@17
  COLORREF colora; // [sp+4Ch] [bp+10h]@16
  COLORREF reference; // [sp+50h] [bp+14h]@16
  BOOL pbmia; // [sp+64h] [bp+28h]@6
  HGDIOBJ pbmib; // [sp+64h] [bp+28h]@15

  otherReference = 0;
  hbm = 0;
  uneeded = 0;
  RtlEnterCriticalSection(gcsHdc);
  if ( !DestWidth )
    DestWidth = SrcWidth;
  if ( !cy )
    cy = SrcHeight;
  if ( DestWidth != SrcWidth || (other = 0, cy != SrcHeight) )
    other = 1;
  header = pbmi;
  if ( mode & 0x2800 )
  {
    pbmia = LOWORD(pbmi->biSize) == 12;
    if ( pbmia )
    {
      lowWitdh = header->biWidth;
      width = header->biHeight;
      header->biWidth = DestWidth;
      header->biHeight = cy;
    }
    else
    {
      lowWitdh = header->biWidth;
      width = header->biHeight;
      compression = header->biCompression;
      receiveCompression = compression;
      header->biWidth = DestWidth;
      header->biHeight = cy;
      if ( receiveCompression != 3 )
        header->biCompression = 0;
    }
    if ( mode & 0x800 )
      bitmap = CreateDIBitmap(ghdcBits2, header, 2u, 0, (const BITMAPINFO *)header, 0);
    else
      bitmap = CreateDIBSection(ghdcBits2, (const BITMAPINFO *)header, 0, &ppvBits, 0, 0);
    hbm = bitmap;
    if ( pbmia )
    {
      header->biWidth = lowWitdh;
      header->biHeight = width;
    }
    else
    {
      header->biWidth = lowWitdh;
      header->biHeight = width;
      header->biCompression = compression;
    }
    otherReference = 0;
  }
  if ( hbm != (HGDIOBJ)otherReference
    || (hbm = CreateScreenBitmap(
                DestWidth,
                cy,
                color,
                number,
                otherReference,
                (int)&uneeded),
        hbm != (HGDIOBJ)otherReference) )
  {
    pbmib = SelectObject(ghdcBits2, hbm);
    if ( other != otherReference )
      mode = SetStretchBltMode(ghdcBits2, number * (unsigned __int16)color != 1 ? 3 : 1);
    reference = SetBkColor(ghdcBits2, 0xFFFFFFu);
    colora = SetTextColor(ghdcBits2, otherReference);
    if ( hPal != (HPALETTE)otherReference )
    {
      hPal = SelectPalette(ghdcBits2, hPal, otherReference);
      RealizePalette(ghdcBits2);
    }
    ms_exc.registration.TryLevel = otherReference;
    if ( other == otherReference )
      resp = SetDIBits(ghdcBits2, hbm, otherReference, cy, lpBits, (const BITMAPINFO *)header, otherReference);
    else
      resp = SmartStretchDIBits(
               ghdcBits2,
               otherReference,
               otherReference,
               DestWidth,
               cy,
               otherReference,
               otherReference,
               SrcWidth,
               SrcHeight,
               lpBits,
               header,
               otherReference,
               0xCC0020u);
    verification = resp <= 0;
    ms_exc.registration.TryLevel = -1;
    if ( hPal != (HPALETTE)otherReference )
    {
      SelectPalette(ghdcBits2, hPal, 1);
      RealizePalette(ghdcBits2);
    }
    SetTextColor(ghdcBits2, colora);
    SetBkColor(ghdcBits2, reference);
    if ( other != otherReference )
      SetStretchBltMode(ghdcBits2, mode);
    SelectObject(ghdcBits2, pbmib);
    if ( verification != otherReference )
    {
      DeleteObject(hbm);
      hbm = (HGDIOBJ)otherReference;
    }
  }
  if ( uneeded != otherReference && hbm != (HGDIOBJ)otherReference )
    hbm = (HGDIOBJ)Convert1BppToMonoBitmap(ghdcBits2, hbm);
  RtlLeaveCriticalSection(gcsHdc);
  return hbm;
}*/

BOOL 
WINAPI 
PrivateRegisterICSProc(RegisterCallback registrator)
{
  BOOL result; // eax@2

  if ( gpICSProc )
  {
    result = FALSE;
  }
  else
  {
    gpICSProc = registrator;
    result = TRUE;
  }
  return result;
}

/**********************************************************************
 *              GetIconInfoExW (USER32.@)
 */
BOOL WINAPI GetIconInfoExW(HICON icon, ICONINFOEXW *info )
{
  BOOL result = TRUE; // eax@2
  TCHAR res; // edi@3
  signed int sei; // eax@4
  char sizedo; // zf@6
  signed int resName; // eax@7
  WORD unsed; // ax@10
  PUNICODE_STRING compare; // [sp+4h] [bp-10h]@3
  TCHAR name; // [sp+8h] [bp-Ch]@3
  PUNICODE_STRING other; // [sp+Ch] [bp-8h]@3
  TCHAR size; // [sp+10h] [bp-4h]@3

  if ( info->cbSize == 1068 )
  {
    name = (WCHAR)(TCHAR)info->szModName[260];
    res = (WCHAR)(TCHAR)info->szResName[260];
    compare = (PUNICODE_STRING)0x1040000;
    other = (PUNICODE_STRING)0x1040000;
    size = (WCHAR)(TCHAR)info->szResName[260];
    result = GetIconInfo(icon, (PICONINFO)info->fIcon);
    if ( result )
    {
      sei = (signed int)compare;
      if ( compare >= (PUNICODE_STRING)0x103u )
        sei = 259;
      sizedo = size == res;
      *&info->szModName[2 * sei] = 0;
      if ( sizedo )
      {
        resName = (signed int)other;
        if ( other >= (PUNICODE_STRING)0x103u )
          resName = 259;
        *&info->szResName[2 * resName + 260] = 0;
        info->wResID = 0;
      }
      else
      {
        unsed = (unsigned __int32)size;
        res = 0;
        info->wResID = unsed;
      }
      result = 1;
    }
  }
  else
  {
    SetLastError(87);
    result = 0;
  }
  return result;
}

void WINAPI InternalGetWindowIcon(HWND window, HICON icon)
{
	;
}