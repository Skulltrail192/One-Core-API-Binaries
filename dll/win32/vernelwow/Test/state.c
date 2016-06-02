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

#ifdef _M_IX86
PVOID WINAPI LocateXStateFeature(PCONTEXT Context, DWORD FeatureId, PDWORD Length)
{
  DWORD Flags; // eax@1
  BYTE reception; // esi@1
  PVOID result; // eax@7

  Flags = Context->ContextFlags;
  reception = 0;
  if ( Flags & 0x10000 )
  {
    if ( (Flags & 0x10040) == 65600 )
      reception = (BYTE)&Context[1];
    if ( !FeatureId )
    {
      if ( Length )
        *Length = 160;
      return Context->ExtendedRegisters;
    }
    if ( FeatureId == 1 )
    {
      if ( Length )
        *Length = 128;
      return &Context->ExtendedRegisters[160];
    }
    goto LABEL_23;
  }
  if ( !(Flags & 0x100000) )
    return 0;
  if ( (Flags & 0x100040) == 0x100040 )
    reception = (BYTE)&Context[1].ExtendedRegisters[312];
  if ( FeatureId )
  {
    if ( FeatureId != 1 )
    {
LABEL_23:
      if ( reception )
        return (PVOID)RtlLocateExtendedFeature(reception, FeatureId, Length);
      return 0;
    }
    if ( Length )
      *Length = 256;
    result = &Context->ExtendedRegisters[212];
  }
  else
  {
    if ( Length )
      *Length = 160;
    result = &Context->ExtendedRegisters[52];
  }
  return result;
}

BOOL WINAPI SetXStateFeaturesMask(PCONTEXT Context, DWORD64 FeatureMask)
{
  DWORD error; // eax@1
  PCONTEXT LocalContext; // esi@1
  NTSTATUS status; // [sp-4h] [bp-Ch]@15

  error = Context->ContextFlags;
  LocalContext = 0;
  if ( error & 0x10000 )
  {
    if ( (error & 0x10040) == 65600 )
      LocalContext = Context + 1;
    if ( FeatureMask & 3 )
    {
      Context->ContextFlags = error | 0x10020;
      if ( !LocalContext )
        goto LABEL_14;
      if ( LocalContext->Dr2 < 0x2CC )
        LocalContext->Dr2 = 716;
    }
  }
  else
  {
    if ( !(error & 0x100000) )
    {
      status = 0xC00000BBu;
      goto LABEL_20;
    }
    if ( (error & 0x100040) == 1048640 )
      LocalContext = Context;
    if ( FeatureMask & 3 )
      Context->ContextFlags = error | 0x100008;
  }
  if ( LocalContext )
    goto LABEL_17;
LABEL_14:
  if ( !(FeatureMask | FeatureMask & 0xFFFFFFFCu) )
  {
    if ( !LocalContext )
      return 1;
LABEL_17:
    RtlSetExtendedFeaturesMask(LocalContext, FeatureMask);
    return 1;
  }
  status = 0xC000000Du;
LABEL_20:
  RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
  return 0;
}

DWORD64 WINAPI GetEnabledXStateFeatures(void)
{
	return 3;
}

BOOL WINAPI GetXStateFeaturesMask(PCONTEXT Context, PDWORD64 FeatureMask)
{
  DWORD contextFlags; // ecx@1
  PDWORD64 longNumber; // qdi@1
  UCHAR *contextLocal; // edx@1
  DWORD flags; // eax@2
  DWORD other; // eax@4
  DWORD compare; // eax@6
  LONG highPart = 0i64; // edx@12

  contextFlags = Context->ContextFlags;
  longNumber = 0i64;
  contextLocal = 0;
  if ( contextFlags & 0x10000 )
  {
    flags = Context->ContextFlags;
    if ( (flags & 0x10040) == 65600 )
      contextLocal = (UCHAR *)&Context[1];
    other = 65568;
  }
  else
  {
    if ( !(contextFlags & 0x100000) )
    {
      RtlSetLastWin32ErrorAndNtStatusFromNtStatus(0xC00000BBu);
      return 0;
    }
    compare = Context->ContextFlags;
    if ( (compare & 0x100040) == 1048640 )
      contextLocal = &Context[1].ExtendedRegisters[312];
    other = 1048584;
  }
  if ( (other & contextFlags) == other )
    longNumber = (PDWORD64)3;
  if ( contextLocal )
  {
    longNumber = RtlGetExtendedFeaturesMask(contextLocal);
    longNumber = (PDWORD64)highPart;
  }
  FeatureMask = longNumber;
  return 1;
}

#elif defined(_M_AMD64)

PVOID WINAPI LocateXStateFeature(PWOW64_CONTEXT Context, DWORD FeatureId, PDWORD Length)
{
  DWORD Flags; // eax@1
  BYTE reception; // esi@1
  PVOID result; // eax@7

  Flags = Context->ContextFlags;
  reception = 0;
  if ( Flags & 0x10000 )
  {
    if ( (Flags & 0x10040) == 65600 )
      reception = (BYTE)&Context[1];
    if ( !FeatureId )
    {
      if ( Length )
        *Length = 160;
      return Context->ExtendedRegisters;
    }
    if ( FeatureId == 1 )
    {
      if ( Length )
        *Length = 128;
      return &Context->ExtendedRegisters[160];
    }
    goto LABEL_23;
  }
  if ( !(Flags & 0x100000) )
    return 0;
  if ( (Flags & 0x100040) == 0x100040 )
    reception = (BYTE)&Context[1].ExtendedRegisters[312];
  if ( FeatureId )
  {
    if ( FeatureId != 1 )
    {
LABEL_23:
      if ( reception )
        return (PVOID)RtlLocateExtendedFeature(reception, FeatureId, Length);
      return 0;
    }
    if ( Length )
      *Length = 256;
    result = &Context->ExtendedRegisters[212];
  }
  else
  {
    if ( Length )
      *Length = 160;
    result = &Context->ExtendedRegisters[52];
  }
  return result;
}

BOOL WINAPI SetXStateFeaturesMask(PWOW64_CONTEXT Context, DWORD64 FeatureMask)
{
  DWORD error; // eax@1
  PWOW64_CONTEXT LocalContext; // esi@1
  NTSTATUS status; // [sp-4h] [bp-Ch]@15

  error = Context->ContextFlags;
  LocalContext = 0;
  if ( error & 0x10000 )
  {
    if ( (error & 0x10040) == 65600 )
      LocalContext = Context + 1;
    if ( FeatureMask & 3 )
    {
      Context->ContextFlags = error | 0x10020;
      if ( !LocalContext )
        goto LABEL_14;
      if ( LocalContext->Dr2 < 0x2CC )
        LocalContext->Dr2 = 716;
    }
  }
  else
  {
    if ( !(error & 0x100000) )
    {
      status = 0xC00000BBu;
      goto LABEL_20;
    }
    if ( (error & 0x100040) == 1048640 )
      LocalContext = Context;
    if ( FeatureMask & 3 )
      Context->ContextFlags = error | 0x100008;
  }
  if ( LocalContext )
    goto LABEL_17;
LABEL_14:
  if ( !(FeatureMask | FeatureMask & 0xFFFFFFFCu) )
  {
    if ( !LocalContext )
      return 1;
LABEL_17:
    RtlSetExtendedFeaturesMask(LocalContext, FeatureMask);
    return 1;
  }
  status = 0xC000000Du;
LABEL_20:
  RtlSetLastWin32ErrorAndNtStatusFromNtStatus(status);
  return 0;
}

DWORD64 WINAPI GetEnabledXStateFeatures(void)
{
	return 3;
}

BOOL WINAPI GetXStateFeaturesMask(PWOW64_CONTEXT Context, PDWORD64 FeatureMask)
{
  DWORD contextFlags; // ecx@1
  PDWORD64 longNumber; // qdi@1
  UCHAR *contextLocal; // edx@1
  DWORD flags; // eax@2
  DWORD other; // eax@4
  DWORD compare; // eax@6
  LONG highPart = 0i64; // edx@12

  contextFlags = Context->ContextFlags;
  longNumber = 0i64;
  contextLocal = 0;
  if ( contextFlags & 0x10000 )
  {
    flags = Context->ContextFlags;
    if ( (flags & 0x10040) == 65600 )
      contextLocal = (UCHAR *)&Context[1];
    other = 65568;
  }
  else
  {
    if ( !(contextFlags & 0x100000) )
    {
      RtlSetLastWin32ErrorAndNtStatusFromNtStatus(0xC00000BBu);
      return 0;
    }
    compare = Context->ContextFlags;
    if ( (compare & 0x100040) == 1048640 )
      contextLocal = &Context[1].ExtendedRegisters[312];
    other = 1048584;
  }
  if ( (other & contextFlags) == other )
    longNumber = (PDWORD64)3;
  if ( contextLocal )
  {
    longNumber = RtlGetExtendedFeaturesMask(contextLocal);
    longNumber = (PDWORD64)highPart;
  }
  FeatureMask = longNumber;
  return 1;
}
#endif  