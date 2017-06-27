@ stdcall ConvertDefaultLocale(long) kernel32.ConvertDefaultLocale
@ stdcall FindNLSString(long long ptr long wstr long ptr) kernel32.FindNLSString
@ stdcall FindNLSStringEx(wstr long wstr long wstr long ptr ptr ptr ptr) kernel32.FindNLSStringEx
@ stdcall GetACP() kernel32.GetACP
@ stdcall GetCalendarInfoEx(wstr long wstr long wstr long ptr) kernel32.GetCalendarInfoEx
@ stdcall GetCalendarInfoW(long long long ptr long ptr) kernel32.GetCalendarInfoW
@ stdcall GetCPFileNameFromRegistry(long wstr long) kernel32.GetCPFileNameFromRegistry
@ stdcall GetCPInfo(long ptr) kernel32.GetCPInfo
@ stdcall GetCPInfoExW(long long ptr) kernel32.GetCPInfoExW
@ stdcall GetFileMUIInfo(long wstr ptr ptr) kernel32.GetFileMUIInfo
@ stdcall GetFileMUIPath(long wstr wstr ptr wstr ptr ptr) kernel32.GetFileMUIPath
@ stdcall GetLocaleInfoEx(wstr long ptr long) kernel32.GetLocaleInfoEx
@ stdcall GetLocaleInfoW(long long ptr long) kernel32.GetLocaleInfoW
@ stdcall GetNLSVersion(long long ptr) kernel32.GetNLSVersion
@ stdcall GetNLSVersionEx(long wstr ptr) kernel32.GetNLSVersionEx
@ stdcall GetOEMCP() kernel32.GetOEMCP
@ stdcall GetProcessPreferredUILanguages(long ptr wstr ptr) kernel32.GetProcessPreferredUILanguages
@ stdcall GetSystemDefaultLangID() kernel32.GetSystemDefaultLangID
@ stdcall GetSystemDefaultLCID() kernel32.GetSystemDefaultLCID
@ stdcall GetSystemPreferredUILanguages(long ptr wstr ptr) kernel32.GetSystemPreferredUILanguages
@ stdcall GetThreadLocale() kernel32.GetThreadLocale
@ stdcall GetThreadPreferredUILanguages(long ptr wstr ptr) kernel32.GetThreadPreferredUILanguages
@ stdcall GetThreadUILanguage() kernel32.GetThreadUILanguage
@ stdcall GetUILanguageInfo(long wstr wstr ptr ptr) kernel32.GetUILanguageInfo
@ stdcall GetUserDefaultLangID() kernel32.GetUserDefaultLangID
@ stdcall GetUserPreferredUILanguages(long ptr wstr ptr) kernel32.GetUserPreferredUILanguages
@ stdcall GetUserDefaultLCID() kernel32.GetUserDefaultLCID
@ stdcall IsNLSDefinedString(long long ptr long long) kernel32.IsNLSDefinedString
@ stdcall IsValidCodePage(long) kernel32.IsValidCodePage
@ stdcall IsValidLanguageGroup(long long) kernel32.IsValidLanguageGroup
@ stdcall IsValidLocale(long long) kernel32.IsValidLocale
@ stdcall LCMapStringEx(wstr long wstr long ptr long ptr ptr long) kernel32.LCMapStringEx
@ stdcall LCMapStringW(long long wstr long ptr long) kernel32.LCMapStringW
@ stdcall NlsCheckPolicy(wstr long) kernel32.NlsCheckPolicy
@ stdcall NlsEventDataDescCreate(ptr wstr wstr long) kernel32.NlsEventDataDescCreate
@ stdcall NlsGetCacheUpdateCount() kernel32.NlsGetCacheUpdateCount
@ stdcall NlsUpdateLocale(long long) kernel32.NlsUpdateLocale
@ stdcall NlsUpdateSystemLocale(long long) kernel32.NlsUpdateSystemLocale
@ stdcall NlsWriteEtwEvent(int64 ptr long ptr) kernel32.NlsWriteEtwEvent
@ stdcall ResolveLocaleName(wstr wstr long) kernel32.ResolveLocaleName
@ stdcall SetCalendarInfoW(long long long wstr) kernel32.SetCalendarInfoW
@ stdcall SetLocaleInfoW(long long wstr) kernel32.SetLocaleInfoW
@ stdcall SetThreadLocale(long) kernel32.SetThreadLocale
@ stdcall VerLanguageNameA(long str long) kernel32.VerLanguageNameA
@ stdcall VerLanguageNameW(long wstr long) kernel32.VerLanguageNameW