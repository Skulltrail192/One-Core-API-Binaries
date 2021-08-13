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
#include <ucrtbase.h>

WINE_DEFAULT_DEBUG_CHANNEL(errno);

/*********************************************************************
 *		__doserrno (MSVCRT.@)
 */
MSVCRT_ulong* CDECL MSVCRT___doserrno(void)
{
    return &msvcrt_get_thread_data()->thread_doserrno;
}

/* INTERNAL: Set the crt and dos errno's from the OS error given. */
void msvcrt_set_errno(int err)
{
  int *errno_ptr = MSVCRT__errno();
  MSVCRT_ulong *doserrno = MSVCRT___doserrno();

  *doserrno = err;

  switch(err)
  {
#define ERR_CASE(oserr) case oserr:
#define ERR_MAPS(oserr, crterr) case oserr: *errno_ptr = crterr; break
    ERR_CASE(ERROR_ACCESS_DENIED)
    ERR_CASE(ERROR_NETWORK_ACCESS_DENIED)
    ERR_CASE(ERROR_CANNOT_MAKE)
    ERR_CASE(ERROR_SEEK_ON_DEVICE)
    ERR_CASE(ERROR_LOCK_FAILED)
    ERR_CASE(ERROR_FAIL_I24)
    ERR_CASE(ERROR_CURRENT_DIRECTORY)
    ERR_CASE(ERROR_DRIVE_LOCKED)
    ERR_CASE(ERROR_NOT_LOCKED)
    ERR_CASE(ERROR_INVALID_ACCESS)
    ERR_CASE(ERROR_SHARING_VIOLATION)
    ERR_MAPS(ERROR_LOCK_VIOLATION,       MSVCRT_EACCES);
    ERR_CASE(ERROR_FILE_NOT_FOUND)
    ERR_CASE(ERROR_NO_MORE_FILES)
    ERR_CASE(ERROR_BAD_PATHNAME)
    ERR_CASE(ERROR_BAD_NETPATH)
    ERR_CASE(ERROR_INVALID_DRIVE)
    ERR_CASE(ERROR_BAD_NET_NAME)
    ERR_CASE(ERROR_FILENAME_EXCED_RANGE)
    ERR_MAPS(ERROR_PATH_NOT_FOUND,       MSVCRT_ENOENT);
    ERR_MAPS(ERROR_IO_DEVICE,            MSVCRT_EIO);
    ERR_MAPS(ERROR_BAD_FORMAT,           MSVCRT_ENOEXEC);
    ERR_MAPS(ERROR_INVALID_HANDLE,       MSVCRT_EBADF);
    ERR_CASE(ERROR_OUTOFMEMORY)
    ERR_CASE(ERROR_INVALID_BLOCK)
    ERR_CASE(ERROR_NOT_ENOUGH_QUOTA)
    ERR_MAPS(ERROR_ARENA_TRASHED,        MSVCRT_ENOMEM);
    ERR_MAPS(ERROR_BUSY,                 MSVCRT_EBUSY);
    ERR_CASE(ERROR_ALREADY_EXISTS)
    ERR_MAPS(ERROR_FILE_EXISTS,          MSVCRT_EEXIST);
    ERR_MAPS(ERROR_BAD_DEVICE,           MSVCRT_ENODEV);
    ERR_MAPS(ERROR_TOO_MANY_OPEN_FILES,  MSVCRT_EMFILE);
    ERR_MAPS(ERROR_DISK_FULL,            MSVCRT_ENOSPC);
    ERR_MAPS(ERROR_BROKEN_PIPE,          MSVCRT_EPIPE);
    ERR_MAPS(ERROR_POSSIBLE_DEADLOCK,    MSVCRT_EDEADLK);
    ERR_MAPS(ERROR_DIR_NOT_EMPTY,        MSVCRT_ENOTEMPTY);
    ERR_MAPS(ERROR_BAD_ENVIRONMENT,      MSVCRT_E2BIG);
    ERR_CASE(ERROR_WAIT_NO_CHILDREN)
    ERR_MAPS(ERROR_CHILD_NOT_COMPLETE,   MSVCRT_ECHILD);
    ERR_CASE(ERROR_NO_PROC_SLOTS)
    ERR_CASE(ERROR_MAX_THRDS_REACHED)
    ERR_MAPS(ERROR_NESTING_NOT_ALLOWED,  MSVCRT_EAGAIN);
  default:
    /*  Remaining cases map to EINVAL */
    /* FIXME: may be missing some errors above */
    *errno_ptr = MSVCRT_EINVAL;
  }
}

/*********************************************************************
 *		_errno (MSVCRT.@)
 */
int* CDECL MSVCRT__errno(void)
{
    return &msvcrt_get_thread_data()->thread_errno;
}