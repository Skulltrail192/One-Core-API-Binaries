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

#include "config.h"
#include <assert.h>
#include <rtl.h>
#include "localelcid.h"

/* Use intrinsics for x86 and x64 */
#if defined(_M_IX86) || defined(_M_AMD64)
#define InterlockedCompareExchange _InterlockedCompareExchange
#define InterlockedIncrement _InterlockedIncrement
#define InterlockedDecrement _InterlockedDecrement
#define InterlockedExchangeAdd _InterlockedExchangeAdd
#define InterlockedExchange _InterlockedExchange
#define InterlockedBitTestAndSet _interlockedbittestandset
#define InterlockedBitTestAndSet64 _interlockedbittestandset64
#endif


/* Definitions *****************************************/

typedef PVOID* PALPC_HANDLE;
typedef PVOID ALPC_HANDLE;

/* SHA1 Helper Macros */

#define rol(value, bits) (((value) << (bits)) | ((value) >> (32 - (bits))))
/* FIXME: This definition of DWORD2BE is little endian specific! */
#define DWORD2BE(x) (((x) >> 24) & 0xff) | (((x) >> 8) & 0xff00) | (((x) << 8) & 0xff0000) | (((x) << 24) & 0xff000000);
/* FIXME: This definition of blk0 is little endian specific! */
#define blk0(i) (Block[i] = (rol(Block[i],24)&0xFF00FF00)|(rol(Block[i],8)&0x00FF00FF))
#define blk1(i) (Block[i&15] = rol(Block[(i+13)&15]^Block[(i+8)&15]^Block[(i+2)&15]^Block[i&15],1))
#define f1(x,y,z) (z^(x&(y^z)))
#define f2(x,y,z) (x^y^z)
#define f3(x,y,z) ((x&y)|(z&(x|y)))
#define f4(x,y,z) (x^y^z)
/* (R0+R1), R2, R3, R4 are the different operations used in SHA1 */
#define R0(v,w,x,y,z,i) z+=f1(w,x,y)+blk0(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R1(v,w,x,y,z,i) z+=f1(w,x,y)+blk1(i)+0x5A827999+rol(v,5);w=rol(w,30);
#define R2(v,w,x,y,z,i) z+=f2(w,x,y)+blk1(i)+0x6ED9EBA1+rol(v,5);w=rol(w,30);
#define R3(v,w,x,y,z,i) z+=f3(w,x,y)+blk1(i)+0x8F1BBCDC+rol(v,5);w=rol(w,30);
#define R4(v,w,x,y,z,i) z+=f4(w,x,y)+blk1(i)+0xCA62C1D6+rol(v,5);w=rol(w,30);

/* Enumarations ****************************************/

/* Structs *********************************************/

typedef struct {
   ULONG Unknown[6];
   ULONG State[5];
   ULONG Count[2];
   UCHAR Buffer[64];
} SHA_CTX, *PSHA_CTX;

static void SHA1Transform(ULONG State[5], UCHAR Buffer[64])
{
   ULONG a, b, c, d, e;
   ULONG *Block;

   Block = (ULONG*)Buffer;

   /* Copy Context->State[] to working variables */
   a = State[0];
   b = State[1];
   c = State[2];
   d = State[3];
   e = State[4];

   /* 4 rounds of 20 operations each. Loop unrolled. */
   R0(a,b,c,d,e, 0); R0(e,a,b,c,d, 1); R0(d,e,a,b,c, 2); R0(c,d,e,a,b, 3);
   R0(b,c,d,e,a, 4); R0(a,b,c,d,e, 5); R0(e,a,b,c,d, 6); R0(d,e,a,b,c, 7);
   R0(c,d,e,a,b, 8); R0(b,c,d,e,a, 9); R0(a,b,c,d,e,10); R0(e,a,b,c,d,11);
   R0(d,e,a,b,c,12); R0(c,d,e,a,b,13); R0(b,c,d,e,a,14); R0(a,b,c,d,e,15);
   R1(e,a,b,c,d,16); R1(d,e,a,b,c,17); R1(c,d,e,a,b,18); R1(b,c,d,e,a,19);
   R2(a,b,c,d,e,20); R2(e,a,b,c,d,21); R2(d,e,a,b,c,22); R2(c,d,e,a,b,23);
   R2(b,c,d,e,a,24); R2(a,b,c,d,e,25); R2(e,a,b,c,d,26); R2(d,e,a,b,c,27);
   R2(c,d,e,a,b,28); R2(b,c,d,e,a,29); R2(a,b,c,d,e,30); R2(e,a,b,c,d,31);
   R2(d,e,a,b,c,32); R2(c,d,e,a,b,33); R2(b,c,d,e,a,34); R2(a,b,c,d,e,35);
   R2(e,a,b,c,d,36); R2(d,e,a,b,c,37); R2(c,d,e,a,b,38); R2(b,c,d,e,a,39);
   R3(a,b,c,d,e,40); R3(e,a,b,c,d,41); R3(d,e,a,b,c,42); R3(c,d,e,a,b,43);
   R3(b,c,d,e,a,44); R3(a,b,c,d,e,45); R3(e,a,b,c,d,46); R3(d,e,a,b,c,47);
   R3(c,d,e,a,b,48); R3(b,c,d,e,a,49); R3(a,b,c,d,e,50); R3(e,a,b,c,d,51);
   R3(d,e,a,b,c,52); R3(c,d,e,a,b,53); R3(b,c,d,e,a,54); R3(a,b,c,d,e,55);
   R3(e,a,b,c,d,56); R3(d,e,a,b,c,57); R3(c,d,e,a,b,58); R3(b,c,d,e,a,59);
   R4(a,b,c,d,e,60); R4(e,a,b,c,d,61); R4(d,e,a,b,c,62); R4(c,d,e,a,b,63);
   R4(b,c,d,e,a,64); R4(a,b,c,d,e,65); R4(e,a,b,c,d,66); R4(d,e,a,b,c,67);
   R4(c,d,e,a,b,68); R4(b,c,d,e,a,69); R4(a,b,c,d,e,70); R4(e,a,b,c,d,71);
   R4(d,e,a,b,c,72); R4(c,d,e,a,b,73); R4(b,c,d,e,a,74); R4(a,b,c,d,e,75);
   R4(e,a,b,c,d,76); R4(d,e,a,b,c,77); R4(c,d,e,a,b,78); R4(b,c,d,e,a,79);

   /* Add the working variables back into Context->State[] */
   State[0] += a;
   State[1] += b;
   State[2] += c;
   State[3] += d;
   State[4] += e;

   /* Wipe variables */
   a = b = c = d = e = 0;
}

extern USHORT NlsAnsiCodePage = 0; /* exported */
extern BOOLEAN NlsMbCodePageTag = FALSE; /* exported */
extern BOOLEAN NlsMbOemCodePageTag = FALSE;

/*Functions ********************************************/

BOOL WINAPI DllMain(HINSTANCE hInstDLL, DWORD fdwReason, LPVOID lpv)
{
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
			initTable();
            DisableThreadLibraryCalls(hInstDLL);
            break;
    }

    return TRUE;
}

/*BOOLEAN WINAPI RtlGetIntegerAtom(PWSTR AtomName, PUSHORT IntegerAtom)
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
}*/

NTSTATUS WINAPI NtTraceControl(ULONG FunctionCode, PVOID InBuffer, ULONG InBufferLen, PVOID OutBuffer, ULONG OutBufferLen, PULONG ReturnLength)
{
	return 0x00000000;
}

void WINAPI A_SHAInit(PSHA_CTX Context) 	
{
   /* SHA1 initialization constants */
   Context->State[0] = 0x67452301;
   Context->State[1] = 0xEFCDAB89;
   Context->State[2] = 0x98BADCFE;
   Context->State[3] = 0x10325476;
   Context->State[4] = 0xC3D2E1F0;
   Context->Count[0] =
   Context->Count[1] = 0;
}

VOID WINAPI A_SHAUpdate(PSHA_CTX Context,
		const unsigned char *  	Buffer,
		UINT  	BufferSize 
	) 		
{
   ULONG BufferContentSize;

   BufferContentSize = Context->Count[1] & 63;
   Context->Count[1] += BufferSize;
   if (Context->Count[1] < BufferSize)
      Context->Count[0]++;
   Context->Count[0] += (BufferSize >> 29);

   if (BufferContentSize + BufferSize < 64)
   {
      RtlCopyMemory(&Context->Buffer[BufferContentSize], Buffer,
                    BufferSize);
   }
   else
   {
      while (BufferContentSize + BufferSize >= 64)
      {
         RtlCopyMemory(Context->Buffer + BufferContentSize, Buffer,
                       64 - BufferContentSize);
         Buffer += 64 - BufferContentSize;
         BufferSize -= 64 - BufferContentSize;
         SHA1Transform(Context->State, Context->Buffer);
         BufferContentSize = 0;
      }
      RtlCopyMemory(Context->Buffer + BufferContentSize, Buffer, BufferSize);
   }
}

VOID WINAPI A_SHAFinal(PSHA_CTX Context, PULONG Result) 		
{
   INT Pad, Index;
   UCHAR Buffer[72];
   ULONG *Count;
   ULONG BufferContentSize, LengthHi, LengthLo;

   BufferContentSize = Context->Count[1] & 63;
   if (BufferContentSize >= 56)
      Pad = 56 + 64 - BufferContentSize;
   else
      Pad = 56 - BufferContentSize;

   LengthHi = (Context->Count[0] << 3) | (Context->Count[1] >> (32 - 3));
   LengthLo = (Context->Count[1] << 3);

   RtlZeroMemory(Buffer + 1, Pad - 1);
   Buffer[0] = 0x80;
   Count = (ULONG*)(Buffer + Pad);
   Count[0] = DWORD2BE(LengthHi);
   Count[1] = DWORD2BE(LengthLo);
   A_SHAUpdate(Context, Buffer, Pad + 8);

   for (Index = 0; Index < 5; Index++)
      Result[Index] = DWORD2BE(Context->State[Index]);

   A_SHAInit(Context);
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

NTSTATUS NTAPI WmiSendDataBlock(int a, int b, int c, int d, int e) 
{
	return 0xC000000D;
}	

DWORD NTAPI RtlInitLUnicodeStringFromNullTerminatedString(int a, int b)
{
	return 0;
}

DWORD WINAPI RtlInternString(int a, int b)
{
	return 0xC000000Du;
}


DWORD WINAPI RtlCloseDefinitionIdentityHandle(int a, int b)
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

DWORD WINAPI RtlParseDefinitionIdentity(int a, int b, BOOL c)
{
	return 0xC000000Du;
}

/*subimplemented*/
DWORD WINAPI RtlGetDefinitionIdentityAttributeValue(int a, int b, int c, int d)
{
	return 0xC000000Du;
}

/*subimplemented*/
DWORD WINAPI RtlReleaseStringPointer(PVOID this, int a, int b)
{
	return 0xC000000Du;
}

/*subimplemented*/
void WINAPI RtlReportErrorOrigination(int a1, char ArgList, int a3, int a4)
{
	//UNIMPLEMENTED;
}

/*subimplemented*/
void WINAPI RtlReportErrorPropagation(int a1, int a2, int a3, int a4)
{
	//UNIMPLEMENTED;
}

NTSTATUS NTAPI RtlGetOSProductName(PUNICODE_STRING string, DWORD flag)
{
	string->Length = 0;
	string->MaximumLength = 400;
	string->Buffer = L"Microsoft Windows Longhorn XP Professional Version 2003";
	return 0x00000000;
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
        assert(Type == ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE ||
               Type == ACCESS_ALLOWED_OBJECT_ACE_TYPE ||
               Type == ACCESS_DENIED_CALLBACK_OBJECT_ACE_TYPE ||
               Type == ACCESS_DENIED_OBJECT_ACE_TYPE ||
               Type == SYSTEM_AUDIT_CALLBACK_OBJECT_ACE_TYPE ||
               Type == SYSTEM_AUDIT_OBJECT_ACE_TYPE);
    }
    else
    {
        assert(Type != ACCESS_ALLOWED_CALLBACK_OBJECT_ACE_TYPE &&
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

BOOL WINAPI RtlGetProductInfo(
  _In_   DWORD dwOSMajorVersion,
  _In_   DWORD dwOSMinorVersion,
  _In_   DWORD dwSpMajorVersion,
  _In_   DWORD dwSpMinorVersion,
  _Out_  PDWORD pdwReturnedProductType
)
{
	*pdwReturnedProductType = 0x00000001;
	return TRUE;
}

NTSTATUS WINAPI NtQueryLicenseValue(
    PUNICODE_STRING Name,
    ULONG *Type,
    PVOID Buffer,
    ULONG Length,
    ULONG *DataLength)
{
    return STATUS_SUCCESS;	
}

NTSTATUS NTAPI RtlGetVersion(
  _Out_  PRTL_OSVERSIONINFOW lpVersionInformation
)
{
	lpVersionInformation->dwOSVersionInfoSize = sizeof(PRTL_OSVERSIONINFOW);
	lpVersionInformation->dwMajorVersion = 5;
	lpVersionInformation->dwMinorVersion = 2;
	lpVersionInformation->dwBuildNumber = 3590;
	lpVersionInformation->dwPlatformId = 2;
	return STATUS_SUCCESS;
}
