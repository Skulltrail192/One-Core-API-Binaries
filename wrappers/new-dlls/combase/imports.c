/*
 *    Copyright 2005 Juan Lang
 *    Copyright 2005-2006 Robert Shearman (for CodeWeavers)
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

#define COBJMACROS
#define NONAMELESSUNION

#include "ntstatus.h"
#define WIN32_NO_STATUS
#define USE_COM_CONTEXT_DEF
#include "objbase.h"
#include "ctxtcall.h"
#include "oleauto.h"
#include "dde.h"
#include "wine/winternl.h"

#include "combase_private.h"

#include "wine/debug.h"
#include "wine/heap.h"

WINE_DEFAULT_DEBUG_CHANNEL(ole);

/***********************************************************************
 *        DestroyRunningObjectTable    (ole32.@)
*/
void WINAPI DestroyRunningObjectTable(void)
{
    struct rot_entry *rot_entry, *cursor2;

    TRACE("\n");

    EnterCriticalSection(&rot.lock);
    LIST_FOR_EACH_ENTRY_SAFE(rot_entry, cursor2, &rot.rot, struct rot_entry, entry)
    {
        list_remove(&rot_entry->entry);
        rot_entry_delete(rot_entry);
    }
    LeaveCriticalSection(&rot.lock);
}