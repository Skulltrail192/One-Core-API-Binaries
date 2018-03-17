/*
 * Copyright 2014 Henri Verbeet for CodeWeavers
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
 
/*** ID2D1Bitmap methods ***/
FORCEINLINE D2D1_SIZE_F ID2D1Bitmap_GetSize(ID2D1Bitmap* This) {
    D2D1_SIZE_F __ret;
    return *This->lpVtbl->GetSize(This,&__ret);
} 

FORCEINLINE D2D1_PIXEL_FORMAT ID2D1RenderTarget_GetPixelFormat(ID2D1RenderTarget* This) {
    D2D1_PIXEL_FORMAT __ret;
    return *This->lpVtbl->GetPixelFormat(This,&__ret);
}

FORCEINLINE D2D1_SIZE_F ID2D1RenderTarget_GetSize(ID2D1RenderTarget* This) {
    D2D1_SIZE_F __ret;
    return *This->lpVtbl->GetSize(This,&__ret);
}

FORCEINLINE D2D1_SIZE_U ID2D1RenderTarget_GetPixelSize(ID2D1RenderTarget* This) {
    D2D1_SIZE_U __ret;
    return *This->lpVtbl->GetPixelSize(This,&__ret);
}