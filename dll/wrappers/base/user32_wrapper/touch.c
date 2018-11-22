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

void WINAPI FreeUMHandleEntry(PRTL_CRITICAL_SECTION a1, PBOOL a2)
{
  PBOOL resp; // ebx@1
  LONG recursion; // esi@2
  LONG count; // esi@2
  PBOOL v5; // [sp+10h] [bp+Ch]@2

  resp = a2;
  if ( a2[1] )
  {
    recursion = (LONG)(a2 - a1->RecursionCount);
    v5 = a2;
    count = recursion >> 4;
    if ( resp[3] )
      RtlFreeHeap(pUserHeap, 0, (HANDLE)resp[3]);
    *resp = 0;
    resp[1] = 0;
    resp[2] = 0;
    resp[3] = 0;
    *resp = (LONG)v5 + 1;
    --*&a1->DebugInfo->CreatorBackTraceIndexHigh;
    if ( count < a1->LockCount )
      a1->LockCount = count;
  }
}

PBOOL WINAPI UMHandleActiveEntryFromHandle(RTL_CRITICAL_SECTION a1, HANDLE a2)
{
  PBOOL result = FALSE; // eax@2 
  DbgPrint("UMHandleActiveEntryFromHandle is UNIMPLEMENTED\n");  
  return result;
}

BOOL WINAPI UnlockUMHandleList(PRTL_CRITICAL_SECTION CriticalSectionObject)
{
  NTSTATUS status = STATUS_SUCCESS; // eax@1
  BOOL result; // eax@2

  RtlLeaveCriticalSection(CriticalSectionObject);
  if ( status >= 0 )
  {
    result = TRUE;
  }
  else
  {
    SetLastError(status);
    result = FALSE;
  }
  return result;
}

BOOL WINAPI LockUMHandleList(PRTL_CRITICAL_SECTION CriticalSectionObject)
{
  NTSTATUS status = STATUS_SUCCESS; // eax@1
  BOOL result; // eax@2

  RtlEnterCriticalSection(CriticalSectionObject);
  if ( status >= 0 )
  {
    result = TRUE;
  }
  else
  {
    SetLastError(status);
    result = FALSE;
  }
  return result;
}

BOOL WINAPI CloseTouchInputHandle(HTOUCHINPUT a1)
{
  BOOL result; // eax@2
  PBOOL v2; // eax@4
  PBOOL v3; // edi@4

  if ( a1 )
  {
    result = LockUMHandleList(&CriticalSectionObject);
    if ( result )
    {
      v2 = UMHandleActiveEntryFromHandle(CriticalSectionObject, a1);
      v3 = v2;
      if ( v2 )
        FreeUMHandleEntry(&CriticalSectionObject, v2);
      else
        SetLastError(6);
      UnlockUMHandleList(&CriticalSectionObject);
      result = v3 != FALSE;
    }
  }
  else
  {
    result = FALSE;
  }
  return result;
}

BOOL WINAPI CloseGestureInfoHandle(HGESTUREINFO  hGestureInfo)
{
  BOOL result; // eax@2
  PBOOL hwnd; // eax@4
  PBOOL compare; // edi@4

  if ( hGestureInfo )
  {
    result = LockUMHandleList(&CriticalSectionObject);
    if ( result )
    {
      hwnd = UMHandleActiveEntryFromHandle(CriticalSectionObject, (HANDLE)(PBOOL)hGestureInfo);
      compare = hwnd;
      if ( hwnd )
        FreeUMHandleEntry(&CriticalSectionObject, hwnd);
      else
        SetLastError(6);
      UnlockUMHandleList(&CriticalSectionObject);
      result = compare != FALSE;
    }
  }
  else
  {
    result = FALSE;
  }
  return result;
}

BOOL WINAPI RegisterTouchWindow(
  _In_  HWND hWnd,
  _In_  ULONG ulFlags
)
{
	DbgPrint("RegisterTouchWindow is UNIMPLEMENTED\n");  
	return TRUE;
}

BOOL WINAPI UnregisterTouchWindow(
  _In_  HWND hWnd
)
{
	DbgPrint("UnregisterTouchWindow is UNIMPLEMENTED\n");  
	return TRUE;
}

DWORD64 WINAPI GetTouchInputDataSize(UINT parameter)
{
  DWORD64 resp; // qax@1

  resp = 40i64 * parameter;
  if ( resp > 0xFFFFFFFF )
    resp = 0;
  return resp;
}

BOOL WINAPI GetTouchInputInfoWorker(HTOUCHINPUT hTouchInput, UINT cInputs, PTOUCHINPUT pInputs, int cbSize)
{
  UINT localInputs; // edi@1
  BOOL resp; // ebx@1
  PBOOL verification; // eax@4
  UINT sizeVerification; // eax@6
  UINT division; // esi@6
  DWORD64 dataSize; // eax@6
  DWORD64 LocalSizeA; // ecx@6
  UINT resulOperation; // eax@6
  BOOL error; // [sp-4h] [bp-Ch]@5
  PBOOL used; // [sp+10h] [bp+8h]@4
  UINT Sizea; // [sp+14h] [bp+Ch]@6

  localInputs = cInputs;
  resp = 1;
  if ( cInputs && pInputs )
  {
    if ( LockUMHandleList(&CriticalSectionObject) )
    {
      verification = (PBOOL)UMHandleActiveEntryFromHandle(CriticalSectionObject, hTouchInput);
      used = verification;
      if ( !verification )
      {
        error = 6;
LABEL_9:
        RtlSetLastWin32Error(error);
        resp = 0;
LABEL_13:
        UnlockUMHandleList(&CriticalSectionObject);
        return resp;
      }
      sizeVerification = verification[2];
      Sizea = sizeVerification;
      division = sizeVerification / 0x28;
      dataSize = GetTouchInputDataSize(sizeVerification / 0x28);
      LocalSizeA = Sizea;
      resulOperation = dataSize == Sizea ? division : 0;
      if ( cbSize )
      {
        if ( resulOperation != localInputs )
        {
          error = 87;
          goto LABEL_9;
        }
      }
      else
      {
        if ( resulOperation > localInputs )
          LocalSizeA = GetTouchInputDataSize(localInputs);
      }
      memcpy(pInputs, (const void *)used[3], LocalSizeA);
      goto LABEL_13;
    }
  }
  else
  {
    RtlSetLastWin32Error(87);
  }
  return 0;
}

BOOL WINAPI GetTouchInputInfo(
  _In_   HTOUCHINPUT hTouchInput,
  _In_   UINT cInputs,
  _Out_  PTOUCHINPUT pInputs,
  _In_   int cbSize
)
{
	  BOOL result; // eax@2
	  if ( cbSize == 40 )
	  {
		result = GetTouchInputInfoWorker(hTouchInput, cInputs, pInputs, 0);
	  }
	  else
	  {
		RtlSetLastWin32Error(87);
		result = 0;
	  }
	  return result;
}

BOOL WINAPI IsTouchWindow(
  _In_       HWND hWnd,
  _Out_opt_  PULONG pulFlags
)
{
	DbgPrint("IsTouchWindow is UNIMPLEMENTED\n");  
	return FALSE;
}

BOOL WINAPI UnregisterPointerInputTarget(
  _In_  HWND hwnd,
  _In_  POINTER_INPUT_TYPE  pointerType
)
{
	DbgPrint("UnregisterPointerInputTarget is UNIMPLEMENTED\n");  
	return TRUE;
}

HRESULT WINAPI PackTouchHitTestingProximityEvaluation(const TOUCH_HIT_TESTING_INPUT *pHitTestingInput, const TOUCH_HIT_TESTING_PROXIMITY_EVALUATION *pProximityEval)
{
  int proximityEval; // esi@1
  int proximityDistance; // edi@1
  HRESULT result; // eax@4

  proximityEval = pProximityEval->adjustedPoint.x - pHitTestingInput->point.x;
  proximityDistance = pProximityEval->adjustedPoint.y - pHitTestingInput->point.y;
  if ( pProximityEval->score > 0xFFFu || abs(proximityEval) >= 511 || abs(proximityDistance) >= 511 )
    result = 0xFFF00000u;
  else
    result = (proximityDistance & 0x3FF) + (((proximityEval & 0x3FF) + (pProximityEval->score << 10)) << 10);
  return result;
}

BOOL WINAPI PtInRect(const RECT *lprc, POINT pt)
{
  return lprc && pt.x >= lprc->left && pt.x < lprc->right && pt.y >= lprc->top && pt.y < lprc->bottom;
}
/*
__int32 WINAPI TouchTargetingCreateContact(const TOUCH_HIT_TESTING_INPUT *this, const RECT *lprc)
{
  POINT one; // ST04_8@1
  BOOL verification; // eax@1
  BOOLEAN compare; // zf@1
  LONG two; // edx@3
  LONG three; // edi@3
  __int32 result; // eax@3
  LONG eight; // esi@8
  LONG nine; // ecx@8
  int ten; // ST18_4@8
  int eleven; // ecx@8
  LONG twelven; // ST20_4@8
  LONG threeten; // ebx@8
  LONG v14; // eax@8
  LONG v15; // ST28_4@8
  LONG v16; // edi@8
  signed int v17; // eax@8
  int v18; // edx@8
  int v19; // ST2C_4@8
  LONG v20; // eax@8
  LONG v21; // edx@8
  __int32 v22; // eax@8
  LONG v23; // ebx@8
  LONG v24; // ecx@8
  LONG v25; // edx@8
  LONG v26; // eax@8
  LONG v27; // ecx@11
  LONG v28; // esi@11
  int v29; // ST18_4@11
  LONG v30; // edi@11
  LONG v31; // esi@11
  signed int v32; // kr00_4@11
  LONG v33; // ecx@11
  LONG v34; // ebx@11
  LONG v35; // ecx@11
  LONG v36; // ebx@11
  LONG v37; // ecx@12
  LONG v38; // [sp+10h] [bp-14h]@3
  RECT *v39; // [sp+18h] [bp-Ch]@1
  __int32 v40; // [sp+1Ch] [bp-8h]@3

  lprc[2].left = this->point.x;
  lprc[2].top = this->point.y;
  lprc[11].top = 1;
  lprc[2].right = this->orientation;
  lprc[1].left = this->boundingBox.left;
  lprc[1].top = this->boundingBox.top;
  lprc[1].right = this->boundingBox.right;
  lprc[1].bottom = this->boundingBox.bottom;
  lprc->left = this->nonOccludedBoundingBox.left;
  lprc->top = this->nonOccludedBoundingBox.top;
  lprc->right = this->nonOccludedBoundingBox.right;
  lprc->bottom = this->nonOccludedBoundingBox.bottom;
  one.y = this->point.y;
  v39 = (RECT *)&lprc[1];
  one.x = this->point.x;
  verification = PtInRect(lprc, one);
  compare = lprc[11].right == 0;
  lprc[11].left = verification;
  if ( compare )
  {
    lprc[11].bottom = 26458;
    lprc[11].right = 26458;
  }
  two = lprc[1].right;
  three = v39->left;
  result = lprc[1].right - v39->left;
  lprc[2].bottom = 12;
  v38 = two;
  v40 = result;
  if ( result < 4 || (result = lprc[1].bottom - lprc[1].top, result < 4) )
    lprc[2].bottom = 4;
  switch ( lprc[2].bottom )
  {
    case 4:
      v37 = lprc[1].top;
      result = lprc[1].bottom;
      lprc[3].left = three;
      lprc[3].top = v37;
      lprc[3].right = three;
      lprc[3].bottom = result;
      lprc[4].left = two;
      lprc[4].top = result;
      lprc[4].right = two;
      lprc[4].bottom = v37;
      break;
    case 8:
      v27 = lprc[1].bottom;
      v28 = lprc[1].top;
      lprc[3].left = three;
      lprc[3].top = (v28 + v27) / 2;
      v29 = (v28 + v27) / 2;
      v30 = 15 * v40 / 100 + three;
      lprc[3].right = v30;
      v31 = 15 * (v28 - v27) / 100 + v27;
      lprc[3].bottom = v31;
      v32 = two + v39->left;
      lprc[4].top = v27;
      v33 = v39->left;
      v34 = lprc[1].right;
      lprc[5].left = v34;
      v35 = v34 + v33 - v30;
      lprc[5].top = v29;
      v36 = lprc[1].top;
      lprc[4].bottom = v31;
      lprc[4].left = v32 / 2;
      lprc[4].right = v35;
      lprc[5].right = v35;
      result = lprc[1].bottom + v36 - v31;
      lprc[6].left = v32 / 2;
      lprc[5].bottom = result;
      lprc[6].top = v36;
      lprc[6].right = v30;
      lprc[6].bottom = result;
      break;
    case 0xC:
      eight = lprc[1].bottom;
      nine = lprc[1].top;
      lprc[3].top = (eight + nine) / 2;
      ten = (eight + nine) / 2;
      eleven = nine - eight;
      lprc[3].left = three;
      lprc[3].right = three + 7 * v40 / 100;
      twelven = three + 7 * v40 / 100;
      threeten = 25 * eleven / 100 + eight;
      lprc[3].bottom = threeten;
      v14 = lprc[1].left + 25 * v40 / 100;
      lprc[4].left = v14;
      v15 = v14;
      v16 = 7 * eleven / 100 + lprc[1].bottom;
      v17 = lprc[1].right + lprc[1].left;
      lprc[4].top = v16;
      lprc[4].bottom = lprc[1].bottom;
      lprc[4].right = v17 / 2;
      v18 = v39->left - v15;
      v19 = v17 / 2;
      v20 = v38 + v39->left - twelven;
      lprc[5].right = v20;
      v21 = v38 + v18;
      lprc[5].bottom = threeten;
      lprc[5].left = v21;
      lprc[6].left = v38;
      lprc[5].top = v16;
      lprc[6].top = ten;
      lprc[6].right = v20;
      v22 = lprc[1].top - threeten;
      v23 = lprc[1].bottom;
      v24 = v22 + v23;
      lprc[7].left = v21;
      v25 = lprc[1].top;
      lprc[6].bottom = v22 + v23;
      v26 = v23 + v25 - v16;
      lprc[7].top = v26;
      lprc[7].bottom = v25;
      lprc[8].top = v26;
      result = twelven;
      lprc[7].right = v19;
      lprc[8].left = v15;
      lprc[8].right = twelven;
      lprc[8].bottom = v24;
      break;
  }
  return result;
}*/

BOOL WINAPI _ValidatePointerTargetingInput(const TOUCH_HIT_TESTING_INPUT *testingInput)
{
  POINT pointY; // ST04_8@1
  RECT localRect; // qdi@1
  LONG right; // eax@8
  LONG left; // eax@10

  pointY.y = testingInput->point.y;
  localRect = testingInput->boundingBox;
  pointY.x = testingInput->point.x;
  return (PtInRect(&testingInput->boundingBox, pointY)
       || (right = testingInput->point.x, localRect.left == right)
       && testingInput->boundingBox.right == right
       && (left = testingInput->point.y, testingInput->boundingBox.top == left)
       && testingInput->boundingBox.bottom == left)
      && localRect.left <= testingInput->nonOccludedBoundingBox.left
      && testingInput->boundingBox.right >= testingInput->nonOccludedBoundingBox.right
      && testingInput->boundingBox.top <= testingInput->nonOccludedBoundingBox.top
      && testingInput->boundingBox.bottom >= testingInput->nonOccludedBoundingBox.bottom;
}
/*
BOOL WINAPI EvaluateProximityToRect(
  _In_   const RECT *controlBoundingBox,
  _In_   const TOUCH_HIT_TESTING_INPUT *pHitTestingInput,
  _Out_  TOUCH_HIT_TESTING_PROXIMITY_EVALUATION *pProximityEval
)
{
	
}*/

BOOL WINAPI GetGestureInfo(
  _In_   HGESTUREINFO hGestureInfo,
  _Out_  PGESTUREINFO pGestureInfo
)
{
	pGestureInfo = NULL;
	DbgPrint("GetGestureInfo is UNIMPLEMENTED\n");  
	return TRUE;
}

BOOL WINAPI SetGestureConfig(
  _In_  HWND hwnd,
  _In_  DWORD dwReserved,
  _In_  UINT cIDs,
  _In_  PGESTURECONFIG pGestureConfig,
  _In_  UINT cbSize
)
{
	pGestureConfig = NULL;
	DbgPrint("SetGestureConfig is UNIMPLEMENTED\n");  
	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	return FALSE;
}

BOOL WINAPI GetGestureConfig(
  _In_     HWND hwnd,
  _In_     DWORD dwReserved,
  _In_     DWORD dwFlags,
  _In_     PUINT pcIDs,
  _Inout_  PGESTURECONFIG pGestureConfig,
  _In_     UINT cbSize
)
{
	pGestureConfig = NULL;
	DbgPrint("GetGestureConfig is UNIMPLEMENTED\n");  
	SetLastError(ERROR_CALL_NOT_IMPLEMENTED);
	return FALSE;
}


BOOL WINAPI GetGestureExtraArgs(
  _In_   HGESTUREINFO hGestureInfo,
  _In_   UINT cbExtraArgs,
  _Out_  PBYTE pExtraArgs
)
{
	pExtraArgs = NULL;
	DbgPrint("GetGestureExtraArgs is UNIMPLEMENTED\n");  	
	return TRUE;
}

BOOL 
WINAPI 
EvaluateProximityToRect(
  _In_  const RECT                                   *controlBoundingBox,
  _In_  const TOUCH_HIT_TESTING_INPUT                *pHitTestingInput,
  _Out_       TOUCH_HIT_TESTING_PROXIMITY_EVALUATION *pProximityEval
)
{
	DbgPrint("EvaluateProximityToRect is UNIMPLEMENTED\n");  	
	return FALSE;
}

/**********************************************************************
 * GetPointerDevices [USER32.@]
 */
BOOL WINAPI GetPointerDevices(UINT32 *device_count, POINTER_DEVICE_INFO *devices)
{
    DbgPrint("GetPointerDevices (%p %p): partial stub\n", device_count, devices);

    if (!device_count)
        return FALSE;

    if (devices)
        return FALSE;

    *device_count = 0;
    return TRUE;
}

BOOL 
WINAPI 
GetPointerType(
  _In_  UINT32             pointerId,
  _Out_ POINTER_INPUT_TYPE *pointerType
)
{
	DbgPrint("GetPointerType is UNIMPLEMENTED\n");		
	*pointerType = PT_MOUSE;
	return TRUE;
}

BOOL 
WINAPI 
RegisterPointerDeviceNotifications(
  _In_ HWND window,
  _In_ BOOL notifyRange
)
{
	DbgPrint("RegisterPointerDeviceNotifications is UNIMPLEMENTED\n");		
	return TRUE;
}

BOOL 
WINAPI 
RegisterTouchHitTestingWindow(
  _In_ HWND  hwnd,
  _In_ ULONG value
)
{
	DbgPrint("RegisterTouchHitTestingWindow is UNIMPLEMENTED\n");		
	return TRUE;
}