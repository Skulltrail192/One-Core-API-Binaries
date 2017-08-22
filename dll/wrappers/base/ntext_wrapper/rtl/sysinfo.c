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
#define NDEBUG 
 
#include <main.h>

BOOLEAN globalVerificationTablet = TRUE;
BOOLEAN globalVerificationMediaCenter = TRUE;
BOOLEAN globalVerificationAppliance = TRUE;

BOOLEAN 
IsVersionInstalled(LSA_UNICODE_STRING *Handle)
{
  BOOLEAN verification; // bl@1
  OBJECT_ATTRIBUTES ObjectAttributes; // [sp+4h] [bp-38h]@1
  BOOLEAN KeyValueInformation; // [sp+1Ch] [bp-20h]@2
  int value = 0; // [sp+28h] [bp-14h]@3
  UNICODE_STRING DestinationString; // [sp+30h] [bp-Ch]@2
  ULONG ResultLength; // [sp+38h] [bp-4h]@2

  ObjectAttributes.ObjectName = Handle;
  verification = 0;
  ObjectAttributes.Length = 24;
  ObjectAttributes.RootDirectory = 0;
  ObjectAttributes.Attributes = 64;
  ObjectAttributes.SecurityDescriptor = 0;
  ObjectAttributes.SecurityQualityOfService = 0;
  if ( NtOpenKey((PHANDLE)&Handle, 0x2000000u, &ObjectAttributes) >= 0 )
  {
    RtlInitUnicodeString(&DestinationString, L"Installed");
    if ( ZwQueryValueKey(
           Handle,
           &DestinationString,
           KeyValuePartialInformation,
           &KeyValueInformation,
           0x14u,
           &ResultLength) >= 0
      && value )
      verification = 1;
    ZwClose(Handle);
  }
  return verification;
}

BOOLEAN 
IsMediaCenterInstalled()
{
  BOOLEAN result; // al@1
  UNICODE_STRING DestinationString; // [sp+0h] [bp-8h]@2

  result = globalVerificationMediaCenter;
  if ( globalVerificationMediaCenter == -1 )
  {
    RtlInitUnicodeString(&DestinationString, L"\\Registry\\Machine\\System\\WPA\\MediaCenter");
    result = IsVersionInstalled(&DestinationString);
    globalVerificationMediaCenter = result;
  }
  return result;
}

BOOLEAN 
IsTabletEdtionInstalled()
{
  BOOLEAN result; // al@1
  UNICODE_STRING DestinationString; // [sp+0h] [bp-8h]@2

  result = globalVerificationTablet;
  if ( globalVerificationTablet == -1 )
  {
    RtlInitUnicodeString(&DestinationString, L"\\Registry\\Machine\\System\\WPA\\TabletPC");
    result = IsVersionInstalled(&DestinationString);
    globalVerificationTablet = result;
  }
  return result;
}

BOOLEAN 
IsApplianceInstalled()
{
  BOOLEAN result; // al@1
  UNICODE_STRING DestinationString; // [sp+0h] [bp-8h]@2

  result = globalVerificationAppliance;
  if ( globalVerificationAppliance == -1 )
  {
    RtlInitUnicodeString(&DestinationString, L"\\Registry\\Machine\\System\\WPA\\ApplianceServer");
    result = IsVersionInstalled(&DestinationString);
    globalVerificationAppliance = result;
  }
  return result;
}

NTSTATUS 
RtlGetOSProductNameStringHelper(
	PUNICODE_STRING Destination, 
	BOOLEAN verification, 
	PCWSTR Source
)
{
  NTSTATUS result; // eax@2

  if ( !verification || (result = RtlAppendUnicodeToString(Destination, Source), result >= 0) )
    result = RtlAppendUnicodeToString(Destination, Source);
  return result;
}

NTSTATUS 
NTAPI
RtlGetOSProductName(
	PUNICODE_STRING Destination, 
	int verification
)
{
  NTSTATUS result; // eax@2
  LSA_UNICODE_STRING *localDestination; // esi@3
  const WCHAR *windowsString; // eax@10
  const WCHAR *edition; // eax@16
  const WCHAR *windowsStringHelper; // [sp-8h] [bp-128h]@8
  struct _OSVERSIONINFOW VersionInformation; // [sp+4h] [bp-11Ch]@4
  int other = 1; // [sp+11Ch] [bp-4h]@10
  BOOLEAN Destinationa; // [sp+128h] [bp+8h]@5

  if ( !verification )
    return STATUS_INVALID_PARAMETER;
  localDestination = Destination;
  if ( Destination->MaximumLength <= 0u )
    return STATUS_BUFFER_TOO_SMALL;
  *Destination->Buffer = 0;
  Destination->Length = 0;
  VersionInformation.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
  result = RtlGetVersion(&VersionInformation);
  if ( result < 0 )
    return result;
  Destinationa = 0;
  if ( verification & 1 )
  {
    result = RtlGetOSProductNameStringHelper(localDestination, 0, L"Microsoft");
    Destinationa = 1;
  }
  if ( verification & 2 )
  {
    windowsStringHelper = L"Windows";
LABEL_13:
    result = RtlGetOSProductNameStringHelper(localDestination, Destinationa, windowsStringHelper);
    Destinationa = 1;
    goto LABEL_14;
  }
  if ( verification & 4 )
  {
    windowsString = L"Longhorn XP";
    if ( other != 1 )
      windowsString = L"Longhorn .Net";
    windowsStringHelper = windowsString;
    goto LABEL_13;
  }
LABEL_14:
  if ( verification & 8 )
  {
    if ( other & 0x40 )
    {
      edition = L"Embedded";
    }
    else
    {
      if ( IsMediaCenterInstalled() )
      {
        edition = L"Media Center Edition";
      }
      else
      {
        if ( IsTabletEdtionInstalled() )
        {
          edition = L"Tablet PC Edition";
        }
        else
        {
          if ( other & 2 )
          {
            edition = L"Home Edition";
          }
          else
          {
            if ( other == 1 )
            {
              edition = L"Professional";
            }
            else
            {
              if ( IsApplianceInstalled() )
              {
                edition = L"Appliance Server";
              }
              else
              {
                if ( other & 4 )
                {
                  edition = L"Web Server";
                }
                else
                {
                  if ( other & 0x21 )
                  {
                    edition = L"Small Business Server";
                  }
                  else
                  {
                    if ( (char)other >= 0 )
                    {
                      if ( other & 2 )
                        edition = L"Enterprise Server";
                      else
                        edition = L"Standard Server";
                    }
                    else
                    {
                      edition = L"Datacenter Server";
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    result = RtlGetOSProductNameStringHelper(localDestination, Destinationa, edition);
    Destinationa = 1;
  }
  if ( verification & 0x10 )
  {
    result = RtlGetOSProductNameStringHelper(localDestination, Destinationa, L"Version 2003");
    Destinationa = 1;
  }
  if ( verification & 0x20 && VersionInformation.szCSDVersion[0] )
  {
    result = RtlGetOSProductNameStringHelper(localDestination, Destinationa, VersionInformation.szCSDVersion);
    Destinationa = 1;
  }
  if ( verification & 0x40 )
    result = RtlGetOSProductNameStringHelper(localDestination, Destinationa, L"Copyright ");
  return result;
}

/***********************************************************************
 *           RtlGetProductInfo    (NTDLL.@)
 *
 * Gives info about the current Windows product type, in a format compatible
 * with the given Windows version
 *
 * Returns TRUE if the input is valid, FALSE otherwise
 */
BOOLEAN 
WINAPI 
RtlGetProductInfo(
	DWORD dwOSMajorVersion, 
	DWORD dwOSMinorVersion, 
	DWORD dwSpMajorVersion,
    DWORD dwSpMinorVersion, 
	PDWORD pdwReturnedProductType
)
{
    RTL_OSVERSIONINFOEXW VersionInformation;
	
	VersionInformation.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
	
	RtlGetVersion((PRTL_OSVERSIONINFOW)&VersionInformation);

    if (!pdwReturnedProductType)
        return FALSE;

    if (VersionInformation.wProductType == VER_NT_WORKSTATION)
	{
		if(VersionInformation.wSuiteMask == VER_SUITE_PERSONAL)
			*pdwReturnedProductType = PRODUCT_HOME_PREMIUM;
		else
			*pdwReturnedProductType = PRODUCT_ULTIMATE;
	}else{
		if(VersionInformation.wSuiteMask == VER_SUITE_BLADE)
			*pdwReturnedProductType = PRODUCT_WEB_SERVER;
		if(VersionInformation.wSuiteMask == VER_SUITE_COMPUTE_SERVER)
			*pdwReturnedProductType = PRODUCT_CLUSTER_SERVER;
		if(VersionInformation.wSuiteMask == VER_SUITE_DATACENTER)
			*pdwReturnedProductType = PRODUCT_DATACENTER_SERVER;
		if(VersionInformation.wSuiteMask == VER_SUITE_ENTERPRISE)
			*pdwReturnedProductType = PRODUCT_ENTERPRISE_SERVER;
		if(VersionInformation.wSuiteMask == VER_SUITE_SMALLBUSINESS)
			*pdwReturnedProductType = PRODUCT_SMALLBUSINESS_SERVER;		
		if(VersionInformation.wSuiteMask == VER_SUITE_SMALLBUSINESS_RESTRICTED)
			*pdwReturnedProductType = PRODUCT_SB_SOLUTION_SERVER;	
		if(VersionInformation.wSuiteMask == VER_SUITE_STORAGE_SERVER)
			*pdwReturnedProductType = PRODUCT_STORAGE_ENTERPRISE_SERVER;		
		if(VersionInformation.wSuiteMask == VER_SUITE_WH_SERVER)
			*pdwReturnedProductType = PRODUCT_HOME_PREMIUM_SERVER;			
	}        

    return TRUE;
}

NTSTATUS 
WINAPI 
NtQueryLicenseValue(
    PUNICODE_STRING Name,
    ULONG *Type,
    PVOID Buffer,
    ULONG Length,
    ULONG *DataLength)
{
    return STATUS_SUCCESS;	
}

void 
WINAPI 
RtlGetNtVersionNumbers(
  LPDWORD major,
  LPDWORD minor,
  LPDWORD build
)
{
	if(major)
		*major = 6;
	if(minor)
		*minor = 0;
	if(build)
		*build = 3790;
}