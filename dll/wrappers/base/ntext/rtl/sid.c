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
Copyright (c) 2015  Microsoft Corporation 
 
Module Name:

    sid.c
 
 */
 
#define NDEBUG

#include <main.h>

ULONG NtdllBaseTag = 0;

#undef SID_IDENTIFIER_AUTHORITY

NTSTATUS 
NTAPI 
RtlCreateServiceSid(PUNICODE_STRING ServiceName,	PSID *ServiceSid, PULONG ServiceSidLength) 
{
  BOOL resp; // cf@3
  NTSTATUS result; // eax@4
  PVOID v5; // eax@7
  PUNICODE_STRING v6 = NULL; // [sp+8h] [bp-84h]@6
  PVOID Src = NULL; // [sp+Ch] [bp-80h]@7
  PSHA_CTX context = NULL; // [sp+10h] [bp-7Ch]@7
  PSID_IDENTIFIER_AUTHORITY identifier; // [sp+6Ch] [bp-20h]@1
  char v10; // [sp+6Dh] [bp-1Fh]@1
  char v11; // [sp+6Eh] [bp-1Eh]@1
  char v12; // [sp+6Fh] [bp-1Dh]@1
  char v13; // [sp+70h] [bp-1Ch]@1
  char v14; // [sp+71h] [bp-1Bh]@1
  PULONG third = NULL; // [sp+74h] [bp-18h]@7
  PVOID v16 = 0; // [sp+78h] [bp-14h]@7
  PVOID LangIDTemp = 0; // [sp+7Ch] [bp-10h]@7
  PVOID v18 = 0; // [sp+80h] [bp-Ch]@7
  PVOID v19 = 0; // [sp+84h] [bp-8h]@7

  identifier = 0;
  v10 = 0;
  v11 = 0;
  v12 = 0;
  v13 = 0;
  v14 = 5;
  if ( ServiceName && ServiceSidLength )
  {
    resp = *(DWORD *)ServiceSidLength < 0x20u;
    *(DWORD *)ServiceSidLength = 32;
    if ( resp )
    {
      result = STATUS_BUFFER_TOO_SMALL;
    }
    else
    {
      result = RtlUpcaseUnicodeString(v6, ServiceName, 1);
      if ( result >= 0 )
      {
        A_SHAInit(context);
        A_SHAUpdate(context, Src, (UINT)v6);
        A_SHAFinal(context, third);
        RtlFreeAnsiString((PANSI_STRING)v6);
        RtlInitializeSid(ServiceSid, identifier, 6);
        v5 = (PVOID)third;   
        ServiceSid[2] = (PSID)80;
        ServiceSid[3] = v5;
        ServiceSid[4] = v16;
        ServiceSid[5] = LangIDTemp;
        ServiceSid[6] = v18;
        ServiceSid[7] = v19;		
        result = 0;
      }
    }
  }
  else
  {
    result = STATUS_INVALID_PARAMETER;
  }
  return result;
}	

BOOLEAN 
RtlIsCapabilitySid(PSID CapabilitySidToCheck)
{
  BOOLEAN Verification; // cf@1
  BOOLEAN result; // al@4
  int Compare; // [sp+4h] [bp-Ch]@1

  Verification = (DWORD)CapabilitySidToCheck + 1 < 2u;
  Compare = 0;
  if ( Verification
    || (DWORD)CapabilitySidToCheck != 1
    || RtlCompareMemory((char *)CapabilitySidToCheck + 2, &Compare, 6u) != 6 )
    result = FALSE;
  else
    result = (DWORD)CapabilitySidToCheck == 3;
  return result;
}

#undef RtlCreateSecurityDescriptorRelative

NTSTATUS
RtlpCreateSecurityDescriptorRelative (
    IN PISECURITY_DESCRIPTOR_RELATIVE SecurityDescriptor,
    IN ULONG Revision
    )

/*++

Routine Description:

    This procedure initializes a new "relative format" security descriptor.
    After the procedure call the security descriptor is initialized with no
    system ACL, no discretionary ACL, no owner, no primary group and
    all control flags set to false (null).

Arguments:


    SecurityDescriptor - Supplies the security descriptor to
        initialize.

    Revision - Provides the revision level to assign to the security
        descriptor.  This should be one (1) for this release.

Return Value:

    STATUS_SUCCESS - Indicates the call completed successfully.

    STATUS_UNKNOWN_REVISION - Indicates the revision level provided
        is not supported by this routine.

Note:
    Warning, this code assume the caller allocated a relative security
    descriptor rather than a relative one.  Absolute is larger on systems
    with 64-bit pointers.

--*/

{
    //
    // Check the requested revision
    //

    if (Revision == SECURITY_DESCRIPTOR_REVISION) {

        //
        // Typecast to the opaque SECURITY_DESCRIPTOR structure.
        //

        RtlZeroMemory( SecurityDescriptor, sizeof(SECURITY_DESCRIPTOR_RELATIVE));

        SecurityDescriptor->Revision = SECURITY_DESCRIPTOR_REVISION;

        return STATUS_SUCCESS;
    }

    return STATUS_UNKNOWN_REVISION;
}

NTSTATUS 
RtlCheckTokenCapability(
	HANDLE TokenHandle, 
	PSID CapabilitySidToCheck, 
	PBOOL HasCapability
)
{
  PISECURITY_DESCRIPTOR_RELATIVE SecurityDescriptor; // edi@1 MAPDST
  NTSTATUS Status; // esi@4
  int SidSize; // ecx@7
  int Size; // esi@7
  PVOID CurrentHeap; // ST14_4@7
  struct _SECURITY_DESCRIPTOR_RELATIVE *DescriptorRelative; // eax@7
  OBJECT_ATTRIBUTES ObjectAttributes; // [sp+Ch] [bp-98h]@28
  ACCESS_MASK GrantedAccess; // [sp+28h] [bp-7Ch]@15
  ULONG SizeACL; // [sp+30h] [bp-74h]@7
  ULONG PrivilegeSetLength; // [sp+34h] [bp-70h]@15
  NTSTATUS AccessStatus; // [sp+38h] [bp-6Ch]@15
  ULONG ReturnLength; // [sp+44h] [bp-60h]@4
  HANDLE NewTokenHandle; // [sp+48h] [bp-5Ch]@1
  GENERIC_MAPPING GenericMapping; // [sp+4Ch] [bp-58h]@1
  ULONG SecurityQualityOfService; // [sp+5Ch] [bp-48h]@28
  PRIVILEGE_SET PrivilegeSet; // [sp+68h] [bp-3Ch]@15

  NewTokenHandle = NULL;
  SecurityDescriptor = NULL;
  GenericMapping.GenericRead = 0x20001;
  GenericMapping.GenericWrite = 0x30000;
  GenericMapping.GenericExecute = 0x20000;
  GenericMapping.GenericAll = 0x1F0001;
  *HasCapability = FALSE;
  if ( !RtlIsCapabilitySid(CapabilitySidToCheck) )
  {
    Status = STATUS_INVALID_PARAMETER;
    goto Finalizing;
  }
  if ( TokenHandle )
  {
    NewTokenHandle = TokenHandle;
Working:
    ReturnLength = 0;
    Status = ZwQueryInformationToken(TokenHandle, TokenUser, 0, 0, &ReturnLength);
    if ( Status != STATUS_BUFFER_TOO_SMALL )
      goto Finalizing;
    SecurityDescriptor = (PISECURITY_DESCRIPTOR_RELATIVE)RtlAllocateHeap(
                                                           NtCurrentPeb()->ProcessHeap,
                                                           NtdllBaseTag + 1310720,
                                                           ReturnLength);
    if ( !SecurityDescriptor )
      goto Insuficiente_resources;
    Status = ZwQueryInformationToken(NewTokenHandle, TokenUser, SecurityDescriptor, ReturnLength, &ReturnLength);
    if ( Status < 0 )
      goto Finalizing;
    SidSize = (DWORD)CapabilitySidToCheck + 1 + 3 * (DWORD)&SecurityDescriptor->Revision + 1;
    Size = 4 * SidSize + 76;
    CurrentHeap = NtCurrentPeb()->ProcessHeap;
    SizeACL = 4 * SidSize + 76;
    DescriptorRelative = (struct _SECURITY_DESCRIPTOR_RELATIVE *)RtlAllocateHeap(
                                                                   CurrentHeap,
                                                                   NtdllBaseTag + 1310720,
                                                                   Size);
    SecurityDescriptor = DescriptorRelative;
    if ( DescriptorRelative )
    {
      memset(DescriptorRelative, 0, Size);
      Status = RtlpCreateSecurityDescriptorRelative(SecurityDescriptor, 1);
      if ( Status >= 0 )
      {
        Status = RtlSetOwnerSecurityDescriptor(SecurityDescriptor, *(PSID *)&SecurityDescriptor->Revision, 0);
        if ( Status >= 0 )
        {
          Status = RtlSetGroupSecurityDescriptor(SecurityDescriptor, *(PSID *)&SecurityDescriptor->Revision, 0);
          if ( Status >= 0 )
          {
            Status = RtlCreateAcl((PACL)&SecurityDescriptor[1], SizeACL - 20, 2u);
            if ( Status >= 0 )
            {
              Status = RtlAddAccessAllowedAce(
                         (PACL)&SecurityDescriptor[1],
                         2u,
                         0x10001u,
                         *(PSID *)&SecurityDescriptor->Revision);
              if ( Status >= 0 )
              {
                Status = RtlAddAccessAllowedAce((PACL)&SecurityDescriptor[1], 2u, 0x10001u, CapabilitySidToCheck);
                if ( Status >= 0 )
                {
                  Status = RtlSetDaclSecurityDescriptor(
                             (PSECURITY_DESCRIPTOR)SecurityDescriptor,
                             1u,
                             (PACL)&SecurityDescriptor[1],
                             0);
                  if ( Status >= 0 )
                  {
                    PrivilegeSetLength = 56;
                    Status = NtAccessCheck(
                               (PSECURITY_DESCRIPTOR)SecurityDescriptor,
                               NewTokenHandle,
                               0x10001u,
                               &GenericMapping,
                               &PrivilegeSet,
                               &PrivilegeSetLength,
                               &GrantedAccess,
                               &AccessStatus);
                    if ( Status >= 0 )
                    {
                      if ( !AccessStatus && GrantedAccess == 65537 )
                        *HasCapability = TRUE;
                      Status = STATUS_SUCCESS;
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    else
    {
Insuficiente_resources:
      Status = STATUS_INSUFFICIENT_RESOURCES;
    }
Finalizing:
    if ( TokenHandle )
      goto Free_memory;
    goto Close_token;
  }
  Status = NtOpenThreadTokenEx((HANDLE)-2, 8, 1, 0, &NewTokenHandle);
  if ( Status == STATUS_NO_TOKEN )
  {
    Status = ZwOpenProcessTokenEx((HANDLE)-1, 10, 0, &TokenHandle);
    if ( Status < STATUS_SUCCESS )
      goto Close_token;
    ObjectAttributes.SecurityQualityOfService = &SecurityQualityOfService;
    ObjectAttributes.Length = 24;
    ObjectAttributes.RootDirectory = 0;
    ObjectAttributes.Attributes = 0;
    ObjectAttributes.ObjectName = 0;
    ObjectAttributes.SecurityDescriptor = 0;
    SecurityQualityOfService = 12;
    Status = NtDuplicateToken(TokenHandle, 8u, &ObjectAttributes, 0, TokenImpersonation, &NewTokenHandle);
    NtClose(TokenHandle);
  }
  if ( Status >= STATUS_SUCCESS )
  {
    TokenHandle = NewTokenHandle;
    goto Working;
  }
Close_token:
  if ( NewTokenHandle )
    NtClose(NewTokenHandle);
Free_memory:
  if ( SecurityDescriptor )
    RtlFreeHeap(NtCurrentPeb()->ProcessHeap, 0, SecurityDescriptor);
  return Status;
}

NTSTATUS 
NTAPI 
RtlCheckTokenMembershipEx(
	HANDLE TokenHandle, 
	PSID SidToCheck, 
	DWORD Flas, 
	PBOOL IsMember
)
{
	DbgPrint("UNIMPLEMENTED: RtlCheckTokenMembershipEx");
	return STATUS_SUCCESS;
}	