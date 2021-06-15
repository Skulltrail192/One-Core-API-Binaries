/*
 * COPYRIGHT:        See COPYING in the top level directory
 * PROJECT:          ReactOS GDI32
 * PURPOSE:          GDI functions for Wrappers
 * FILE:             win32ss/gdi/gdi32/unimplemented.c
 * PROGRAMERS:       Alex Ionescu (alex@relsoft.net)
 *                   Magnus Olsen (magnus@greatlord.com)
 */

/* INCLUDES ******************************************************************/

#include <precomp.h>

#include <stdio.h>
#include <d3dhal.h>

typedef struct gdi_physdev
{
    const struct gdi_dc_funcs *funcs;
    struct gdi_physdev        *next;
    HDC                        hdc;
} *PHYSDEV;

/******************************************************************************
 *		D3DKMTCloseAdapter [GDI32.@]
 */
NTSTATUS WINAPI D3DKMTCloseAdapter( const D3DKMT_CLOSEADAPTER *desc )
{
	return STATUS_SUCCESS;
}

/******************************************************************************
 *		D3DKMTCreateDevice [GDI32.@]
 */
NTSTATUS WINAPI D3DKMTCreateDevice( D3DKMT_CREATEDEVICE *desc )
{
	return STATUS_SUCCESS;
}

/******************************************************************************
 *		D3DKMTDestroyDevice [GDI32.@]
 */
NTSTATUS WINAPI D3DKMTDestroyDevice( const D3DKMT_DESTROYDEVICE *desc )
{
	return STATUS_SUCCESS;
}

/******************************************************************************
 *		D3DKMTOpenAdapterFromGdiDisplayName [GDI32.@]
 */
NTSTATUS WINAPI D3DKMTOpenAdapterFromGdiDisplayName( D3DKMT_OPENADAPTERFROMGDIDISPLAYNAME *desc )
{
	return STATUS_SUCCESS;
}

/******************************************************************************
 *		D3DKMTSetVidPnSourceOwner [GDI32.@]
 */
NTSTATUS WINAPI D3DKMTSetVidPnSourceOwner( const D3DKMT_SETVIDPNSOURCEOWNER *desc )
{
	return STATUS_SUCCESS;
}

/******************************************************************************
 *		D3DKMTEscape [GDI32.@]
 */
NTSTATUS WINAPI D3DKMTEscape( const void *pData )
{
	return STATUS_SUCCESS;
}

/******************************************************************************
 *		D3DKMTOpenAdapterFromHdc [GDI32.@]
 */
NTSTATUS WINAPI D3DKMTOpenAdapterFromHdc( void *pData )
{
	return STATUS_SUCCESS;
}

/*************************************************************************
 *             GetFontFileData   (GDI32.@)
 */
BOOL WINAPI GetFontFileData( DWORD instance_id, DWORD unknown, UINT64 offset, void *buff, DWORD buff_size )
{
	return FALSE;
}

/*************************************************************************
 *             GetFontFileInfo   (GDI32.@)
 */
BOOL WINAPI GetFontFileInfo( DWORD instance_id, DWORD unknown, struct font_fileinfo *info, SIZE_T size, SIZE_T *needed )
{
	return FALSE;
}

/*************************************************************
 * freetype_GetFontRealizationInfo
 */
static BOOL CDECL freetype_GetFontRealizationInfo( PHYSDEV dev, void *ptr )
{
	return FALSE;
}

/*************************************************************
 *           GetFontRealizationInfo    (GDI32.@)
 */
BOOL WINAPI GetFontRealizationInfo(HDC hdc, struct font_realization_info *info)
{
	return FALSE;
}