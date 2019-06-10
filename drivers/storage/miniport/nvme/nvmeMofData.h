#ifndef _nvmeMofData_h_
#define _nvmeMofData_h_

// NVMe_DataType - NVMe_DataType
#define NVMe_DataTypeGuid \
    { 0xc105bb0d,0xc2d8,0x415b, { 0x9b,0x0d,0x91,0xa2,0x99,0x79,0xeb,0xdd } }

#if ! (defined(MIDL_PASS))
DEFINE_GUID(NVMe_DataType_GUID, \
            0xc105bb0d,0xc2d8,0x415b,0x9b,0x0d,0x91,0xa2,0x99,0x79,0xeb,0xdd);
#endif


typedef struct _NVMe_DataType
{
    // 
    LONG Id;
    #define NVMe_DataType_Id_SIZE sizeof(LONG)
    #define NVMe_DataType_Id_ID 1

    // 
    ULONGLONG u64;
    #define NVMe_DataType_u64_SIZE sizeof(ULONGLONG)
    #define NVMe_DataType_u64_ID 2

} NVMe_DataType, *PNVMe_DataType;

#define NVMe_DataType_SIZE (FIELD_OFFSET(NVMe_DataType, u64) + NVMe_DataType_u64_SIZE)

// NVMe_QueryDevInfo - NVMe_QueryDevInfo
// Sample to Query WMI data from the NVMe  Device Driver
#define NVMe_QueryDevInfoGuid \
    { 0x0f70da81,0xfd3f,0x40cb, { 0x8e,0xf1,0x52,0x38,0x5f,0x31,0xdb,0x2f } }

#if ! (defined(MIDL_PASS))
DEFINE_GUID(NVMe_QueryDevInfo_GUID, \
            0x0f70da81,0xfd3f,0x40cb,0x8e,0xf1,0x52,0x38,0x5f,0x31,0xdb,0x2f);
#endif


typedef struct _NVMe_QueryDevInfo
{
    // 
    ULONG maxDataXferSize;
    #define NVMe_QueryDevInfo_maxDataXferSize_SIZE sizeof(ULONG)
    #define NVMe_QueryDevInfo_maxDataXferSize_ID 1

    // 
    ULONG numberOfNamespaces;
    #define NVMe_QueryDevInfo_numberOfNamespaces_SIZE sizeof(ULONG)
    #define NVMe_QueryDevInfo_numberOfNamespaces_ID 2

    // 
    NVMe_DataType Data;
    #define NVMe_QueryDevInfo_Data_SIZE sizeof(NVMe_DataType)
    #define NVMe_QueryDevInfo_Data_ID 3

} NVMe_QueryDevInfo, *PNVMe_QueryDevInfo;

#define NVMe_QueryDevInfo_SIZE (FIELD_OFFSET(NVMe_QueryDevInfo, Data) + NVMe_QueryDevInfo_Data_SIZE)

// NVMe_Method - NVMe_Method
// Sample to invoke methods on WMI class
#define NVMe_MethodGuid \
    { 0x3865ddfa,0x7774,0x4cf4, { 0xb5,0xca,0xb7,0x56,0x9a,0x28,0xfb,0xb4 } }

#if ! (defined(MIDL_PASS))
DEFINE_GUID(NVMe_Method_GUID, \
            0x3865ddfa,0x7774,0x4cf4,0xb5,0xca,0xb7,0x56,0x9a,0x28,0xfb,0xb4);
#endif

//
// Method id definitions for NVMe_Method
#define GetControllerInfo     1
typedef struct _GetControllerInfo_OUT
{
    // 
    USHORT pciVendorId;
    #define GetControllerInfo_OUT_pciVendorId_SIZE sizeof(USHORT)
    #define GetControllerInfo_OUT_pciVendorId_ID 1

    // 
    USHORT pciSsVendId;
    #define GetControllerInfo_OUT_pciSsVendId_SIZE sizeof(USHORT)
    #define GetControllerInfo_OUT_pciSsVendId_ID 2

} GetControllerInfo_OUT, *PGetControllerInfo_OUT;

#define GetControllerInfo_OUT_SIZE (FIELD_OFFSET(GetControllerInfo_OUT, pciSsVendId) + GetControllerInfo_OUT_pciSsVendId_SIZE)

#define GetNameSpaceInfo     2
typedef struct _GetNameSpaceInfo_IN
{
    // 
    ULONG lunId;
    #define GetNameSpaceInfo_IN_lunId_SIZE sizeof(ULONG)
    #define GetNameSpaceInfo_IN_lunId_ID 1

} GetNameSpaceInfo_IN, *PGetNameSpaceInfo_IN;

#define GetNameSpaceInfo_IN_SIZE (FIELD_OFFSET(GetNameSpaceInfo_IN, lunId) + GetNameSpaceInfo_IN_lunId_SIZE)

typedef struct _GetNameSpaceInfo_OUT
{
    // 
    ULONGLONG nSize;
    #define GetNameSpaceInfo_OUT_nSize_SIZE sizeof(ULONGLONG)
    #define GetNameSpaceInfo_OUT_nSize_ID 2

    // 
    ULONGLONG nCap;
    #define GetNameSpaceInfo_OUT_nCap_SIZE sizeof(ULONGLONG)
    #define GetNameSpaceInfo_OUT_nCap_ID 3

} GetNameSpaceInfo_OUT, *PGetNameSpaceInfo_OUT;

#define GetNameSpaceInfo_OUT_SIZE (FIELD_OFFSET(GetNameSpaceInfo_OUT, nCap) + GetNameSpaceInfo_OUT_nCap_SIZE)


#endif
