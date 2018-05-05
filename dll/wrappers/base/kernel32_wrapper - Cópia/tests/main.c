#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "locale.h"

static int (WINAPI *pLCIDToLocaleName)(LCID, LPWSTR, int ,  DWORD);
static int (WINAPI *pLocaleNameToLCID)(LPCWSTR, DWORD);
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int main(int argc, char *argv[]) {	
	int i;
	LCID lcid;
	WCHAR strNameBuffer[LOCALE_NAME_MAX_LENGTH];	
	HMODULE hkernel32 = GetModuleHandleA("kernel32.dll");

	pLCIDToLocaleName = (void *)GetProcAddress(hkernel32, "LCIDToLocaleName");	
	pLocaleNameToLCID = (void *)GetProcAddress(hkernel32, "LocaleNameToLCID");
	printf("Enter LCID\n");
	scanf("%x", &lcid);
	if(pLCIDToLocaleName){
		pLCIDToLocaleName(lcid, strNameBuffer, 256, 0);
		wprintf(L"Locale Name is %s\n", strNameBuffer);
	}
	if(pLocaleNameToLCID){
		lcid = pLocaleNameToLCID(strNameBuffer, 0);
		wprintf(L"LCID is 0x%x\n", lcid);
	}
//	for(i=0;i<LOCALE_TABLE_SIZE;i++){
//		if(lcid == locale_table[i].lcidID){
//			wcscpy(strNameBuffer, locale_table[i].localeName);
//		}
//	}
	//wprintf(L"Locale Name is %s\n", strNameBuffer);
	system("PAUSE");
	return 0;
}
