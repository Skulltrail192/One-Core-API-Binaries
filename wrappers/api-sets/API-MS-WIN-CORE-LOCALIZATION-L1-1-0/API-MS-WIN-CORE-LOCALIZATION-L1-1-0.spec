@ stdcall ConvertDefaultLocale(long) kernelbase.ConvertDefaultLocale
@ stdcall FindNLSString(long long ptr long wstr long ptr) kernelbase.FindNLSString
@ stdcall FindNLSStringEx(wstr long wstr long wstr long ptr ptr ptr ptr) kernelbase.FindNLSStringEx
@ stdcall GetACP() kernelbase.GetACP
@ stdcall GetCalendarInfoEx(wstr long wstr long wstr long ptr) kernelbase.GetCalendarInfoEx
@ stdcall GetCalendarInfoW(long long long ptr long ptr) kernelbase.GetCalendarInfoW
@ stdcall GetCPFileNameFromRegistry(long wstr long) kernelbase.GetCPFileNameFromRegistry
@ stdcall GetCPInfo(long ptr) kernelbase.GetCPInfo
@ stdcall GetCPInfoExW(long long ptr) kernelbase.GetCPInfoExW
@ stdcall GetFileMUIInfo(long wstr ptr ptr) kernelbase.GetFileMUIInfo
@ stdcall GetFileMUIPath(long wstr wstr ptr wstr ptr ptr) kernelbase.GetFileMUIPath
@ stdcall GetLocaleInfoEx(wstr long ptr long) kernelbase.GetLocaleInfoEx
@ stdcall GetLocaleInfoW(long long ptr long) kernelbase.GetLocaleInfoW
@ stdcall GetNLSVersion(long long ptr) kernelbase.GetNLSVersion
@ stdcall GetNLSVersionEx(long wstr ptr) kernelbase.GetNLSVersionEx
@ stdcall GetOEMCP() kernelbase.GetOEMCP
@ stdcall GetProcessPreferredUILanguages(long ptr wstr ptr) kernelbase.GetProcessPreferredUILanguages
@ stdcall GetSystemDefaultLangID() kernelbase.GetSystemDefaultLangID
@ stdcall GetSystemDefaultLCID() kernelbase.GetSystemDefaultLCID
@ stdcall GetSystemPreferredUILanguages(long ptr wstr ptr) kernelbase.GetSystemPreferredUILanguages
@ stdcall GetThreadLocale() kernelbase.GetThreadLocale
@ stdcall GetThreadPreferredUILanguages(long ptr wstr ptr) kernelbase.GetThreadPreferredUILanguages
@ stdcall GetThreadUILanguage() kernelbase.GetThreadUILanguage
@ stub GetUILanguageInfo ;(long wstr wstr ptr ptr) kernelbase.GetUILanguageInfo
@ stdcall GetUserDefaultLangID() kernelbase.GetUserDefaultLangID
@ stdcall GetUserPreferredUILanguages(long ptr wstr ptr) kernelbase.GetUserPreferredUILanguages
@ stdcall GetUserDefaultLCID() kernelbase.GetUserDefaultLCID
@ stdcall IsNLSDefinedString(long long ptr long long) kernelbase.IsNLSDefinedString
@ stdcall IsValidCodePage(long) kernelbase.IsValidCodePage
@ stdcall IsValidLanguageGroup(long long) kernelbase.IsValidLanguageGroup
@ stdcall IsValidLocale(long long) kernelbase.IsValidLocale
@ stdcall LCMapStringEx(wstr long wstr long ptr long ptr ptr long) kernelbase.LCMapStringEx
@ stdcall LCMapStringW(long long wstr long ptr long) kernelbase.LCMapStringW
@ stub NlsCheckPolicy ;(wstr long) kernelbase.NlsCheckPolicy
@ stub NlsEventDataDescCreate   ;(ptr wstr wstr long) kernelbase.NlsEventDataDescCreate
@ stdcall NlsGetCacheUpdateCount() kernelbase.NlsGetCacheUpdateCount
@ stub NlsUpdateLocale ;(long long) kernelbase.NlsUpdateLocale
@ stub NlsUpdateSystemLocale    ;(long long) kernelbase.NlsUpdateSystemLocale
@ stub NlsWriteEtwEvent  ;(int64 ptr long ptr) kernelbase.NlsWriteEtwEvent
@ stdcall ResolveLocaleName(wstr wstr long) kernelbase.ResolveLocaleName
@ stdcall SetCalendarInfoW(long long long wstr) kernelbase.SetCalendarInfoW
@ stdcall SetLocaleInfoW(long long wstr) kernelbase.SetLocaleInfoW
@ stdcall SetThreadLocale(long) kernelbase.SetThreadLocale
@ stdcall VerLanguageNameA(long str long) kernelbase.VerLanguageNameA
@ stdcall VerLanguageNameW(long wstr long) kernelbase.VerLanguageNameW