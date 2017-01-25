 /*
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
 
#define NDEBUG

#include <main.h>

WINE_DEFAULT_DEBUG_CHANNEL(vernel32);

HRESULT 
WINAPI 
WerpRecoveryInProgress(PBOOL pbCancelled)
{
  HRESULT result; // eax@2
  int v2 = 0; // eax@3
  HANDLE *v3; // esi@5
  BOOL v4 = FALSE; // [sp+0h] [bp-Ch]@0
  HANDLE *v5; // [sp+8h] [bp-4h]@1

  v5 = 0;
  if ( pbCancelled )
  {
    *pbCancelled = 0;
    if ( v2 >= 0 )
    {
      v3 = v5;
      if ( v5[12] )
      {
        if ( WaitForSingleObject(v5[10], 0) )
        {
          if ( v3 )
            SetEvent(v3[6]);
        }
        else
        {
          *pbCancelled = 1;
        }
        result = 0;
      }
      else
      {
        result = E_FAIL;
      }
    }
    else
    {
      TRACE(0, "WerpGetRecoveryInfoForSelf failed with 0x%x", v2);
      result = E_FAIL;
    }
  }
  else
  {
    TRACE(0, "WerpRecoveryInProgress Invalid params", v4);
    result = E_INVALIDARG;
  }
  return result;
}

HRESULT WINAPI ApplicationRecoveryInProgress(PBOOL pbCancelled)
{
  return WerpRecoveryInProgress(pbCancelled);
}

HRESULT WINAPI UnregisterApplicationRestart(void)
{
	return S_OK;
}

HRESULT WINAPI UnregisterApplicationRecoveryCallback(void)
{
	return S_OK;
}
