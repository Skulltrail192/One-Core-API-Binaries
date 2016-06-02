/* COPYRIGHT:       See COPYING in the top level directory
 * PROJECT:         ReactOS system libraries
 * PURPOSE:         Security manager
 * FILE:            lib/rtl/acl.c
 * PROGRAMER:       David Welch <welch@cwcom.net>
 */

/* INCLUDES *****************************************************************/

#include <rtl.h>
#include <rtlfuncs.h>
#include <kefuncs.h>
#include <../../ntoskrnl/include/internal/se.h>
#define NDEBUG
#include <debug.h>

#define SeLengthSid( Sid ) \
     (8 + (4 * ((SID *)Sid)->SubAuthorityCount))

//
//  Define the local macros and procedure for this module
//

//
//  Return a pointer to the first Ace in an Acl (even if the Acl is empty).
//
//      PACE_HEADER
//      FirstAce (
//          IN PACL Acl
//          );
//

#define FirstAce(Acl) ((PVOID)((PUCHAR)(Acl) + sizeof(ACL)))

//
//  Return a pointer to the next Ace in a sequence (even if the input
//  Ace is the one in the sequence).
//
//      PACE_HEADER
//      NextAce (
//          IN PACE_HEADER Ace
//          );
//

#define NextAce(Ace) ((PVOID)((PUCHAR)(Ace) + ((PACE_HEADER)(Ace))->AceSize))

#define LongAligned( ptr )  (LongAlign(ptr) == ((PVOID)(ptr)))
#define WordAligned( ptr )  (WordAlign(ptr) == ((PVOID)(ptr)))

/* PRIVATE FUNCTIONS **********************************************************/

BOOLEAN
NTAPI
RtlFirstFreeAce(IN PACL Acl,
                OUT PACE* FirstFreeAce)
{
    PACE Current;
    ULONG_PTR AclEnd;
    ULONG i;
    PAGED_CODE_RTL();

    /* Assume failure */
    *FirstFreeAce = NULL;

    /* Get the start and end pointers */
    Current = (PACE)(Acl + 1);
    AclEnd = (ULONG_PTR)Acl + Acl->AclSize;

    /* Loop all the ACEs */
    for (i = 0; i < Acl->AceCount; i++)
    {
        /* If any is beyond the DACL, bail out, otherwise keep going */
        if ((ULONG_PTR)Current >= AclEnd) return FALSE;
        Current = (PACE)((ULONG_PTR)Current + Current->Header.AceSize);
    }

    /* If the last spot is empty and still valid, return it */
    if ((ULONG_PTR)Current < AclEnd) *FirstFreeAce = Current;
    return TRUE;
}

VOID
NTAPI
RtlpAddData(IN PVOID AceList,
            IN ULONG AceListLength,
            IN PVOID Ace,
            IN ULONG Offset)
{
    /* Shift the buffer down */
    if (Offset > 0)
    {
        RtlCopyMemory((PVOID)((ULONG_PTR)Ace + AceListLength),
                      Ace,
                      Offset);
    }

    /* Copy the new data in */
    if (AceListLength) RtlCopyMemory(Ace, AceList, AceListLength);
}

VOID
NTAPI
RtlpDeleteData(IN PVOID Ace,
               IN ULONG AceSize,
               IN ULONG Offset)
{
    /* Move the data up */
    if (AceSize < Offset)
    {
        RtlMoveMemory(Ace,
                      (PVOID)((ULONG_PTR)Ace + AceSize),
                      Offset - AceSize);
    }

    /* Zero the rest */
    if ((Offset - AceSize) < Offset)
    {
        RtlZeroMemory((PVOID)((ULONG_PTR)Ace + Offset - AceSize), AceSize);
    }
}

NTSTATUS
NTAPI
RtlpAddKnownAce(IN PACL Acl,
                IN ULONG Revision,
                IN ULONG Flags,
                IN ACCESS_MASK AccessMask,
                IN PSID Sid,
                IN UCHAR Type)
{
    PKNOWN_ACE Ace;
    ULONG AceSize, InvalidFlags;
    PAGED_CODE_RTL();

    /* Check the validity of the SID */
    if (!RtlValidSid(Sid)) return STATUS_INVALID_SID;

    /* Check the validity of the revision */
    if ((Acl->AclRevision > ACL_REVISION4) || (Revision > ACL_REVISION4))
    {
        return STATUS_REVISION_MISMATCH;
    }

    /* Pick the smallest of the revisions */
    if (Revision < Acl->AclRevision) Revision = Acl->AclRevision;

    /* Validate the flags */
    if (Type == SYSTEM_AUDIT_ACE_TYPE)
    {
        InvalidFlags = Flags & ~(VALID_INHERIT_FLAGS |
                                 SUCCESSFUL_ACCESS_ACE_FLAG |
                                 FAILED_ACCESS_ACE_FLAG);
    }
    else
    {
        InvalidFlags = Flags & ~VALID_INHERIT_FLAGS;
    }

    /* If flags are invalid, bail out */
    if (InvalidFlags != 0) return STATUS_INVALID_PARAMETER;

    /* If ACL is invalid, bail out */
    if (!RtlValidAcl(Acl)) return STATUS_INVALID_ACL;

    /* If there's no free ACE, bail out */
    if (!RtlFirstFreeAce(Acl, (PACE*)&Ace)) return STATUS_INVALID_ACL;

    /* Calculate the size of the ACE and bail out if it's too small */
    AceSize = RtlLengthSid(Sid) + sizeof(ACE);
    if (!(Ace) || ((ULONG_PTR)Ace + AceSize > (ULONG_PTR)Acl + Acl->AclSize))
    {
        return STATUS_ALLOTTED_SPACE_EXCEEDED;
    }

    /* Initialize the header and common fields */
    Ace->Header.AceFlags = (BYTE)Flags;
    Ace->Header.AceType = Type;
    Ace->Header.AceSize = (WORD)AceSize;
    Ace->Mask = AccessMask;

    /* Copy the SID */
    RtlCopySid(RtlLengthSid(Sid), &Ace->SidStart, Sid);

    /* Fill out the ACL header and return */
    Acl->AceCount++;
    Acl->AclRevision = (BYTE)Revision;
    return STATUS_SUCCESS;
}

NTSTATUS
NTAPI
RtlpAddKnownObjectAce(IN PACL Acl,
                      IN ULONG Revision,
                      IN ULONG Flags,
                      IN ACCESS_MASK AccessMask,
                      IN GUID *ObjectTypeGuid OPTIONAL,
                      IN GUID *InheritedObjectTypeGuid OPTIONAL,
                      IN PSID Sid,
                      IN UCHAR Type)
{
    PKNOWN_OBJECT_ACE Ace;
    ULONG_PTR SidStart;
    ULONG AceSize, InvalidFlags, AceObjectFlags = 0;
    PAGED_CODE_RTL();

    /* Check the validity of the SID */
    if (!RtlValidSid(Sid)) return STATUS_INVALID_SID;

    /* Check the validity of the revision */
    if ((Acl->AclRevision > ACL_REVISION4) || (Revision > ACL_REVISION4))
    {
        return STATUS_REVISION_MISMATCH;
    }

    /* Pick the smallest of the revisions */
    if (Revision < Acl->AclRevision) Revision = Acl->AclRevision;

    /* Validate the flags */
    if ((Type == SYSTEM_AUDIT_OBJECT_ACE_TYPE) ||
        (Type == SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE))
    {
        InvalidFlags = Flags & ~(VALID_INHERIT_FLAGS |
                               SUCCESSFUL_ACCESS_ACE_FLAG | FAILED_ACCESS_ACE_FLAG);
    }
    else
    {
        InvalidFlags = Flags & ~VALID_INHERIT_FLAGS;
    }

    /* If flags are invalid, bail out */
    if (InvalidFlags != 0) return STATUS_INVALID_PARAMETER;

    /* If ACL is invalid, bail out */
    if (!RtlValidAcl(Acl)) return STATUS_INVALID_ACL;

    /* If there's no free ACE, bail out */
    if (!RtlFirstFreeAce(Acl, (PACE*)&Ace)) return STATUS_INVALID_ACL;

    /* Calculate the size of the ACE */
    AceSize = RtlLengthSid(Sid) + sizeof(ACE) + sizeof(ULONG);

    /* Add-in the size of the GUIDs if any and update flags as needed */
    if (ObjectTypeGuid)
    {
        AceObjectFlags |= ACE_OBJECT_TYPE_PRESENT;
        AceSize += sizeof(GUID);
    }
    if (InheritedObjectTypeGuid)
    {
        AceObjectFlags |= ACE_INHERITED_OBJECT_TYPE_PRESENT;
        AceSize += sizeof(GUID);
    }

    /* Bail out if there's not enough space in the ACL */
    if (!(Ace) || ((ULONG_PTR)Ace + AceSize > (ULONG_PTR)Acl + Acl->AclSize))
    {
        return STATUS_ALLOTTED_SPACE_EXCEEDED;
    }

    /* Initialize the header and common fields */
    Ace->Header.AceFlags = (BYTE)Flags;
    Ace->Header.AceType = Type;
    Ace->Header.AceSize = (WORD)AceSize;
    Ace->Mask = AccessMask;
    Ace->Flags = AceObjectFlags;

    /* Copy the GUIDs */
    SidStart = (ULONG_PTR)&Ace->SidStart;
    if (ObjectTypeGuid )
    {
        RtlCopyMemory((PVOID)SidStart, ObjectTypeGuid, sizeof(GUID));
        SidStart += sizeof(GUID);
    }
    if (InheritedObjectTypeGuid)
    {
        RtlCopyMemory((PVOID)SidStart, InheritedObjectTypeGuid, sizeof(GUID));
        SidStart += sizeof(GUID);
    }

    /* Copy the SID */
    RtlCopySid(RtlLengthSid(Sid), (PSID)SidStart, Sid);

    /* Fill out the ACL header and return */
    Acl->AceCount++;
    Acl->AclRevision = (BYTE)Revision;
    return STATUS_SUCCESS;
}

/* PUBLIC FUNCTIONS ***********************************************************/

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlAddAccessAllowedAce(IN OUT PACL Acl,
                       IN ULONG Revision,
                       IN ACCESS_MASK AccessMask,
                       IN PSID Sid)
{
    PAGED_CODE_RTL();

    /* Call the worker function */
    return RtlpAddKnownAce(Acl,
                           Revision,
                           0,
                           AccessMask,
                           Sid,
                           ACCESS_ALLOWED_ACE_TYPE);
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlAddAccessAllowedAceEx(IN OUT PACL Acl,
                         IN ULONG Revision,
                         IN ULONG Flags,
                         IN ACCESS_MASK AccessMask,
                         IN PSID Sid)
{
    PAGED_CODE_RTL();

    /* Call the worker function */
    return RtlpAddKnownAce(Acl,
                           Revision,
                           Flags,
                           AccessMask,
                           Sid,
                           ACCESS_ALLOWED_ACE_TYPE);
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlAddAccessAllowedObjectAce(IN OUT PACL Acl,
                             IN ULONG Revision,
                             IN ULONG Flags,
                             IN ACCESS_MASK AccessMask,
                             IN GUID *ObjectTypeGuid  OPTIONAL,
                             IN GUID *InheritedObjectTypeGuid  OPTIONAL,
                             IN PSID Sid)
{
    PAGED_CODE_RTL();

    /* Is there no object data? */
    if (!(ObjectTypeGuid) && !(InheritedObjectTypeGuid))
    {
        /* Use the usual routine */
        return RtlpAddKnownAce(Acl,
                               Revision,
                               Flags,
                               AccessMask,
                               Sid,
                               ACCESS_ALLOWED_ACE_TYPE);
    }

    /* Use the object routine */
    return RtlpAddKnownObjectAce(Acl,
                                 Revision,
                                 Flags,
                                 AccessMask,
                                 ObjectTypeGuid,
                                 InheritedObjectTypeGuid,
                                 Sid,
                                 ACCESS_ALLOWED_OBJECT_ACE_TYPE);
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlAddAccessDeniedAce(IN PACL Acl,
                      IN ULONG Revision,
                      IN ACCESS_MASK AccessMask,
                      IN PSID Sid)
{
    PAGED_CODE_RTL();

    /* Call the worker function */
    return RtlpAddKnownAce(Acl,
                           Revision,
                           0,
                           AccessMask,
                           Sid,
                           ACCESS_DENIED_ACE_TYPE);
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlAddAccessDeniedAceEx(IN OUT PACL Acl,
                        IN ULONG Revision,
                        IN ULONG Flags,
                        IN ACCESS_MASK AccessMask,
                        IN PSID Sid)
{
    PAGED_CODE_RTL();

    /* Call the worker function */
    return RtlpAddKnownAce(Acl,
                           Revision,
                           Flags,
                           AccessMask,
                           Sid,
                           ACCESS_DENIED_ACE_TYPE);
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlAddAccessDeniedObjectAce(IN OUT PACL Acl,
                            IN ULONG Revision,
                            IN ULONG Flags,
                            IN ACCESS_MASK AccessMask,
                            IN GUID *ObjectTypeGuid OPTIONAL,
                            IN GUID *InheritedObjectTypeGuid OPTIONAL,
                            IN PSID Sid)
{
    PAGED_CODE_RTL();

    /* Is there no object data? */
    if (!(ObjectTypeGuid) && !(InheritedObjectTypeGuid))
    {
        /* Use the usual routine */
        return RtlpAddKnownAce(Acl,
                               Revision,
                               Flags,
                               AccessMask,
                               Sid,
                               ACCESS_DENIED_ACE_TYPE);
    }

    /* There's object data, use the object routine */
    return RtlpAddKnownObjectAce(Acl,
                                 Revision,
                                 Flags,
                                 AccessMask,
                                 ObjectTypeGuid,
                                 InheritedObjectTypeGuid,
                                 Sid,
                                 ACCESS_DENIED_OBJECT_ACE_TYPE);
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlAddAuditAccessAce(IN PACL Acl,
                     IN ULONG Revision,
                     IN ACCESS_MASK AccessMask,
                     IN PSID Sid,
                     IN BOOLEAN Success,
                     IN BOOLEAN Failure)
{
    ULONG Flags = 0;
    PAGED_CODE_RTL();

    /* Add flags */
    if (Success) Flags |= SUCCESSFUL_ACCESS_ACE_FLAG;
    if (Failure) Flags |= FAILED_ACCESS_ACE_FLAG;

    /* Call the worker routine */
    return RtlpAddKnownAce(Acl,
                           Revision,
                           Flags,
                           AccessMask,
                           Sid,
                           SYSTEM_AUDIT_ACE_TYPE);
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlAddAuditAccessAceEx(IN PACL Acl,
                       IN ULONG Revision,
                       IN ULONG Flags,
                       IN ACCESS_MASK AccessMask,
                       IN PSID Sid,
                       IN BOOLEAN Success,
                       IN BOOLEAN Failure)
{
    PAGED_CODE_RTL();

    /* Add flags */
    if (Success) Flags |= SUCCESSFUL_ACCESS_ACE_FLAG;
    if (Failure) Flags |= FAILED_ACCESS_ACE_FLAG;

    /* Call the worker routine */
    return RtlpAddKnownAce(Acl,
                           Revision,
                           Flags,
                           AccessMask,
                           Sid,
                           SYSTEM_AUDIT_ACE_TYPE);
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlAddAuditAccessObjectAce(IN PACL Acl,
                           IN ULONG Revision,
                           IN ULONG Flags,
                           IN ACCESS_MASK AccessMask,
                           IN GUID *ObjectTypeGuid  OPTIONAL,
                           IN GUID *InheritedObjectTypeGuid  OPTIONAL,
                           IN PSID Sid,
                           IN BOOLEAN Success,
                           IN BOOLEAN Failure)
{
    /* Add flags */
    if (Success) Flags |= SUCCESSFUL_ACCESS_ACE_FLAG;
    if (Failure) Flags |= FAILED_ACCESS_ACE_FLAG;

    /* Is there no object data? */
    if (!(ObjectTypeGuid) && !(InheritedObjectTypeGuid))
    {
        /* Call the normal routine */
        return RtlpAddKnownAce(Acl,
                               Revision,
                               Flags,
                               AccessMask,
                               Sid,
                               SYSTEM_AUDIT_ACE_TYPE);
    }

    /* There's object data, use the object routine */
    return RtlpAddKnownObjectAce(Acl,
                                 Revision,
                                 Flags,
                                 AccessMask,
                                 ObjectTypeGuid,
                                 InheritedObjectTypeGuid,
                                 Sid,
                                 SYSTEM_AUDIT_OBJECT_ACE_TYPE);
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlGetAce(IN PACL Acl,
          IN ULONG AceIndex,
          OUT PVOID *Ace)
{
    ULONG i;
    PAGED_CODE_RTL();

    /* Bail out if the revision or the index are invalid */
    if ((Acl->AclRevision < MIN_ACL_REVISION) ||
        (Acl->AclRevision > MAX_ACL_REVISION) ||
        (AceIndex >= Acl->AceCount))
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* Loop through the ACEs */
    *Ace = (PVOID)((PACE)(Acl + 1));
    for (i = 0; i < AceIndex; i++)
    {
        /* Bail out if an invalid ACE is ever found */
        if ((ULONG_PTR)*Ace >= (ULONG_PTR)Acl + Acl->AclSize)
        {
            return STATUS_INVALID_PARAMETER;
        }

        /* Keep going */
        *Ace = (PVOID)((PACE)((ULONG_PTR)(*Ace) + ((PACE)(*Ace))->Header.AceSize));
    }

    /* Check if the last ACE is still valid */
    if ((ULONG_PTR)*Ace >= (ULONG_PTR)Acl + Acl->AclSize)
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* All good, return */
    return STATUS_SUCCESS;
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlAddAce(IN PACL Acl,
          IN ULONG AclRevision,
          IN ULONG StartingIndex,
          IN PVOID AceList,
          IN ULONG AceListLength)
{
    PACE Ace, FreeAce;
    USHORT NewAceCount;
    ULONG Index;
    PAGED_CODE_RTL();

    /* Bail out if the ACL is invalid */
    if (!RtlValidAcl(Acl)) return STATUS_INVALID_PARAMETER;

    /* Bail out if there's no space */
    if (!RtlFirstFreeAce(Acl, &FreeAce)) return STATUS_INVALID_PARAMETER;

    /* Loop over all the ACEs, keeping track of new ACEs as we go along */
    for (Ace = AceList, NewAceCount = 0;
         Ace < (PACE)((ULONG_PTR)AceList + AceListLength);
         NewAceCount++)
    {
        /* Make sure that the revision of this ACE is valid in this list.
           The initial check looks strange, but it is what Windows does. */
        if (Ace->Header.AceType <= ACCESS_MAX_MS_ACE_TYPE)
        {
            if (Ace->Header.AceType > ACCESS_MAX_MS_V3_ACE_TYPE)
            {
                if (AclRevision < ACL_REVISION4) return STATUS_INVALID_PARAMETER;
            }
            else if (Ace->Header.AceType > ACCESS_MAX_MS_V2_ACE_TYPE)
            {
                if (AclRevision < ACL_REVISION3) return STATUS_INVALID_PARAMETER;
            }
        }

        /* Move to the next ACE */
        Ace = (PACE)((ULONG_PTR)Ace + Ace->Header.AceSize);
    }

    /* Bail out if there's no more space for us */
    if ((ULONG_PTR)Ace > ((ULONG_PTR)AceList + AceListLength))
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* Bail out if there's no free ACE spot, or if we would overflow it */
    if (!(FreeAce) ||
        ((ULONG_PTR)FreeAce + AceListLength > (ULONG_PTR)Acl + Acl->AclSize))
    {
        return STATUS_BUFFER_TOO_SMALL;
    }

    /* Go down the list until we find our index */
    Ace = (PACE)(Acl + 1);
    for (Index = 0; (Index < StartingIndex) && (Index < Acl->AceCount); Index++)
    {
        Ace = (PACE)((ULONG_PTR)Ace + Ace->Header.AceSize);
    }

    /* Found where we want to do, add us to the list */
    RtlpAddData(AceList,
                AceListLength,
                Ace,
                (ULONG_PTR)FreeAce - (ULONG_PTR)Ace);

    /* Update the header and return */
    Acl->AceCount += NewAceCount;
    Acl->AclRevision = (UCHAR)min(Acl->AclRevision, AclRevision);
    return STATUS_SUCCESS;
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlDeleteAce(IN PACL Acl,
             IN ULONG AceIndex)
{
    PACE FreeAce, Ace;
    PAGED_CODE_RTL();

    /* Bail out if the ACL is invalid */
    if (!RtlValidAcl(Acl)) return STATUS_INVALID_PARAMETER;

    /* Bail out if there's no space or if we're full */
    if ((Acl->AceCount <= AceIndex) || !(RtlFirstFreeAce(Acl, &FreeAce)))
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* Enumerate until the indexed ACE is reached */
    Ace = (PACE)(Acl + 1);
    while (AceIndex--) Ace = (PACE)((ULONG_PTR)Ace + Ace->Header.AceSize);

    /* Delete this ACE */
    RtlpDeleteData(Ace,
                   Ace->Header.AceSize,
                   (ULONG)((ULONG_PTR)FreeAce - (ULONG_PTR)Ace));

    /* Decrease an ACE and return success */
    Acl->AceCount--;
    return STATUS_SUCCESS;
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlCreateAcl(IN PACL Acl,
             IN ULONG AclSize,
             IN ULONG AclRevision)
{
    PAGED_CODE_RTL();

    /* Bail out if too small */
    if (AclSize < sizeof(ACL)) return STATUS_BUFFER_TOO_SMALL;

    /* Bail out if too large or invalid revision */
    if ((AclRevision < MIN_ACL_REVISION) ||
        (AclRevision > MAX_ACL_REVISION) ||
        (AclSize > MAXUSHORT))
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* Setup the header */
    Acl->AclSize = (USHORT)ROUND_UP(AclSize, 4);
    Acl->AclRevision = (UCHAR)AclRevision;
    Acl->AceCount = 0;
    Acl->Sbz1 = 0;
    Acl->Sbz2 = 0;
    return STATUS_SUCCESS;
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlQueryInformationAcl(IN PACL Acl,
                       IN PVOID Information,
                       IN ULONG InformationLength,
                       IN ACL_INFORMATION_CLASS InformationClass)
{
    PACE Ace;
    PACL_REVISION_INFORMATION RevisionInfo;
    PACL_SIZE_INFORMATION SizeInfo;
    PAGED_CODE_RTL();

    /* Validate the ACL revision */
    if ((Acl->AclRevision < MIN_ACL_REVISION) ||
        (Acl->AclRevision > MAX_ACL_REVISION))
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* Check what the caller is querying */
    switch (InformationClass)
    {
        /* Revision data */
        case AclRevisionInformation:

            /* Bail out if the buffer is too small */
            if (InformationLength < sizeof(ACL_REVISION_INFORMATION))
            {
               return STATUS_BUFFER_TOO_SMALL;
            }

            /* Return the current revision */
            RevisionInfo = (PACL_REVISION_INFORMATION)Information;
            RevisionInfo->AclRevision = Acl->AclRevision;
            break;

        /* Size data */
        case AclSizeInformation:

            /* Bail out if the buffer is too small */
            if (InformationLength < sizeof(ACL_SIZE_INFORMATION))
            {
               return STATUS_BUFFER_TOO_SMALL;
            }

            /* Bail out if there's no space in the ACL */
            if (!RtlFirstFreeAce(Acl, &Ace)) return STATUS_INVALID_PARAMETER;

            /* Read the number of ACEs and check if there was a free ACE */
            SizeInfo = (PACL_SIZE_INFORMATION)Information;
            SizeInfo->AceCount = Acl->AceCount;
            if (Ace)
            {
                /* Return how much space there is in the ACL */
                SizeInfo->AclBytesInUse = (ULONG_PTR)Ace - (ULONG_PTR)Acl;
                SizeInfo->AclBytesFree = Acl->AclSize - SizeInfo->AclBytesInUse;
            }
            else
            {
                /* No free ACE, means the whole ACL is full */
                SizeInfo->AclBytesInUse = Acl->AclSize;
                SizeInfo->AclBytesFree = 0;
            }
            break;

        default:
            /* Anything else is illegal */
            return STATUS_INVALID_INFO_CLASS;
    }

    /* All done */
    return STATUS_SUCCESS;
}

/*
 * @implemented
 */
NTSTATUS
NTAPI
RtlSetInformationAcl(IN PACL Acl,
                     IN PVOID Information,
                     IN ULONG InformationLength,
                     IN ACL_INFORMATION_CLASS InformationClass)
{
    PACL_REVISION_INFORMATION Info ;
    PAGED_CODE_RTL();

    /* Validate the ACL revision */
    if ((Acl->AclRevision < MIN_ACL_REVISION) ||
        (Acl->AclRevision > MAX_ACL_REVISION))
    {
        return STATUS_INVALID_PARAMETER;
    }

    /* What is the caller trying to set? */
    switch (InformationClass)
    {
        /* This is the only info class */
        case AclRevisionInformation:

            /* Make sure the buffer is large enough */
            if (InformationLength < sizeof(ACL_REVISION_INFORMATION))
            {
                return STATUS_BUFFER_TOO_SMALL;
            }

            /* Make sure the new revision is within the acceptable bounds*/
            Info = (PACL_REVISION_INFORMATION)Information;
            if (Acl->AclRevision >= Info->AclRevision)
            {
                return STATUS_INVALID_PARAMETER;
            }

            /* Set the new revision */
            Acl->AclRevision = (BYTE)Info->AclRevision;
            break;

        default:
            /* Anything else is invalid */
            return STATUS_INVALID_INFO_CLASS;
    }

    /* All good */
    return STATUS_SUCCESS;
}

/*
 * @implemented
 */
BOOLEAN
NTAPI
RtlValidAcl(IN PACL Acl)
{
    PACE_HEADER Ace;
    PISID Sid;
    ULONG i;
    PAGED_CODE_RTL();

    _SEH2_TRY
    {
        /* First, validate the revision */
        if ((Acl->AclRevision < MIN_ACL_REVISION) ||
            (Acl->AclRevision > MAX_ACL_REVISION))
        {
            DPRINT1("Invalid ACL revision\n");
            _SEH2_YIELD(return FALSE);
        }

        /* Next, validate that the ACL is USHORT-aligned */
        if (ROUND_DOWN(Acl->AclSize, sizeof(USHORT)) != Acl->AclSize)
        {
            DPRINT1("Invalid ACL size\n");
            _SEH2_YIELD(return FALSE);
        }

        /* And that it's big enough */
        if (Acl->AclSize < sizeof(ACL))
        {
            DPRINT1("Invalid ACL size\n");
            _SEH2_YIELD(return FALSE);
        }

        /* Loop each ACE */
        Ace = (PACE_HEADER)((ULONG_PTR)Acl + sizeof(ACL));
        for (i = 0; i < Acl->AceCount; i++)
        {
            /* Validate we have space for this ACE header */
            if (((ULONG_PTR)Ace + sizeof(ACE_HEADER)) >= ((ULONG_PTR)Acl + Acl->AclSize))
            {
                DPRINT1("Invalid ACE size\n");
                _SEH2_YIELD(return FALSE);
            }

            /* Validate the length of this ACE */
            if (ROUND_DOWN(Ace->AceSize, sizeof(USHORT)) != Ace->AceSize)
            {
                DPRINT1("Invalid ACE size: %lx\n", Ace->AceSize);
                _SEH2_YIELD(return FALSE);
            }

            /* Validate we have space for the entire ACE */
            if (((ULONG_PTR)Ace + Ace->AceSize) > ((ULONG_PTR)Acl + Acl->AclSize))
            {
                DPRINT1("Invalid ACE size %lx %lx\n", Ace->AceSize, Acl->AclSize);
                _SEH2_YIELD(return FALSE);
            }

            /* Check what kind of ACE this is */
            if (Ace->AceType <= ACCESS_MAX_MS_V2_ACE_TYPE)
            {
                /* Validate the length of this ACE */
                if (ROUND_DOWN(Ace->AceSize, sizeof(ULONG)) != Ace->AceSize)
                {
                    DPRINT1("Invalid ACE size\n");
                    _SEH2_YIELD(return FALSE);
                }

                /* The ACE size should at least have enough for the header */
                if (Ace->AceSize < sizeof(ACE_HEADER))
                {
                    DPRINT1("Invalid ACE size: %lx %lx\n", Ace->AceSize, sizeof(ACE_HEADER));
                    _SEH2_YIELD(return FALSE);
                }

                /* Check if the SID revision is valid */
                Sid = (PISID)&((PKNOWN_ACE)Ace)->SidStart;
                if (Sid->Revision != SID_REVISION)
                {
                    DPRINT1("Invalid SID\n");
                    _SEH2_YIELD(return FALSE);
                }

                /* Check if the SID is out of bounds */
                if (Sid->SubAuthorityCount > SID_MAX_SUB_AUTHORITIES)
                {
                    DPRINT1("Invalid SID\n");
                    _SEH2_YIELD(return FALSE);
                }

                /* The ACE size should at least have enough for the header and SID */
                if (Ace->AceSize < (sizeof(ACE_HEADER) + RtlLengthSid(Sid)))
                {
                    DPRINT1("Invalid ACE size\n");
                    _SEH2_YIELD(return FALSE);
                }
            }
            else if (Ace->AceType == ACCESS_ALLOWED_COMPOUND_ACE_TYPE)
            {
                DPRINT1("Unsupported ACE in ReactOS, assuming valid\n");
            }
            else if ((Ace->AceType >= ACCESS_MIN_MS_OBJECT_ACE_TYPE) &&
                     (Ace->AceType <= ACCESS_MAX_MS_OBJECT_ACE_TYPE))
            {
                DPRINT1("Unsupported ACE in ReactOS, assuming valid\n");
            }
            else
            {
                /* Unknown ACE, see if it's as big as a header at least */
                if (Ace->AceSize < sizeof(ACE_HEADER))
                {
                    DPRINT1("Unknown ACE\n");
                    _SEH2_YIELD(return FALSE);
                }
            }

            /* Move to the next ace */
            Ace = (PACE_HEADER)((ULONG_PTR)Ace + Ace->AceSize);
        }
    }
    _SEH2_EXCEPT(EXCEPTION_EXECUTE_HANDLER)
    {
        /* Something was invalid, fail */
        _SEH2_YIELD(return FALSE);
    }
    _SEH2_END;

    /* The ACL looks ok */
    return TRUE;
}

//
//  Internal support routine
//

NTSTATUS
NTAPI
RtlAddCompoundAce (
    IN PACL Acl,
    IN ULONG AceRevision,
    IN UCHAR CompoundAceType,
    IN ACCESS_MASK AccessMask,
    IN PSID ServerSid,
    IN PSID ClientSid
    )

/*++

Routine Description:

    This routine adds a KNOWN_COMPOUND_ACE to an ACL.  This is
    expected to be a common form of ACL modification.

Arguments:

    Acl - Supplies the Acl being modified

    AceRevision - Supplies the Acl/Ace revision of the ACE being added

    CompoundAceType - Supplies the type of compound ACE being added.
        Currently the only defined type is COMPOUND_ACE_IMPERSONATION.

    AccessMask - The mask of accesses to be granted to the specified SID pair.

    ServerSid - Pointer to the Server SID to be placed in the ACE.

    ClientSid - Pointer to the Client SID to be placed in the ACE.

Return Value:

    NTSTATUS - STATUS_SUCCESS if successful and an appropriate error
        status otherwise

    STATUS_INVALID_PARAMETER - The AceFlags parameter was invalid.

--*/




{
    PVOID FirstFree = NULL;
    USHORT AceSize;
    PKNOWN_COMPOUND_ACE GrantAce;
    UCHAR NewRevision;

    RTL_PAGED_CODE;

    //
    // Validate the structure of the SID
    //

    if (!RtlValidSid(ServerSid) || !RtlValidSid(ClientSid)) {
        return STATUS_INVALID_SID;
    }

    //
    //  Check the ACL & ACE revision levels
    // Compund ACEs become valid in version 3.
    //

    if ( Acl->AclRevision > ACL_REVISION4 ||
         AceRevision < ACL_REVISION3 ||
         AceRevision > ACL_REVISION4 ) {
        return STATUS_REVISION_MISMATCH;
    }

    //
    // Calculate the new revision of the ACL.  The new revision is the maximum
    // of the old revision and and new ACE's revision.  This is possible because
    // the format of previously defined ACEs did not change across revisions.
    //

    NewRevision = Acl->AclRevision > (UCHAR)AceRevision ? Acl->AclRevision : (UCHAR)AceRevision;

    //
    //  Locate the first free ace and check to see that the Acl is
    //  well formed.
    //

    if (!RtlValidAcl( Acl )) {
        return STATUS_INVALID_ACL;
    }

    if (!RtlFirstFreeAce( Acl, (PACE* )FirstFree )) {

        return STATUS_INVALID_ACL;
    }

    //
    //  Check to see if there is enough room in the Acl to store the new
    //  ACE
    //

    AceSize = (USHORT)(sizeof(KNOWN_COMPOUND_ACE) -
                       sizeof(ULONG)              +
                       SeLengthSid(ClientSid)    +
                       SeLengthSid(ServerSid)
                       );

    if (  FirstFree == NULL ||
          ((PUCHAR)FirstFree + AceSize > ((PUCHAR)Acl + Acl->AclSize))
       ) {

        return STATUS_ALLOTTED_SPACE_EXCEEDED;
    }

    //
    // Add the ACE to the end of the ACL
    //

    GrantAce = (PKNOWN_COMPOUND_ACE)FirstFree;
    GrantAce->Header.AceFlags = 0;
    GrantAce->Header.AceType = ACCESS_ALLOWED_COMPOUND_ACE_TYPE;
    GrantAce->Header.AceSize = AceSize;
    GrantAce->Mask = AccessMask;
    GrantAce->CompoundAceType = CompoundAceType;
    RtlCopySid( SeLengthSid(ServerSid), (PSID)(&GrantAce->SidStart), ServerSid );
    RtlCopySid( SeLengthSid(ClientSid), (PSID)(((PCHAR)&GrantAce->SidStart) + SeLengthSid(ServerSid)), ClientSid );

    //
    // Increment the number of ACEs by 1.
    //

    Acl->AceCount += 1;

    //
    // Adjust the Acl revision, if necessary
    //

    Acl->AclRevision = NewRevision;

    //
    //  And return to our caller
    //

    return STATUS_SUCCESS;
}


/* EOF */
