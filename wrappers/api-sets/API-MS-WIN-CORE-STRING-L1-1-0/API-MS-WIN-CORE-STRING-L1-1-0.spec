@ stdcall CompareStringEx(wstr long wstr long wstr long ptr ptr long)
@ stdcall CompareStringW(long long wstr long wstr long) kernelbase.CompareStringW
@ stdcall CompareStringOrdinal(wstr long wstr long long) kernelbase.CompareStringOrdinal
@ stdcall FoldStringW(long wstr long ptr long)
@ stdcall GetStringTypeExW(long long wstr long ptr)
@ stdcall GetStringTypeW(long wstr long ptr) kernelbase.GetStringTypeW
@ stdcall MultiByteToWideChar(long long str long ptr long) kernelbase.MultiByteToWideChar
@ stdcall WideCharToMultiByte(long long wstr long ptr long ptr ptr) kernelbase.WideCharToMultiByte