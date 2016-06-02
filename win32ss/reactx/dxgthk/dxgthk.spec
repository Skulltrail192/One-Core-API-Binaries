@ stdcall DriverEntry(ptr ptr)
@ stdcall CLIPOBJ_bEnum(ptr long ptr) win32k.CLIPOBJ_bEnum
@ stdcall CLIPOBJ_cEnumStart(ptr long long long long) win32k.CLIPOBJ_cEnumStart
@ stdcall EngAlphaBlend(ptr ptr ptr ptr ptr ptr ptr) win32k.EngAlphaBlend
@ stdcall EngAssociateSurface(ptr ptr long) win32k.EngAssociateSurface
@ stdcall EngBitBlt(ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr long) win32k.EngBitBlt
@ stdcall EngAcquireSemaphore(ptr) win32k.EngAcquireSemaphore
@ stdcall EngAllocMem(long long long) win32k.EngAllocMem
@ stdcall EngAllocUserMem(long long) win32k.EngAllocUserMem
@ stdcall EngBugCheckEx(long ptr ptr ptr ptr) NTOSKRNL.KeBugCheckEx
@ stdcall EngCopyBits(ptr ptr ptr ptr ptr ptr) win32k.EngCopyBits
@ stdcall EngCreateBitmap(long long long long ptr) win32k.EngCreateBitmap
@ stdcall EngCreateDeviceSurface(ptr long long long) win32k.EngCreateDeviceSurface
@ stdcall EngCreatePalette(long long long long long long) win32k.EngCreatePalette
@ stdcall EngCreateSemaphore() win32k.EngCreateSemaphore
@ stdcall EngDeletePalette(ptr) win32k.EngDeletePalette
@ stdcall EngDeleteSemaphore(ptr) win32k.EngDeleteSemaphore
@ stdcall EngDeleteSurface(ptr) win32k.EngDeleteSurface
@ stdcall EngDeviceIoControl(ptr long ptr long ptr long ptr) win32k.EngDeviceIoControl
@ stdcall EngFillPath(ptr ptr ptr ptr ptr long long) win32k.EngFillPath
@ stdcall EngFindImageProcAddress(ptr str) win32k.EngFindImageProcAddress
@ stdcall EngFreeMem(ptr) win32k.EngFreeMem
@ stdcall EngFreeUserMem(ptr) win32k.EngFreeUserMem
@ stdcall EngGradientFill(ptr ptr ptr ptr long ptr long ptr ptr long) win32k.EngGradientFill
@ stdcall EngLineTo(ptr ptr ptr long long long long ptr long) win32k.EngLineTo
@ stdcall EngLoadImage(wstr) win32k.EngLoadImage
@ stdcall EngLockSurface(ptr) win32k.EngLockSurface
@ stdcall EngPlgBlt(ptr ptr ptr ptr ptr ptr ptr ptr ptr ptr long) win32k.EngPlgBlt
@ stdcall EngReleaseSemaphore(ptr) win32k.EngReleaseSemaphore
@ stdcall EngSetLastError(long) win32k.EngSetLastError
@ stdcall EngStretchBlt(ptr ptr ptr ptr ptr long long ptr ptr ptr long) win32k.EngStretchBlt
@ stdcall EngStretchBltROP(ptr ptr ptr ptr ptr long long ptr ptr ptr long ptr long) win32k.EngStretchBltROP
@ stdcall EngStrokeAndFillPath(ptr ptr ptr ptr ptr ptr ptr ptr long long) win32k.EngStrokeAndFillPath
@ stdcall EngStrokePath(ptr ptr ptr ptr ptr ptr ptr long) win32k.EngStrokePath
@ stdcall EngTextOut(ptr ptr ptr ptr ptr ptr ptr ptr ptr long) win32k.EngTextOut
@ stdcall EngTransparentBlt(ptr ptr ptr ptr ptr ptr long long) win32k.EngTransparentBlt
@ stdcall EngUnloadImage(ptr) win32k.EngUnloadImage
@ stdcall EngUnlockSurface(ptr) win32k.EngUnlockSurface
@ stdcall PALOBJ_cGetColors(ptr long long ptr) win32k.PALOBJ_cGetColors
@ stdcall PATHOBJ_vGetBounds(ptr ptr) win32k.PATHOBJ_vGetBounds

#Vista Functions
@ stdcall EngCombineRgn(ptr ptr ptr long)
@ stdcall EngCreateRectRgn(long long long long)
@ stdcall EngDeleteRgn(ptr)
@ stdcall EngGetRgnData(ptr long ptr)
@ stdcall EngOffsetRgn(ptr long long)
@ stdcall EngSetRectRgn(ptr long long long long)
@ stdcall EngDxgkGetAdapterHandle(ptr long long long long) EngQueryW32kCddInterface
@ stdcall EngQueryW32kCddInterface(ptr long long long long)
