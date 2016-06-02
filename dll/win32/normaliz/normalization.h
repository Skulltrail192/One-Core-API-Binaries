// Normalization.h
// Copyright 2002 Microsoft
//
// Excerpted from LH winnls.h

#ifdef __cplusplus
extern "C" {
#endif

//
//  Normalization forms
//

typedef enum _NORM_FORM {
    NormalizationOther  = 0,       // Not supported
    NormalizationC      = 0x1,     // Each base plus combining characters to the canonical precomposed equivalent.
    NormalizationD      = 0x2,     // Each precomposed character to its canonical decomposed equivalent.
    NormalizationKC     = 0x5,     // Each base plus combining characters to the canonical precomposed
                                   //   equivalents and all compatibility characters to their equivalents.
    NormalizationKD     = 0x6      // Each precomposed character to its canonical decomposed equivalent
                                   //   and all compatibility characters to their equivalents.
} NORM_FORM;

//
// IDN (International Domain Name) Flags
//
#define IDN_ALLOW_UNASSIGNED      0x01  // Allow unassigned "query" behavior per RFC 3454
#define IDN_USE_STD3_ASCII_RULES  0x02  // Enforce STD3 ASCII restrictions for legal characters
//
// Windows API Normalization Functions
//

int
WINAPI NormalizeString( __in                      NORM_FORM NormForm,
                        __in_ecount(cwSrcLength)  LPCWSTR   lpSrcString,
                        __in                      int       cwSrcLength,
                        __out_ecount(cwDstLength) LPWSTR    lpDstString,
                        __in                      int       cwDstLength );

BOOL
WINAPI IsNormalizedString( __in                   NORM_FORM NormForm,
                           __in_ecount(cwLength)  LPCWSTR   lpString,
                           __in                   int       cwLength );

//
// IDN (International Domain Name) Functions
//
int
WINAPI IdnToAscii(__in                        DWORD    dwFlags,
                  __in_ecount(cchUnicodeChar) LPCWSTR  lpUnicodeCharStr,
                  __in                        int      cchUnicodeChar,
                  __out_ecount(cchASCIIChar)  LPWSTR   lpASCIICharStr,
                  __in                        int      cchASCIIChar);

int
WINAPI IdnToNameprepUnicode(__in                            DWORD   dwFlags,
                            __in_ecount(cchUnicodeChar)     LPCWSTR lpUnicodeCharStr,
                            __in                            int     cchUnicodeChar,
                            __out_ecount(cchNameprepChar)   LPWSTR  lpNameprepCharStr,
                            __in                            int     cchNameprepChar);

int
WINAPI IdnToUnicode(__in                         DWORD   dwFlags,
                    __in_ecount(cchASCIIChar)    LPCWSTR lpASCIICharStr,
                    __in                         int     cchASCIIChar,
                    __out_ecount(cchUnicodeChar) LPWSTR  lpUnicodeCharStr,
                    __in                         int     cchUnicodeChar);

#ifdef __cplusplus
}
#endif
