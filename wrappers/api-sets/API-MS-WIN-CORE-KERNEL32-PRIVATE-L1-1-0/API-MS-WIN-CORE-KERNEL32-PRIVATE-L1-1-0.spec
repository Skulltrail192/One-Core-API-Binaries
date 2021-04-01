@ stdcall BaseSetLastNTError(long) kernelbase.BaseSetLastNTError
@ stdcall CheckElevation(wstr long ptr long long) kernelbase.CheckElevation
@ stdcall CheckElevationEnabled(ptr) kernelbase.CheckElevationEnabled
@ stdcall CompareCalendarDates(ptr ptr long) kernelbase.CompareCalendarDates
@ stdcall DosPathToSessionPathA(long str str) kernelbase.DosPathToSessionPathA
@ stdcall DosPathToSessionPathW(long wstr wstr) kernelbase.DosPathToSessionPathW
@ stdcall EnumerateLocalComputerNamesA(ptr long str ptr) kernelbase.EnumerateLocalComputerNamesA
@ stdcall EnumerateLocalComputerNamesW(ptr long wstr ptr) kernelbase.EnumerateLocalComputerNamesW
@ stdcall GetCalendarMonthsInYear(long long long) kernelbase.GetCalendarMonthsInYear
@ stdcall _lclose(long) kernelbase._lclose
@ stdcall _lcreat(str long) kernelbase._lcreat
@ stdcall _llseek(long long long) kernelbase._llseek
@ stdcall _lopen(str long) kernelbase._lopen
@ stdcall _lread(long ptr long) kernelbase._lread
@ stdcall _lwrite(long ptr long) kernelbase._lwrite
@ stdcall RegisterWaitForInputIdle(ptr) kernelbase.RegisterWaitForInputIdle
@ stdcall RemoveLocalAlternateComputerNameW(wstr long) kernelbase.RemoveLocalAlternateComputerNameW
@ stdcall SetLocalPrimaryComputerNameW(long long) kernelbase.SetLocalPrimaryComputerNameW
@ stdcall Wow64EnableWow64FsRedirection(long) kernelbase.Wow64EnableWow64FsRedirection