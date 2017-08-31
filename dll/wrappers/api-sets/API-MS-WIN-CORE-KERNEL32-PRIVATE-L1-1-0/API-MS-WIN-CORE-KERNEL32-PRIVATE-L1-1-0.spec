@ stdcall BaseSetLastNTError(long) kernel32.BaseSetLastNTError
@ stdcall CheckElevation(wstr long ptr long long) kernel32.CheckElevation
@ stdcall CheckElevationEnabled(ptr) kernel32.CheckElevationEnabled
@ stdcall CompareCalendarDates(ptr ptr long) kernel32.CompareCalendarDates
@ stdcall DosPathToSessionPathA(long str str) kernel32.DosPathToSessionPathA
@ stdcall DosPathToSessionPathW(long wstr wstr) kernel32.DosPathToSessionPathW
@ stdcall EnumerateLocalComputerNamesA(ptr long str ptr) kernel32.EnumerateLocalComputerNamesA
@ stdcall EnumerateLocalComputerNamesW(ptr long wstr ptr) kernel32.EnumerateLocalComputerNamesW
@ stdcall GetCalendarMonthsInYear(long long long) kernel32.GetCalendarMonthsInYear
@ stdcall _lclose(long) kernel32._lclose
@ stdcall _lcreat(str long) kernel32._lcreat
@ stdcall _llseek(long long long) kernel32._llseek
@ stdcall _lopen(str long) kernel32._lopen
@ stdcall _lread(long ptr long) kernel32._lread
@ stdcall _lwrite(long ptr long) kernel32._lwrite
@ stdcall RegisterWaitForInputIdle(ptr) kernel32.RegisterWaitForInputIdle
@ stdcall RemoveLocalAlternateComputerNameW(wstr long) kernel32.RemoveLocalAlternateComputerNameW
@ stdcall SetLocalPrimaryComputerNameW(long long) kernel32.SetLocalPrimaryComputerNameW
@ stdcall Wow64EnableWow64FsRedirection(long) kernel32.Wow64EnableWow64FsRedirection