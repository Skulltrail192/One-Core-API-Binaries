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

BOOL WINAPI GetCurrentInputMessageSource(INPUT_MESSAGE_DEVICE_TYPE *this, BOOL otherParamter)
{
  INPUT_MESSAGE_SOURCE *messageSource; // esi@1
  INPUT_MESSAGE_SOURCE *source; // edi@1
  BOOL otherVerification; // ecx@4
  BOOL verification; // [sp+20h] [bp-1Ch]@1

  messageSource = (INPUT_MESSAGE_SOURCE *)this;
  source = (INPUT_MESSAGE_SOURCE *)GetCurrentThread();
  verification = 0;
  if ((DWORD)(GetCurrentThread()) & 0x40000000 )
    verification = 1;
  if ((DWORD)(GetCurrentThread()) & 0x80000000 )
    otherVerification = 1;
  else
    otherVerification = 0;
  if ( otherParamter )
  {
    if ( !otherVerification && !(source[26].deviceType & 8) )
      goto LABEL_13;
  }
  else
  {
    if ( !verification && !(source[26].deviceType & 6) )
    {
LABEL_13:
      messageSource->deviceType = source[73].originId;
      messageSource->originId = source[74].deviceType;
      return 1;
    }
  }
  messageSource->deviceType = 0;
  messageSource->originId = 0;
  return 1;
}