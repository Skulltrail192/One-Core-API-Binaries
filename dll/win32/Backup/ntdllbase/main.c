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
 
extern USHORT NlsAnsiCodePage = 0; /* exported */
extern BOOLEAN NlsMbCodePageTag = FALSE; /* exported */
extern BOOLEAN NlsMbOemCodePageTag = FALSE;

/*Functions ********************************************/

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
	NTSTATUS status;
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
			initTable();
            break;
    }

    return TRUE;
}

BOOLEAN WINAPI RtlGetIntegerAtom(PWSTR AtomName, PUSHORT IntegerAtom)
{
  PWSTR string; // ecx@5
  WCHAR other; // dx@5
  PWSTR recept; // eax@5
  PWSTR unknown; // eax@8
  PUSHORT v7; // ecx@11
  PUNICODE_STRING receiveString; // [sp+0h] [bp-8h]@16
  PUNICODE_STRING stringReceive; // [sp+2h] [bp-6h]@16
  PWSTR obj; // [sp+4h] [bp-4h]@16

  if ( !((unsigned int)AtomName & 0xFFFF0000) )
  {
    unknown = AtomName;
    if ( (unsigned int)AtomName < 0xC000u )
    {
      if ( !AtomName )
        unknown = (PWSTR)0xC000u;
      v7 = IntegerAtom;
      if ( !IntegerAtom )
        return 1;
      goto LABEL_12;
    }
    return 0;
  }
  if ( *AtomName != 35 )
    return 0;
  string = AtomName + 1;
  other = AtomName[1];
  recept = AtomName + 1;
  if ( other )
  {
    while ( other >= 0x30u && other <= 0x39u )
    {
      ++recept;
      other = *recept;
      if ( !*recept )
        goto LABEL_16;
    }
    return 0;
  }
LABEL_16:
  AtomName = NULL;
  receiveString = (PUNICODE_STRING)recept;
  stringReceive = (PUNICODE_STRING)string;
  obj = string;
  if ( RtlUnicodeStringToInteger(receiveString, 10, (PULONG)AtomName) < 0 )
    return 0;
  v7 = IntegerAtom;
  if ( !IntegerAtom )
    return 1;
  unknown = AtomName;
  if ( !AtomName || (unsigned int)AtomName > 0xC000 )
  {
    *IntegerAtom = 0xC000u;
    return 1;
  }
LABEL_12:
  *v7 = (unsigned int)unknown;
  return 1;
}

NTSTATUS NTAPI RtlCreateServiceSid(PUNICODE_STRING ServiceName,	PSID *ServiceSid, PULONG ServiceSidLength) 
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
  PVOID v17 = 0; // [sp+7Ch] [bp-10h]@7
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
      result = 0xC0000023u;
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
        ServiceSid[5] = v17;
        ServiceSid[6] = v18;
        ServiceSid[7] = v19;		
        result = 0;
      }
    }
  }
  else
  {
    result = 0xC000000Du;
  }
  return result;
}	

DWORD WINAPI RtlInternString(int a, int b)
{
	return 0xC000000Du;
}

DWORD WINAPI RtlCloseStringHandle(ULONGLONG a)
{
	return 0xC000000Du;
}

DWORD WINAPI RtlGetStringPointer(int a, int b, int c, int d)
{
	return 0xC000000Du;
}

/*subimplemented*/
DWORD WINAPI RtlReleaseStringPointer(PVOID this, int a, int b)
{
	return 0xC000000Du;
}

static
NTSTATUS
RtlpAddKnownAce(
    PACL Acl,
    ULONG Revision,
    ULONG Flags,
    ACCESS_MASK AccessMask,
    GUID *ObjectTypeGuid  OPTIONAL,
    GUID *InheritedObjectTypeGuid  OPTIONAL,
    PSID Sid,
    UCHAR Type)
{
    PACE Ace;
    PSID SidStart;
    ULONG AceSize, InvalidFlags;
    ULONG AceObjectFlags = 0;

#if DBG
    /* check if RtlpAddKnownAce was called incorrectly */
    if (ObjectTypeGuid != NULL || InheritedObjectTypeGuid != NULL)
    {
        ASSERT(Type == ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE ||
               Type == ACCESS_ALLOWED_OBJECT_ACE_TYPE ||
               Type == ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE ||
               Type == ACCESS_DENIED_OBJECT_ACE_TYPE ||
               Type == SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE ||
               Type == SYSTEM_AUDIT_OBJECT_ACE_TYPE);
    }
    else
    {
        ASSERT(Type != ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE &&
               Type != ACCESS_ALLOWED_OBJECT_ACE_TYPE &&
               Type != ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE &&
               Type != ACCESS_DENIED_OBJECT_ACE_TYPE &&
               Type != SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE &&
               Type != SYSTEM_AUDIT_OBJECT_ACE_TYPE);
    }
#endif

    if (!RtlValidSid(Sid))
    {
        return STATUS_INVALID_SID;
    }

    if (Type == SYSTEM_MANDATORY_LABEL_ACE_TYPE)
    {
        static const SID_IDENTIFIER_AUTHORITY MandatoryLabelAuthority = {SECURITY_MANDATORY_LABEL_AUTHORITY};

        /* The SID's identifier authority must be SECURITY_MANDATORY_LABEL_AUTHORITY! */
        if (RtlCompareMemory(&((PISID)Sid)->IdentifierAuthority,
                             &MandatoryLabelAuthority,
                             sizeof(MandatoryLabelAuthority)) != sizeof(MandatoryLabelAuthority))
        {
            return STATUS_INVALID_PARAMETER;
        }
    }

    if (Acl->AclRevision > MAX_ACL_REVISION ||
        Revision > MAX_ACL_REVISION)
    {
        return STATUS_UNKNOWN_REVISION;
    }

    if (Revision < Acl->AclRevision)
    {
        Revision = Acl->AclRevision;
    }

    /* Validate the flags */
    if (Type == SYSTEM_AUDIT_ACE_TYPE ||
        Type == SYSTEM_AUDIT_OBJECT_ACE_TYPE ||
        Type == SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE)
    {
        InvalidFlags = Flags & ~(VALID_INHERIT_FLAGS |
                               SUCCESSFUL_ACCESS_ACE_FLAG | FAILED_ACCESS_ACE_FLAG);
    }
    else
    {
        InvalidFlags = Flags & ~VALID_INHERIT_FLAGS;
    }

    if (InvalidFlags != 0)
    {
        return STATUS_INVALID_PARAMETER;
    }

    if (!RtlFirstFreeAce(Acl, &Ace))
    {
        return STATUS_INVALID_ACL;
    }
    if (Ace == NULL)
    {
        return STATUS_ALLOTTED_SPACE_EXCEEDED;
    }

    /* Calculate the size of the ACE */
    AceSize = RtlLengthSid(Sid) + sizeof(ACE);
    if (ObjectTypeGuid != NULL)
    {
        AceObjectFlags |= ACE_OBJECT_TYPE_PRESENT;
        AceSize += sizeof(GUID);
    }
    if (InheritedObjectTypeGuid != NULL)
    {
        AceObjectFlags |= ACE_INHERITED_OBJECT_TYPE_PRESENT;
        AceSize += sizeof(GUID);
    }

    if (AceObjectFlags != 0)
    {
        /* Don't forget the ACE object flags
          (corresponds to the Flags field in the *_OBJECT_ACE structures) */
        AceSize += sizeof(ULONG);
    }

    if ((ULONG_PTR)Ace + AceSize >
        (ULONG_PTR)Acl + Acl->AclSize)
    {
        return STATUS_ALLOTTED_SPACE_EXCEEDED;
    }

    /* initialize the header and common fields */
    Ace->Header.AceFlags = (BYTE)Flags;
    Ace->Header.AceType = Type;
    Ace->Header.AceSize = (WORD)AceSize;
    Ace->AccessMask = AccessMask;

    if (AceObjectFlags != 0)
    {
        /* Write the ACE flags to the ACE
           (corresponds to the Flags field in the *_OBJECT_ACE structures) */
        *(PULONG)(Ace + 1) = AceObjectFlags;
        SidStart = (PSID)((ULONG_PTR)(Ace + 1) + sizeof(ULONG));
    }
    else
        SidStart = (PSID)(Ace + 1);

    /* copy the GUIDs */
    if (ObjectTypeGuid != NULL)
    {
        RtlCopyMemory(SidStart, ObjectTypeGuid, sizeof(GUID));
        SidStart = (PSID)((ULONG_PTR)SidStart + sizeof(GUID));
    }
    if (InheritedObjectTypeGuid != NULL)
    {
        RtlCopyMemory(SidStart, InheritedObjectTypeGuid, sizeof(GUID));
        SidStart = (PSID)((ULONG_PTR)SidStart + sizeof(GUID));
    }

    /* copy the SID */
    RtlCopySid(RtlLengthSid(Sid), SidStart, Sid);
    Acl->AceCount++;
    Acl->AclRevision = (BYTE)Revision;

    return STATUS_SUCCESS;
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlAddMandatoryAce(
    IN OUT PACL Acl,
    IN ULONG Revision,
    IN ULONG Flags,
    IN ULONG MandatoryFlags,
    IN UCHAR AceType,
    IN PSID LabelSid)
{
    if (MandatoryFlags & ~SYSTEM_MANDATORY_LABEL_VALID_MASK)
        return STATUS_INVALID_PARAMETER;

    if (AceType != SYSTEM_MANDATORY_LABEL_ACE_TYPE)
        return STATUS_INVALID_PARAMETER;

    return RtlpAddKnownAce(Acl,
                           Revision,
                           Flags,
                           (ACCESS_MASK)MandatoryFlags,
                           NULL,
                           NULL,
                           LabelSid,
                           AceType);
}