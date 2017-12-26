#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "locale.h"

#define MUI_LANGUAGE_NAME                   0x08
#define MUI_LANGUAGE_ID						0x04
#define MUI_MERGE_USER_FALLBACK 0x20
#define MUI_MERGE_SYSTEM_FALLBACK 0x10
#define MUI_THREAD_LANGUAGES                0x40
#define MUI_UI_FALLBACK                     MUI_MERGE_SYSTEM_FALLBACK | MUI_MERGE_USER_FALLBACK

typedef struct _LSA_UNICODE_STRING {
  USHORT Length;
  USHORT MaximumLength;
  PWSTR  Buffer;
} LSA_UNICODE_STRING, *PLSA_UNICODE_STRING, UNICODE_STRING, *PUNICODE_STRING;

#define DECLARE_UNICODE_STRING(_var, _string) \
const WCHAR _var ## _buffer[] = _string; \
UNICODE_STRING _var = { sizeof(_string) - sizeof(WCHAR), sizeof(_string), (PWCH) _var ## _buffer }

static int (WINAPI *pLCIDToLocaleName)(LCID, LPWSTR, int ,  DWORD);
static int (WINAPI *pLocaleNameToLCID)(LPCWSTR, DWORD);
static int (WINAPI *pGetSystemDefaultLocaleName)(LPCWSTR, int);
static BOOL (WINAPI *pGetThreadPreferredUILanguages)(DWORD, PULONG, PZZWSTR, PULONG);
static NTSTATUS (WINAPI *pRtlGetThreadPreferredUILanguages)(DWORD, PULONG, PZZWSTR, PULONG);
static BOOL (WINAPI *pGetUserPreferredUILanguages)(DWORD, PULONG, PZZWSTR, PULONG);
static void (WINAPI *pRtlGetNtVersionNumbers)(LPDWORD, LPDWORD, LPDWORD);
static NTSTATUS (WINAPI *pRtlLCIDToCultureName)(ULONG lcid, PUNICODE_STRING name);
HMODULE hntdll;

void testLCIDToLocale(){
	LCID lcid;
	int length;
	WCHAR strNameBuffer[LOCALE_NAME_MAX_LENGTH];
	pLCIDToLocaleName = (void *)GetProcAddress(hntdll, "LCIDToLocaleName");	
	printf("Enter LCID\n");
	scanf("%x", &lcid);
	if(pLCIDToLocaleName){
		length = pLCIDToLocaleName(lcid, strNameBuffer, 256, 0);
		wprintf(L"The size of string is %d. And Locale Name is %s\n", length, strNameBuffer);
	}	
}

void testRtlLCIDToLocale(){
	LCID lcid;
	int length;
	//PUNICODE_STRING name;
	DECLARE_UNICODE_STRING(name, L"dummystringinitializer");
	pRtlLCIDToCultureName = (void *)GetProcAddress(hntdll, "RtlLCIDToCultureName");	
	printf("Enter LCID\n");
	scanf("%x", &lcid);
	if(pRtlLCIDToCultureName){
		wprintf(L"We have RtlLCIDToCultureName\n");
		length = pRtlLCIDToCultureName(lcid, &name);
		wprintf(L"Locale Name is %s\n", name.Buffer);
	}	
}

void testLocaleNameToLcid(){
	WCHAR strNameBuffer[] = L"en-US";
	//strNameBuffer = L"en-US";
	LCID lcid;
	pLocaleNameToLCID = (void *)GetProcAddress(hntdll, "LocaleNameToLCID");
	if(pLocaleNameToLCID){
		lcid = pLocaleNameToLCID(strNameBuffer, 0);
		wprintf(L"LCID is 0x%x\n", lcid);
	}	
}

void testGetThreadPreferredUILanguages(){
	ULONG numLang = 0;
	pGetThreadPreferredUILanguages = (void *)GetProcAddress(hntdll, "GetThreadPreferredUILanguages"); 
	DWORD byteOffset = 0;
	WCHAR *buf;
	BOOL ret;
	ULONG count, size;	
	if(pGetThreadPreferredUILanguages){
		wprintf(L"We have GetThreadPreferredUILanguages\n");
			        
	    size = count = 0;
	    
	    ret = pGetThreadPreferredUILanguages(MUI_UI_FALLBACK, &count, NULL, &size);	
			
	    count = 0;
	    buf = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size * sizeof(WCHAR));
	    ret = pGetThreadPreferredUILanguages(MUI_LANGUAGE_ID, &count, buf, &size);
	    wprintf(L"Count langs: %d\n", count);	    
	    wprintf(L"Buffer Size is %d\n",size);
    	wprintf(L"Lang is %s\n", buf);
	    HeapFree(GetProcessHeap(), 0, buf);
	}	
}

void testGetUserPreferredUILanguages(){
	ULONG numLang = 0;
	pGetUserPreferredUILanguages = (void *)GetProcAddress(hntdll, "RtlGetUserPreferredUILanguages"); 
	DWORD byteOffset = 0;
	WCHAR *buf;
	BOOL ret;
	ULONG count, size;	
	if(pGetUserPreferredUILanguages){
		wprintf(L"We have RtlGetUserPreferredUILanguages\n");
			        
	    size = count = 0;
	    
	    ret = pGetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &count, NULL, &size);	
			
	    count = 0;
	    buf = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size * sizeof(WCHAR));
	    ret = pGetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &count, buf, &size);
	    wprintf(L"Count langs: %d\n", count);	    
	    wprintf(L"Buffer Size is %d\n",size);
    	wprintf(L"Lang is %s\n", buf);
	    HeapFree(GetProcessHeap(), 0, buf);
	}	
}

void testRtlGetThreadPreferredUILanguages(){
	ULONG numLang = 0;
	pRtlGetThreadPreferredUILanguages = (void *)GetProcAddress(hntdll, "RtlGetThreadPreferredUILanguages"); 
	DWORD byteOffset = 0;
	WCHAR *buf;
	BOOL ret;
	ULONG count, size;	
	if(pRtlGetThreadPreferredUILanguages){
		wprintf(L"We have RtlGetThreadPreferredUILanguages\n");
			        
	    size = count = 0;
	    
	   // ret = pRtlGetThreadPreferredUILanguages(MUI_LANGUAGE_NAME, &count, NULL, &size);	
			
//	    count = 0;
//	    buf = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size * sizeof(WCHAR));
//	    ret = pRtlGetThreadPreferredUILanguages(MUI_LANGUAGE_NAME, &count, buf, &size);
//	    wprintf(L"Count langs: %d\n", count);	    
//	    wprintf(L"Buffer Size is %d\n",size);
//    	wprintf(L"Lang is %s\n", buf);
//	    HeapFree(GetProcessHeap(), 0, buf);
	}	
}

void testRtlNtVersion(){
	DWORD major, minor, build;
	pRtlGetNtVersionNumbers = (void *)GetProcAddress(hntdll, "RtlGetNtVersionNumbers"); 
	if(pRtlGetNtVersionNumbers){
		wprintf(L"We have RtlGetNtVersionNumbers\n");  
	    pRtlGetNtVersionNumbers(&major, &minor, &build);	
	    wprintf(L"Major is %d\n", major);	    
	    wprintf(L"Mnior is %d\n", minor);
    	wprintf(L"Build is %04x\n", build);
	}		
}

void testGetSystemDefaultLocaleName(){
	WCHAR strNameBuffer[LOCALE_NAME_MAX_LENGTH];
	pGetSystemDefaultLocaleName = (void *)GetProcAddress(hntdll, "GetSystemDefaultLocaleName");
	pGetSystemDefaultLocaleName(strNameBuffer, LOCALE_NAME_MAX_LENGTH);
	wprintf(L"System Default Locale Name is %s\n", strNameBuffer);
}

void testCopyWchar(){
	  wchar_t wcs1[] = L"To be or not to be";
	  wchar_t wcs2[40];
	  wchar_t wcs3[40];
	
	  /* copy to sized buffer (overflow safe): */
	  wcsncpy ( wcs2, wcs1, 40 );
	
	  /* partial copy (only 5 characters): */
	  wcsncpy ( wcs3, wcs2, 5 );
	  wcs3[5] = L'\0';   /* null character manually added */
	
	  wprintf (L"%ls\n%ls\n%ls\n",wcs1,wcs2,wcs3);	
}
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {	
	int i;			
	hntdll = LoadLibraryA("ntdllnew.dll");

	//testLCIDToLocale();

	//testGetThreadPreferredUILanguages();
	//testGetUserPreferredUILanguages();
	//testRtlNtVersion();
	//testRtlLCIDToLocale();
	testRtlGetThreadPreferredUILanguages();

	system("PAUSE");
	return 0;
}
