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

static int (WINAPI *pLCIDToLocaleName)(LCID, LPWSTR, int ,  DWORD);
static int (WINAPI *pLocaleNameToLCID)(LPCWSTR, DWORD);
static int (WINAPI *pGetSystemDefaultLocaleName)(LPCWSTR, int);
static BOOL (WINAPI *pGetThreadPreferredUILanguages)(DWORD, PULONG, PZZWSTR, PULONG);
static BOOL (WINAPI *pGetSystemPreferredUILanguages)(DWORD, PULONG, PZZWSTR, PULONG);
static BOOL (WINAPI *pGetUserPreferredUILanguages)(DWORD, PULONG, PZZWSTR, PULONG);
HMODULE hkernel32;

void testLCIDToLocale(){
	LCID lcid;
	int length;
	WCHAR strNameBuffer[LOCALE_NAME_MAX_LENGTH];
	pLCIDToLocaleName = (void *)GetProcAddress(hkernel32, "LCIDToLocaleName");	
	printf("Enter LCID\n");
	scanf("%x", &lcid);
	if(pLCIDToLocaleName){
		length = pLCIDToLocaleName(lcid, strNameBuffer, 256, 0);
		wprintf(L"The size of string is %d. And Locale Name is %s\n", length, strNameBuffer);
	}	
}

void testLocaleNameToLcid(){
	WCHAR strNameBuffer[] = L"en-US";
	//strNameBuffer = L"en-US";
	LCID lcid;
	pLocaleNameToLCID = (void *)GetProcAddress(hkernel32, "LocaleNameToLCID");
	if(pLocaleNameToLCID){
		lcid = pLocaleNameToLCID(strNameBuffer, 0);
		wprintf(L"LCID is 0x%x\n", lcid);
	}	
}

void testGetThreadPreferredUILanguages(){
	ULONG numLang = 0;
	pGetThreadPreferredUILanguages = (void *)GetProcAddress(hkernel32, "GetThreadPreferredUILanguages"); 
	DWORD byteOffset = 0;
	WCHAR *buf;
	BOOL ret;
	ULONG count, size;	
	if(pGetThreadPreferredUILanguages){
		wprintf(L" --------------------------------------------------------------------- \n");
		wprintf(L"We have GetThreadPreferredUILanguages\n");
			        
	    size = count = 0;
	    
	    ret = pGetThreadPreferredUILanguages(MUI_LANGUAGE_NAME, &count, NULL, &size);	
			
	    count = 0;
	    buf = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size * sizeof(WCHAR));
	    ret = pGetThreadPreferredUILanguages(MUI_LANGUAGE_NAME, &count, buf, &size);
	    wprintf(L"Count langs: %d\n", count);	    
	    wprintf(L"Buffer Size is %d\n",size);
    	wprintf(L"Lang is %s\n", buf);
		wprintf(L"\n\n");
	    HeapFree(GetProcessHeap(), 0, buf);
	}	
}

void testGetSystemPreferredUILanguages(){
	ULONG numLang = 0;
	pGetSystemPreferredUILanguages = (void *)GetProcAddress(hkernel32, "GetSystemPreferredUILanguages"); 
	DWORD byteOffset = 0;
	WCHAR *buf;
	BOOL ret;
	ULONG count, size;	
	if(pGetSystemPreferredUILanguages){
		wprintf(L" --------------------------------------------------------------------- \n");
		wprintf(L"We have GetSystemPreferredUILanguages\n");
			        
	    size = count = 0;
	    
	    ret = pGetSystemPreferredUILanguages(MUI_LANGUAGE_NAME, &count, NULL, &size);	
			
	    count = 0;
	    buf = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size * sizeof(WCHAR));
	    ret = pGetSystemPreferredUILanguages(MUI_LANGUAGE_NAME, &count, buf, &size);
	    wprintf(L"Count langs: %d\n", count);	    
	    wprintf(L"Buffer Size is %d\n",size);
    	wprintf(L"Lang is %s\n", buf);	
		wprintf(L"\n\n");
	    HeapFree(GetProcessHeap(), 0, buf);
	}	
}

void testGetUserPreferredUILanguages(){
	ULONG numLang = 0;
	pGetUserPreferredUILanguages = (void *)GetProcAddress(hkernel32, "GetUserPreferredUILanguages"); 
	DWORD byteOffset = 0;
	WCHAR *buf;
	BOOL ret;
	ULONG count, size;	
	if(pGetUserPreferredUILanguages){
		wprintf(L" --------------------------------------------------------------------- \n");
		wprintf(L"We have GetUserPreferredUILanguages\n");
			        
	    size = count = 0;
	    
	    ret = pGetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &count, NULL, &size);	
			
	    count = 0;
	    buf = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, size * sizeof(WCHAR));
	    ret = pGetUserPreferredUILanguages(MUI_LANGUAGE_NAME, &count, buf, &size);
	    wprintf(L"Count langs: %d\n", count);	    
	    wprintf(L"Buffer Size is %d\n",size);
    	wprintf(L"Lang is %s\n", buf);
		wprintf(L"\n\n");
	    HeapFree(GetProcessHeap(), 0, buf);
	}	
}

void testGetSystemDefaultLocaleName(){
	WCHAR strNameBuffer[LOCALE_NAME_MAX_LENGTH];
	pGetSystemDefaultLocaleName = (void *)GetProcAddress(hkernel32, "GetSystemDefaultLocaleName");
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
	hkernel32 = LoadLibraryA("kernel32.dll");

	//testLCIDToLocale();

	testGetThreadPreferredUILanguages();
	testGetUserPreferredUILanguages();
	testGetSystemPreferredUILanguages();	

	system("PAUSE");
	return 0;
}
