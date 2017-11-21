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

    acl.c
 
 */
 
#define NDEBUG

#include <main.h> 

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
NTSYSAPI 
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

NTSTATUS 
NTAPI 
RtlAddResourceAttributeAce(
	PACL pAcl, 
	DWORD dwAceRevision, 
	DWORD AceFlags, 
	DWORD AccessMask, 
	PSID pSid, 
	PCLAIM_SECURITY_ATTRIBUTES_INFORMATION  pAttributeInfo, 
	PDWORD pReturnLength
)
{
	DbgPrint("UNIMPLEMENTED: RtlAddResourceAttributeAce");
	return STATUS_SUCCESS;
}