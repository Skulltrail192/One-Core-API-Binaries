/***************************************************************************\
* ConvertDIBIcon
*
* Called when a cursor/icon in DIB format is loaded.  This converts the
* cursor/icon into the old format and returns the resource handle.  IE,
* grabs the DIB bits and transforms them into physical bitmap bits.
*
*
* DIB Formats for icons/cursors 101
*
* Old Win 3.0 format icons/cursors start with an OLDICON/OLDCURSOR header
* followed by a double high monochrome DIB.  The height refered to in the
* header is the icon/cursor height, not the DIB height which is twice as
* high.  The XOR mask is in the first-half of the DIB bits.
*
* Old PM format icons/cursors start with a BITMAPCOREHEADER and
* are identical to the current win 3.1/NT format thereafter.
*
* Current NT/Chicago/Win 3.1 format icons/cursors start with
* a BITAMPINFOHEADER.  The height of this header refers to the height
* of the first bitmap which may either be color or truely monochrome.
* If its color, it is followed by the monochrome AND mask bits imediately
* after the color bits.  If it is truely monochrome, the AND and XOR
* masks are totally contained in the first DIB bits and no more bits
* follow.
*
* 5-Oct-1994 SanfordS   Recreated
\***************************************************************************/

// HICON ConvertDIBIcon(
    // LPBITMAPINFOHEADER lpbih,
    // HINSTANCE          hmod,
    // LPCWSTR            lpName,
    // BOOL               fIcon,
    // DWORD              cxNew,
    // DWORD              cyNew,
    // UINT               LR_flags)
// {
    // LPBITMAPINFOHEADER lpbihNew = NULL;
    // LPSTR              lpBitsNextMask = NULL;
    // HICON              hicoNew = NULL;
    // BOOL               fOldFormat = FALSE;
    // CURSORDATA         cur;
    // WCHAR              achModName[MAX_PATH];

    // /*
     // * Because Icons/Cursors always get public bitmaps, we cannot use
     // * LR_CREATEDIBSECTION on them.
     // */
    // if (LR_flags & LR_CREATEDIBSECTION) {
        // LR_flags = (LR_flags & ~LR_CREATEDIBSECTION) | LR_CREATEREALDIB;
    // }

    // RtlZeroMemory(&cur, sizeof(cur));

    // if (!fIcon) {
        // /*
         // * Cursors have an extra two words preceeding the BITMAPINFOHEADER
         // * indicating the hot-spot.  After doing the increments, the
         // * pointer should be at the dib-header.
         // */
        // cur.xHotspot = (short)(int)*(((LPWORD)lpbih)++);
        // cur.yHotspot = (short)(int)*(((LPWORD)lpbih)++);
    // }

    // /*
     // * If the color bitmap is a single plane, 32bpp image, it might
     // * contain an alpha channel, so we have to preserve it as a DIB.
     // */
    // if (lpbih->biPlanes == 1 && lpbih->biBitCount == 32) {
        // LR_flags |= LR_CREATEREALDIB;
    // }

    // /*
     // * Get the XOR/Color mask.
     // * The XOR bits are first in the DIB because the header info
     // * pertains to them.
     // * The AND mask is always monochrome.
     // */
    // lpBitsNextMask = NULL;  // not passing lpBits in.
    // cur.hbmColor = ConvertDIBBitmap(lpbih,
                                    // cxNew,
                                    // cyNew,
                                    // LR_flags,
                                    // &lpbihNew,
                                    // &lpBitsNextMask);
    // if (cur.hbmColor == NULL)
        // return NULL;

    // if (hmod == NULL) {
        // cur.lpModName = NULL;
    // } else {
        // cur.CURSORF_flags = CURSORF_FROMRESOURCE;
        // if (hmod == hmodUser) {
            // cur.lpModName     = szUSER32;
        // } else  {
            // WowGetModuleFileName(hmod,
                              // achModName,
                              // sizeof(achModName) / sizeof(WCHAR));
            // cur.lpModName = achModName;
        // }
    // }
    // cur.rt     = (fIcon ? PTR_TO_ID(RT_ICON) : PTR_TO_ID(RT_CURSOR));
    // cur.lpName = (LPWSTR)lpName;
    // cur.bpp    = lpbihNew->biBitCount * lpbihNew->biPlanes;

    // if (cxNew == 0)
        // cxNew = lpbihNew->biWidth;

    // if (cyNew == 0)
        // cyNew = lpbihNew->biHeight / 2;

    // if (!fIcon) {

        // cur.xHotspot = MultDiv(cur.xHotspot,
                               // cxNew,
                               // lpbihNew->biWidth);
        // cur.yHotspot = MultDiv(cur.yHotspot,
                               // cyNew,
                               // lpbihNew->biHeight / 2);
    // } else {

        // /*
         // * For an icon the hot spot is the center of the icon
         // */
        // cur.xHotspot = (INT)(cxNew) / 2;
        // cur.yHotspot = (INT)(cyNew) / 2;
    // }

    // /*
     // * Setup header for monochrome DIB.  Note that we use the COPY.
     // */
    // ChangeDibColors(lpbihNew, LR_MONOCHROME);

    // if (lpBitsNextMask != NULL) {
        // cur.hbmMask = BitmapFromDIB(cxNew,
                                    // cyNew * 2,
                                    // 1,
                                    // 1,
                                    // 0,
                                    // lpbihNew->biWidth,
                                    // lpbihNew->biHeight,
                                    // lpBitsNextMask,
                                    // (LPBITMAPINFO)lpbihNew,
                                    // NULL);

        // if (cur.hbmMask == NULL) {
            // DeleteObject(KHBITMAP_TO_HBITMAP(cur.hbmColor));
            // UserLocalFree(lpbihNew);
            // return NULL;
        // }

    // } else {
        // cur.hbmMask = cur.hbmColor;
        // cur.hbmColor = NULL;
    // }

    // cur.cx = cxNew;
    // cur.cy = cyNew * 2;

    // /*
     // * Free our dib header copy allocated by ConvertDIBBitmap
     // */
    // UserLocalFree(lpbihNew);

    // if (LR_flags & LR_SHARED)
        // cur.CURSORF_flags |= CURSORF_LRSHARED;

    // if (LR_flags & LR_GLOBAL)
        // cur.CURSORF_flags |= CURSORF_GLOBAL;

    // if (LR_flags & LR_ACONFRAME)
        // cur.CURSORF_flags |= CURSORF_ACONFRAME;

    // return CreateIcoCur(&cur);
// }