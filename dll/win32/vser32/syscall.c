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
  source = (INPUT_MESSAGE_DEVICE_TYPE *)GetCurrentThread();
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
/*
DWORD_PTR WINAPI NtUserGetThreadState(int a1)
{
  DESKTOPINFO *info; // eax@1
  DESKTOPINFO *otherInfo; // esi@1
  LIST_ENTRY *list; // esi@2
  LIST_ENTRY *entry; // eax@16

  EnterCriticalSection(lpCriticalSection);
  otherInfo = info;
  if ( a1 )
  {
    switch ( a1 )
    {
      case 0:
        goto LABEL_2;
      case 2:
        list = info[1].aphkStart[1].Blink[4].Blink;
        if ( !list )
          break;
        goto LABEL_3;
      case 8:
        list = info[1].aphkStart[1].Blink[34].Flink;
        break;
      case 1:
        list = info[1].aphkStart[1].Blink[5].Blink;
        if ( !list )
          break;
        goto LABEL_3;
      case 3:
        list = info[2].aphkStart[1].Blink;
        if ( !list )
          break;
        goto LABEL_3;
      case 10:
        list = info[1].aphkStart[7].Blink;
        break;
      case 9:
        entry = info[1].aphkStart[6].Blink;
        if ( !entry )
          goto LABEL_25;
        if ( entry[1].Flink )
        {
          list = (LIST_ENTRY *)1;
        }
        else
        {
          if ( (unsigned int)entry[4].Blink & 0x300 )
            list = (LIST_ENTRY *)4;
          else
            list = (LIST_ENTRY *)2;
        }
        if ( (unsigned int)entry[4].Blink & 1 )
          list = (LIST_ENTRY *)((unsigned int)list | 8);
        break;
      case 4:
        list = info[2].aphkStart[2].Flink;
        if ( !list )
          break;
        goto LABEL_3;
      case 14:
        if ( GetCurrentProcessId() != gpidLogon && otherInfo[1].aphkStart[3].Flink != grpdeskLogon )
          goto LABEL_25;
        goto LABEL_27;
      case 11:
        if ( info[1].aphkStart[1].Blink == gpqForeground )
          goto LABEL_27;
        goto LABEL_25;
      case 15:
LABEL_27:
        list = (LIST_ENTRY *)1;
        break;
      case 6:
        list = info[1].aphkStart[1].Blink[31].Flink;
        if ( !list )
          break;
        goto LABEL_3;
      case 12:
        list = info[1].aphkStart[1].Flink[48].Flink;
        break;
      case 5:
        if ( !(GetQueueStatus(7295) & 5) )
          goto LABEL_25;
        list = (LIST_ENTRY *)1;
        break;
      case 13:
        list = info[2].aphkStart[2].Blink;
        break;
      case 7:
        list = (LIST_ENTRY *)LOWORD(info[1].aphkStart[2].Blink->Blink);
        break;
      default:
LABEL_25:
        list = 0;
        break;
    }
  }
  else
  {
LABEL_2:
    list = info[1].aphkStart[1].Blink[5].Flink;
    if ( list )
LABEL_3:
      list = list->Flink;
    else
      list = 0;
  }
  LeaveCriticalSection(lpCriticalSection);
  return (DWORD_PTR)list;
}*/