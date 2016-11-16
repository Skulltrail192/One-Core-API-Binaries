@ stub __conio_common_vcprintf ; ; ; ; ; ; ;(ptr ptr ptr ptr ptr) ucrtbase.__conio_common_vcprintf
@ stub __conio_common_vcprintf_p ; ; ; ; ; ; ;(ptr ptr ptr ptr ptr) ucrtbase.__conio_common_vcprintf_p
@ stub __conio_common_vcprintf_s ; ; ; ; ; ; ;(ptr ptr ptr ptr ptr) ucrtbase.__conio_common_vcprintf_s
@ stub __conio_common_vcscanf ; ; ; ; ; ; ;(ptr ptr ptr ptr ptr) ucrtbase.__conio_common_vcscanf
@ stub __conio_common_vcwprintf ; ; ; ; ; ; ;(ptr ptr ptr ptr ptr) ucrtbase.__conio_common_vcwprintf
@ stub __conio_common_vcwprintf_p ; ; ; ; ; ; ;(ptr ptr ptr ptr ptr) ucrtbase.__conio_common_vcwprintf_p
@ stub __conio_common_vcwprintf_s ; ; ; ; ; ; ;(ptr ptr ptr ptr ptr) ucrtbase.__conio_common_vcwprintf_s
@ stub __conio_common_vcwscanf ; ; ; ; ; ; ;(ptr ptr ptr ptr ptr) ucrtbase.__conio_common_vcwscanf
@ stub _cgets ; ; ; ; ; ; ;(ptr) ucrtbase._cgets
@ stub _cgets_s ; ; ; ; ; ; ;(ptr ptr ptr) ucrtbase._cgets_s
@ stub _cgetws ; ; ; ; ; ; ;(wstr) ucrtbase._cgetws
@ stub _cgetws_s ; ; ; ; ; ; ;(wstr long long) ucrtbase._cgetws_s
@ stub _cputs ; ; ; ; ; ; ;(str) ucrtbase._cputs
@ stub _cputws ; ; ; ; ; ; ;(wstr) ucrtbase._cputws
@ stub _getch ; ; ; ; ; ; ;() ucrtbase._getch
@ stub _getch_nolock ; ; ; ; ; ; ;() ucrtbase._getch_nolock
@ stub _getche ; ; ; ; ; ; ;() ucrtbase._getche
@ stub _getche_nolock ; ; ; ; ; ; ;() ucrtbase._getche_nolock
@ stub _getwch ; ; ; ; ; ; ;() ucrtbase._getwch
@ stub _getwch_nolock ; ; ; ; ; ; ;() ucrtbase._getwch_nolock
@ stub _getwche ; ; ; ; ; ; ;() ucrtbase._getwche
@ stub _getwche_nolock ; ; ; ; ; ; ;() ucrtbase._getwche_nolock
@ stub _putch ; ; ; ; ; ; ;(long) ucrtbase._putch
@ stub _putch_nolock ; ; ; ; ; ; ;(long) ucrtbase._putch_nolock
@ stub _putwch ; ; ; ; ; ; ;(long) ucrtbase._putwch
@ stub _putwch_nolock ; ; ; ; ; ; ;(long) ucrtbase._putwch_nolock
@ stub _ungetch ; ; ; ; ; ; ;(long) ucrtbase._ungetch
@ stub _ungetch_nolock ; ; ; ; ; ; ;(long) ucrtbase._ungetch_nolock
@ stub _ungetwch ; ; ; ; ; ; ;(long) ucrtbase._ungetwch
@ stub _ungetwch_nolock ; ; ; ; ; ; ;(long) ucrtbase._ungetwch_nolock
@ stub __toascii ; ; ; ; ; ;(long) ucrtbase.__toascii
@ stub _atodbl ; ; ; ; ; ;(ptr str) ucrtbase._atodbl
@ stub _atodbl_l ; ; ; ; ; ;(ptr str ptr) ucrtbase._atodbl_l
@ stub _atof_l ; ; ; ; ; ;(str ptr) ucrtbase._atof_l
@ stub _atoflt ; ; ; ; ; ;(ptr str) ucrtbase._atoflt
@ stub _atoflt_l ; ; ; ; ; ;(ptr str ptr) ucrtbase._atoflt_l
@ stub -ret64 _atoi64 ; ; ; ; ; ;(str) ucrtbase._atoi64
@ stub _atoi64_l ; ; ; ; ; ;(str long) ucrtbase._atoi64_l
@ stub _atoi_l ; ; ; ; ; ;(str ptr) ucrtbase._atoi_l
@ stub _atol_l ; ; ; ; ; ;(str long) ucrtbase._atol_l
@ stub _atoldbl ; ; ; ; ; ;(ptr str) ucrtbase._atoldbl
@ stub _atoldbl_l ; ; ; ; ; ;(ptr str long) ucrtbase._atoldbl_l
@ stub -ret64 _atoll_l ; ; ; ; ; ;(str ptr) ucrtbase._atoll_l
@ stub _ecvt ; ; ; ; ; ;(double long ptr ptr) ucrtbase._ecvt
@ stub _ecvt_s ; ; ; ; ; ;(str long double long ptr ptr) ucrtbase._ecvt_s
@ stub _fcvt ; ; ; ; ; ;(double long ptr ptr) ucrtbase._fcvt
@ stub _fcvt_s ; ; ; ; ; ;(ptr long double long ptr ptr) ucrtbase._fcvt_s
@ stub _gcvt ; ; ; ; ; ;(double long str) ucrtbase._gcvt
@ stub _gcvt_s ; ; ; ; ; ;(ptr long  double long) ucrtbase._gcvt_s
@ stub _i64toa ; ; ; ; ; ;(int64 ptr long) ucrtbase._i64toa
@ stub _i64toa_s ; ; ; ; ; ;(int64 ptr long long) ucrtbase._i64toa_s
@ stub _i64tow ; ; ; ; ; ;(int64 ptr long) ucrtbase._i64tow
@ stub _i64tow_s ; ; ; ; ; ;(int64 ptr long long) ucrtbase._i64tow_s
@ stub _itoa ; ; ; ; ; ;(long ptr long) ucrtbase._itoa
@ stub _itoa_s ; ; ; ; ; ;(long ptr long long) ucrtbase._itoa_s
@ stub _itow ; ; ; ; ; ;(long ptr long) ucrtbase._itow
@ stub _itow_s ; ; ; ; ; ;(long ptr long long) ucrtbase._itow_s
@ stub _ltoa ; ; ; ; ; ;(long ptr long) ucrtbase._ltoa
@ stub _ltoa_s ; ; ; ; ; ;(long ptr long long) ucrtbase._ltoa_s
@ stub _ltow ; ; ; ; ; ;(long ptr long) ucrtbase._ltow
@ stub _ltow_s ; ; ; ; ; ;(long ptr long long) ucrtbase._ltow_s
@ stub _strtod_l ; ; ; ; ; ;(str ptr ptr) ucrtbase._strtod_l
@ stub _strtof_l ; ; ; ; ; ;(str ptr ptr) ucrtbase._strtof_l
@ stub -ret64 _strtoi64 ; ; ; ; ; ;(str ptr long) ucrtbase._strtoi64
@ stub -ret64 _strtoi64_l ; ; ; ; ; ;(str ptr long ptr) ucrtbase._strtoi64_l
@ stub _strtoimax_l ; ; ; ; ; ;(str str long long) ucrtbase._strtoimax_l
@ stub _strtol_l ; ; ; ; ; ;(str ptr long ptr) ucrtbase._strtol_l
@ stub _strtold_l ; ; ; ; ; ;(str str long) ucrtbase._strtold_l
@ stub -ret64 _strtoll_l ; ; ; ; ; ;(str ptr long ptr) ucrtbase._strtoll_l
@ stub -ret64 _strtoui64 ; ; ; ; ; ;(str ptr long) ucrtbase._strtoui64
@ stub -ret64 _strtoui64_l ; ; ; ; ; ;(str ptr long ptr) ucrtbase._strtoui64_l
@ stub _strtoul_l ; ; ; ; ; ;(str ptr long ptr) ucrtbase._strtoul_l
@ stub -ret64 _strtoull_l ; ; ; ; ; ;(str ptr long ptr) ucrtbase._strtoull_l
@ stub _strtoumax_l ; ; ; ; ; ;(str str long long) ucrtbase._strtoumax_l
@ stub _ui64toa ; ; ; ; ; ;(int64 ptr long) ucrtbase._ui64toa
@ stub _ui64toa_s ; ; ; ; ; ;(int64 ptr long long) ucrtbase._ui64toa_s
@ stub _ui64tow ; ; ; ; ; ;(int64 ptr long) ucrtbase._ui64tow
@ stub _ui64tow_s ; ; ; ; ; ;(int64 ptr long long) ucrtbase._ui64tow_s
@ stub _ultoa ; ; ; ; ; ;(long ptr long) ucrtbase._ultoa
@ stub _ultoa_s ; ; ; ; ; ;(long ptr long long) ucrtbase._ultoa_s
@ stub _ultow ; ; ; ; ; ;(long ptr long) ucrtbase._ultow
@ stub _ultow_s ; ; ; ; ; ;(long ptr long long) ucrtbase._ultow_s
@ stub _wcstod_l ; ; ; ; ; ;(wstr ptr) ucrtbase._wcstod_l
@ stub _wcstof_l ; ; ; ; ; ;(wstr ptr ptr) ucrtbase._wcstof_l
@ stub -ret64 _wcstoi64 ; ; ; ; ; ;(wstr ptr long) ucrtbase._wcstoi64
@ stub -ret64 _wcstoi64_l ; ; ; ; ; ;(wstr ptr long ptr) ucrtbase._wcstoi64_l
@ stub _wcstoimax_l ; ; ; ; ; ;(wstr wstr long long) ucrtbase._wcstoimax_l
@ stub _wcstol_l ; ; ; ; ; ;(wstr ptr long ptr) ucrtbase._wcstol_l
@ stub _wcstold_l ; ; ; ; ; ;(wstr wstr long) ucrtbase._wcstold_l
@ stub -ret64 _wcstoll_l ; ; ; ; ; ;(wstr ptr long ptr) ucrtbase._wcstoll_l
@ stub _wcstombs_l ; ; ; ; ; ;(ptr ptr long ptr) ucrtbase._wcstombs_l
@ stub _wcstombs_s_l ; ; ; ; ; ;(ptr ptr long wstr long ptr) ucrtbase._wcstombs_s_l
@ stub -ret64 _wcstoui64 ; ; ; ; ; ;(wstr ptr long) ucrtbase._wcstoui64
@ stub -ret64 _wcstoui64_l ; ; ; ; ; ;(wstr ptr long ptr) ucrtbase._wcstoui64_l
@ stub _wcstoul_l ; ; ; ; ; ;(wstr ptr long ptr) ucrtbase._wcstoul_l
@ stub _wcstoull_l ; ; ; ; ; ;(wstr wstr long long) ucrtbase._wcstoull_l
@ stub _wcstoumax_l ; ; ; ; ; ;(wstr wstr long long) ucrtbase._wcstoumax_l
@ stub _wctomb_l ; ; ; ; ; ;(ptr long ptr) ucrtbase._wctomb_l
@ stub _wctomb_s_l ; ; ; ; ; ;(ptr ptr long long ptr) ucrtbase._wctomb_s_l
@ stub _wtof ; ; ; ; ; ;(wstr) ucrtbase._wtof
@ stub _wtof_l ; ; ; ; ; ;(wstr ptr) ucrtbase._wtof_l
@ stub _wtoi ; ; ; ; ; ;(wstr) ucrtbase._wtoi
@ stub -ret64 _wtoi64 ; ; ; ; ; ;(wstr) ucrtbase._wtoi64
@ stub -ret64 _wtoi64_l ; ; ; ; ; ;(wstr ptr) ucrtbase._wtoi64_l
@ stub _wtoi_l ; ; ; ; ; ;(wstr ptr) ucrtbase._wtoi_l
@ stub _wtol ; ; ; ; ; ;(wstr) ucrtbase._wtol
@ stub _wtol_l ; ; ; ; ; ;(wstr ptr) ucrtbase._wtol_l
@ stub -ret64 _wtoll ; ; ; ; ; ;(wstr) ucrtbase._wtoll
@ stub -ret64 _wtoll_l ; ; ; ; ; ;(wstr ptr) ucrtbase._wtoll_l
@ stub atof ; ; ; ; ; ;(str) ucrtbase.atof
@ stub atoi ; ; ; ; ; ;(str) ucrtbase.atoi
@ stub atol ; ; ; ; ; ;(str) ucrtbase.atol
@ stub -ret64 atoll ; ; ; ; ; ;(str) ucrtbase.atoll
@ stub btowc ; ; ; ; ; ;(long) ucrtbase.btowc
@ stub c16rtomb ; ; ; ; ; ;(str str ptr) ucrtbase.c16rtomb
@ stub c32rtomb ; ; ; ; ; ;(str str ptr) ucrtbase.c32rtomb
@ stub mbrtoc16 ; ; ; ; ; ;(str str long ptr) ucrtbase.mbrtoc16
@ stub mbrtoc32 ; ; ; ; ; ;(str str long ptr) ucrtbase.mbrtoc32
@ stub mbrtowc ; ; ; ; ; ;(ptr str long ptr) ucrtbase.mbrtowc
@ stub mbsrtowcs ; ; ; ; ; ;(ptr ptr long ptr) ucrtbase.mbsrtowcs
@ stub mbsrtowcs_s ; ; ; ; ; ;(ptr ptr long ptr long ptr) ucrtbase.mbsrtowcs_s
@ stub mbstowcs ; ; ; ; ; ;(ptr str long) ucrtbase.mbstowcs
@ stub mbstowcs_s ; ; ; ; ; ;(ptr ptr long str long) ucrtbase.mbstowcs_s
@ stub mbtowc ; ; ; ; ; ;(ptr str long) ucrtbase.mbtowc
@ stub strtod ; ; ; ; ; ;(str ptr) ucrtbase.strtod
@ stub strtof ; ; ; ; ; ;(str ptr) ucrtbase.strtof
@ stub strtoimax ; ; ; ; ; ;(str str long) ucrtbase.strtoimax
@ stub strtol ; ; ; ; ; ;(str ptr long) ucrtbase.strtol
@ stub strtold ; ; ; ; ; ;(str str) ucrtbase.strtold
@ stub -ret64 strtoll ; ; ; ; ; ;(str ptr long) ucrtbase.strtoll
@ stub strtoul ; ; ; ; ; ;(str ptr long) ucrtbase.strtoul
@ stub -ret64 strtoull ; ; ; ; ; ;(str ptr long) ucrtbase.strtoull
@ stub strtoumax ; ; ; ; ; ;(str str long) ucrtbase.strtoumax
@ stub wcrtomb ; ; ; ; ; ;(ptr long ptr) ucrtbase.wcrtomb
@ stub wcrtomb_s ; ; ; ; ; ;(ptr str wstr ptr) ucrtbase.wcrtomb_s
@ stub wcsrtombs ; ; ; ; ; ;(ptr ptr long ptr) ucrtbase.wcsrtombs
@ stub wcsrtombs_s ; ; ; ; ; ;(ptr ptr long ptr long ptr) ucrtbase.wcsrtombs_s
@ stub wcstod ; ; ; ; ; ;(wstr ptr) ucrtbase.wcstod
@ stub wcstof ; ; ; ; ; ;(ptr ptr) ucrtbase.wcstof
@ stub wcstoimax ; ; ; ; ; ;(wstr wstr long) ucrtbase.wcstoimax
@ stub wcstol ; ; ; ; ; ;(wstr ptr long) ucrtbase.wcstol
@ stub wcstold ; ; ; ; ; ;(wstr wstr) ucrtbase.wcstold
@ stub -ret64 wcstoll ; ; ; ; ; ;(wstr ptr long) ucrtbase.wcstoll
@ stub wcstombs ; ; ; ; ; ;(ptr ptr long) ucrtbase.wcstombs
@ stub wcstombs_s ; ; ; ; ; ;(ptr ptr long wstr long) ucrtbase.wcstombs_s
@ stub wcstoul ; ; ; ; ; ;(wstr ptr long) ucrtbase.wcstoul
@ stub wcstoull ; ; ; ; ; ;(wstr wstr long) ucrtbase.wcstoull
@ stub wcstoumax ; ; ; ; ; ;(wstr wstr long) ucrtbase.wcstoumax
@ stub wctob ; ; ; ; ; ;(long) ucrtbase.wctob
@ stub wctomb ; ; ; ; ; ;(ptr long) ucrtbase.wctomb
@ stub wctomb_s ; ; ; ; ; ;(ptr ptr long long) ucrtbase.wctomb_s
@ stub wctrans ; ; ; ; ; ;(str) ucrtbase.wctrans
@ stub __p__environ ; ; ; ; ;() ucrtbase.__p__environ
@ stub __p__wenviron ; ; ; ; ;() ucrtbase.__p__wenviron
@ stub _dupenv_s ; ; ; ; ;(ptr ptr str) ucrtbase._dupenv_s
@ stub _putenv ; ; ; ; ;(str) ucrtbase._putenv
@ stub _putenv_s ; ; ; ; ;(str str) ucrtbase._putenv_s
@ stub _searchenv ; ; ; ; ;(str str ptr) ucrtbase._searchenv
@ stub _searchenv_s ; ; ; ; ;(str str ptr long) ucrtbase._searchenv_s
@ stub _wdupenv_s ; ; ; ; ;(ptr ptr wstr) ucrtbase._wdupenv_s
@ stub _wgetcwd ; ; ; ; ;(wstr long) ucrtbase._wgetcwd
@ stub _wgetdcwd ; ; ; ; ;(long wstr long) ucrtbase._wgetdcwd
@ stub _wgetenv ; ; ; ; ;(wstr) ucrtbase._wgetenv
@ stub _wgetenv_s ; ; ; ; ;(ptr ptr long wstr) ucrtbase._wgetenv_s
@ stub _wputenv ; ; ; ; ;(wstr) ucrtbase._wputenv
@ stub _wputenv_s ; ; ; ; ;(wstr wstr) ucrtbase._wputenv_s
@ stub _wsearchenv ; ; ; ; ;(wstr wstr ptr) ucrtbase._wsearchenv
@ stub _wsearchenv_s ; ; ; ; ;(wstr wstr ptr long) ucrtbase._wsearchenv_s
@ stub getenv ; ; ; ; ;(str) ucrtbase.getenv
@ stub getenv_s ; ; ; ; ;(ptr ptr long str) ucrtbase.getenv_s
@ stub _access ; ; ; ;(str long) ucrtbase._access
@ stub _access_s ; ; ; ;(str long) ucrtbase._access_s
@ stub _chdir ; ; ; ;(str) ucrtbase._chdir
@ stub _chdrive ; ; ; ;(long) ucrtbase._chdrive
@ stub _chmod ; ; ; ;(str long) ucrtbase._chmod
@ stub _findclose ; ; ; ;(long) ucrtbase._findclose
@ stub _findfirst32 ; ; ; ;(str ptr) ucrtbase._findfirst32
@ stub _findfirst32i64 ; ; ; ;(str ptr) ucrtbase._findfirst32i64
@ stub _findfirst64 ; ; ; ;(str ptr) ucrtbase._findfirst64
@ stub _findfirst64i32 ; ; ; ;(str ptr) ucrtbase._findfirst64i32
@ stub _findnext32 ; ; ; ;(long ptr) ucrtbase._findnext32
@ stub _findnext32i64 ; ; ; ;(ptr ptr) ucrtbase._findnext32i64
@ stub _findnext64 ; ; ; ;(long ptr) ucrtbase._findnext64
@ stub _findnext64i32 ; ; ; ;(long ptr) ucrtbase._findnext64i32
@ stub _fstat32 ; ; ; ;(long ptr) ucrtbase._fstat32
@ stub _fstat32i64 ; ; ; ;(long ptr) ucrtbase._fstat32i64
@ stub _fstat64 ; ; ; ;(long ptr) ucrtbase._fstat64
@ stub _fstat64i32 ; ; ; ;(long ptr) ucrtbase._fstat64i32
@ stub _fullpath ; ; ; ;(ptr str long) ucrtbase._fullpath
@ stub _getdiskfree ; ; ; ;(long ptr) ucrtbase._getdiskfree
@ stub _getdrive ; ; ; ;() ucrtbase._getdrive
@ stub _getdrives ; ; ; ;() ucrtbase._getdrives
@ stub _lock_file ; ; ; ;(ptr) ucrtbase._lock_file
@ stub _makepath ; ; ; ;(ptr str str str str) ucrtbase._makepath
@ stub _makepath_s ; ; ; ;(ptr long str str str str) ucrtbase._makepath_s
@ stub _mkdir ; ; ; ;(str) ucrtbase._mkdir
@ stub _rmdir ; ; ; ;(str) ucrtbase._rmdir
@ stub _splitpath ; ; ; ;(str ptr ptr ptr ptr) ucrtbase._splitpath
@ stub _splitpath_s ; ; ; ;(str ptr long ptr long ptr long ptr long) ucrtbase._splitpath_s
@ stub _stat32 ; ; ; ;(str ptr) ucrtbase._stat32
@ stub _stat32i64 ; ; ; ;(str ptr) ucrtbase._stat32i64
@ stub _stat64 ; ; ; ;(str ptr) ucrtbase._stat64
@ stub _stat64i32 ; ; ; ;(str ptr) ucrtbase._stat64i32
@ stub _umask ; ; ; ;(long) ucrtbase._umask
@ stub _umask_s ; ; ; ;(long long) ucrtbase._umask_s
@ stub _unlink ; ; ; ;(str) ucrtbase._unlink
@ stub _unlock_file ; ; ; ;(ptr) ucrtbase._unlock_file
@ stub _waccess ; ; ; ;(wstr long) ucrtbase._waccess
@ stub _waccess_s ; ; ; ;(wstr long) ucrtbase._waccess_s
@ stub _wchdir ; ; ; ;(wstr) ucrtbase._wchdir
@ stub _wchmod ; ; ; ;(wstr long) ucrtbase._wchmod
@ stub _wfindfirst32 ; ; ; ;(wstr ptr) ucrtbase._wfindfirst32
@ stub _wfindfirst32i64 ; ; ; ;(wstr ptr) ucrtbase._wfindfirst32i64
@ stub _wfindfirst64 ; ; ; ;(wstr ptr) ucrtbase._wfindfirst64
@ stub _wfindfirst64i32 ; ; ; ;(wstr ptr) ucrtbase._wfindfirst64i32
@ stub _wfindnext32 ; ; ; ;(ptr ptr) ucrtbase._wfindnext32
@ stub _wfindnext32i64 ; ; ; ;(ptr ptr) ucrtbase._wfindnext32i64
@ stub _wfindnext64 ; ; ; ;(long ptr) ucrtbase._wfindnext64
@ stub _wfindnext64i32 ; ; ; ;(long ptr) ucrtbase._wfindnext64i32
@ stub _wfullpath ; ; ; ;(ptr wstr long) ucrtbase._wfullpath
@ stub _wmakepath ; ; ; ;(ptr wstr wstr wstr wstr) ucrtbase._wmakepath
@ stub _wmakepath_s ; ; ; ;(ptr long wstr wstr wstr wstr) ucrtbase._wmakepath_s
@ stub _wmkdir ; ; ; ;(wstr) ucrtbase._wmkdir
@ stub _wremove ; ; ; ;(wstr) ucrtbase._wremove
@ stub _wrename ; ; ; ;(wstr wstr) ucrtbase._wrename
@ stub _wrmdir ; ; ; ;(wstr) ucrtbase._wrmdir
@ stub _wsplitpath ; ; ; ;(wstr ptr ptr ptr ptr) ucrtbase._wsplitpath
@ stub _wsplitpath_s ; ; ; ;(wstr ptr long ptr long ptr long ptr long) ucrtbase._wsplitpath_s
@ stub _wstat32 ; ; ; ;(wstr ptr) ucrtbase._wstat32
@ stub _wstat32i64 ; ; ; ;(wstr ptr) ucrtbase._wstat32i64
@ stub _wstat64 ; ; ; ;(wstr ptr) ucrtbase._wstat64
@ stub _wstat64i32 ; ; ; ;(wstr ptr) ucrtbase._wstat64i32
@ stub _wunlink ; ; ; ;(wstr) ucrtbase._wunlink
@ stub remove ; ; ; ;(str) ucrtbase.remove
@ stub rename ; ; ; ;(str str) ucrtbase.rename
@ stub _aligned_free ; ; ; ;(ptr) ucrtbase._aligned_free
@ stub _aligned_malloc ; ; ; ;(long long) ucrtbase._aligned_malloc
@ stub _aligned_msize ; ; ; ;(ptr long long) ucrtbase._aligned_msize
@ stub _aligned_offset_malloc ; ; ; ;(long long long) ucrtbase._aligned_offset_malloc
@ stub _aligned_offset_realloc ; ; ; ;(ptr long long long) ucrtbase._aligned_offset_realloc
@ stub _aligned_offset_recalloc ; ; ; ;(ptr long long long long) ucrtbase._aligned_offset_recalloc
@ stub _aligned_realloc ; ; ; ;(ptr long long) ucrtbase._aligned_realloc
@ stub _aligned_recalloc ; ; ; ;(ptr long long long) ucrtbase._aligned_recalloc
@ stub _callnewh ; ; ; ;(long) ucrtbase._callnewh
@ stub _calloc_base ; ; ; ;(long long) ucrtbase._calloc_base
@ stub _expand ; ; ; ;(ptr long) ucrtbase._expand
@ stub _free_base ; ; ; ;(ptr) ucrtbase._free_base
@ stub _get_heap_handle ; ; ; ;() ucrtbase._get_heap_handle
@ stub _heapchk ; ; ; ;() ucrtbase._heapchk
@ stub _heapmin ; ; ; ;() ucrtbase._heapmin
@ stub _heapwalk ; ; ; ;(ptr) ucrtbase._heapwalk
@ stub _malloc_base ; ; ; ;(long) ucrtbase._malloc_base
@ stub _msize ; ; ; ;(ptr) ucrtbase._msize
@ stub _query_new_handler ; ; ; ;() ucrtbase._query_new_handler
@ stub _query_new_mode ; ; ; ;() ucrtbase._query_new_mode
@ stub _realloc_base ; ; ; ;(ptr long) ucrtbase._realloc_base
@ stub _recalloc ; ; ; ;(ptr long long) ucrtbase._recalloc
@ stub _set_new_mode ; ; ; ;(long) ucrtbase._set_new_mode
@ stub calloc ; ; ; ;(long long) ucrtbase.calloc
@ stub free ; ; ; ;(ptr) ucrtbase.free
@ stub malloc ; ; ; ;(long) ucrtbase.malloc
@ stub realloc ; ; ; ;(ptr long) ucrtbase.realloc
@ stub ___lc_codepage_func ; ; ; ;() ucrtbase.___lc_codepage_func
@ stub ___lc_collate_cp_func ; ; ; ;() ucrtbase.___lc_collate_cp_func
@ stub ___lc_locale_name_func ; ; ; ;() ucrtbase.___lc_locale_name_func
@ stub ___mb_cur_max_func ; ; ; ;() ucrtbase.___mb_cur_max_func
@ stub ___mb_cur_max_l_func ; ; ; ;(ptr) ucrtbase.___mb_cur_max_l_func
@ stub __initialize_lconv_for_unsigned_char ; ; ; ;() ucrtbase.__initialize_lconv_for_unsigned_char
@ stub __pctype_func ; ; ; ;() ucrtbase.__pctype_func
@ stub __pwctype_func ; ; ; ;() ucrtbase.__pwctype_func
@ stub _configthreadlocale ; ; ; ;(long) ucrtbase._configthreadlocale
@ stub _create_locale ; ; ; ;(long str) ucrtbase._create_locale
@ stub _free_locale ; ; ; ;(ptr) ucrtbase._free_locale
@ stub _get_current_locale ; ; ; ;() ucrtbase._get_current_locale
@ stub _getmbcp ; ; ; ;() ucrtbase._getmbcp
@ stub _lock_locales ; ; ; ;() ucrtbase._lock_locales
@ stub _setmbcp ; ; ; ;(long) ucrtbase._setmbcp
@ stub _unlock_locales ; ; ; ;() ucrtbase._unlock_locales
@ stub _wcreate_locale ; ; ; ;(long wstr) ucrtbase._wcreate_locale
@ stub _wsetlocale ; ; ; ;(long wstr) ucrtbase._wsetlocale
@ stub localeconv ; ; ; ;() ucrtbase.localeconv
@ stub setlocale ; ; ; ;(long str) ucrtbase.setlocale
@ stub -arch=i386 _CIacos ; ; ;() ucrtbase._CIacos
@ stub -arch=i386 _CIasin ; ; ;() ucrtbase._CIasin
@ stub -arch=i386 _CIatan ; ; ;() ucrtbase._CIatan
@ stub -arch=i386 _CIatan2 ; ; ;() ucrtbase._CIatan2
@ stub -arch=i386 _CIcos ; ; ;() ucrtbase._CIcos
@ stub -arch=i386 _CIcosh ; ; ;() ucrtbase._CIcosh
@ stub -arch=i386 _CIexp ; ; ;() ucrtbase._CIexp
@ stub -arch=i386 _CIfmod ; ; ;() ucrtbase._CIfmod
@ stub -arch=i386 _CIlog ; ; ;() ucrtbase._CIlog
@ stub -arch=i386 _CIlog10 ; ; ;() ucrtbase._CIlog10
@ stub -arch=i386 _CIpow ; ; ;() ucrtbase._CIpow
@ stub -arch=i386 _CIsin ; ; ;() ucrtbase._CIsin
@ stub -arch=i386 _CIsinh ; ; ;() ucrtbase._CIsinh
@ stub -arch=i386 _CIsqrt ; ; ;() ucrtbase._CIsqrt
@ stub -arch=i386 _CItan ; ; ;() ucrtbase._CItan
@ stub -arch=i386 _CItanh ; ; ;() ucrtbase._CItanh
@ stub _Cbuild ; ; ;(long long long) ucrtbase._Cbuild
@ stub _Cmulcc ; ; ;(double double) ucrtbase._Cmulcc
@ stub _Cmulcr ; ; ;(long) ucrtbase._Cmulcr
@ stub _FCbuild ; ; ;(long long) ucrtbase._FCbuild
@ stub _FCmulcc ; ; ;(long long long long) ucrtbase._FCmulcc
@ stub _FCmulcr ; ; ;(long long long long) ucrtbase._FCmulcr
@ stub _LCbuild ; ; ;(long double long double) ucrtbase._LCbuild
@ stub _LCmulcc ; ; ;(long long) ucrtbase._LCmulcc
@ stub _LCmulcr ; ; ;(long str long long long double) ucrtbase._LCmulcr
@ stub -arch=i386 __libm_sse2_acos ; ; ;() ucrtbase.__libm_sse2_acos
@ stub -arch=i386 __libm_sse2_acosf ; ; ;() ucrtbase.__libm_sse2_acosf
@ stub -arch=i386 __libm_sse2_asin ; ; ;() ucrtbase.__libm_sse2_asin
@ stub -arch=i386 __libm_sse2_asinf ; ; ;() ucrtbase.__libm_sse2_asinf
@ stub -arch=i386 __libm_sse2_atan ; ; ;() ucrtbase.__libm_sse2_atan
@ stub -arch=i386 __libm_sse2_atan2 ; ; ;() ucrtbase.__libm_sse2_atan2
@ stub -arch=i386 __libm_sse2_atanf ; ; ;() ucrtbase.__libm_sse2_atanf
@ stub -arch=i386 __libm_sse2_cos ; ; ;() ucrtbase.__libm_sse2_cos
@ stub -arch=i386 __libm_sse2_cosf ; ; ;() ucrtbase.__libm_sse2_cosf
@ stub -arch=i386 __libm_sse2_exp ; ; ;() ucrtbase.__libm_sse2_exp
@ stub -arch=i386 __libm_sse2_expf ; ; ;() ucrtbase.__libm_sse2_expf
@ stub -arch=i386 __libm_sse2_log ; ; ;() ucrtbase.__libm_sse2_log
@ stub -arch=i386 __libm_sse2_log10 ; ; ;() ucrtbase.__libm_sse2_log10
@ stub -arch=i386 __libm_sse2_log10f ; ; ;() ucrtbase.__libm_sse2_log10f
@ stub -arch=i386 __libm_sse2_logf ; ; ;() ucrtbase.__libm_sse2_logf
@ stub -arch=i386 __libm_sse2_pow ; ; ;() ucrtbase.__libm_sse2_pow
@ stub -arch=i386 __libm_sse2_powf ; ; ;() ucrtbase.__libm_sse2_powf
@ stub -arch=i386 __libm_sse2_sin ; ; ;() ucrtbase.__libm_sse2_sin
@ stub -arch=i386 __libm_sse2_sinf ; ; ;() ucrtbase.__libm_sse2_sinf
@ stub -arch=i386 __libm_sse2_tan ; ; ;() ucrtbase.__libm_sse2_tan
@ stub -arch=i386 __libm_sse2_tanf ; ; ;() ucrtbase.__libm_sse2_tanf
@ stub __setusermatherr ; ; ;(ptr) ucrtbase.__setusermatherr
@ stub _cabs ; ; ;(long) ucrtbase._cabs
@ stub _chgsign ; ; ;(double) ucrtbase._chgsign
@ stub _chgsignf ; ; ;(float) ucrtbase._chgsignf
@ stub _copysign ; ; ;(double double) ucrtbase._copysign
@ stub _copysignf ; ; ;(float float) ucrtbase._copysignf
@ stub _d_int ; ; ;() ucrtbase._d_int
@ stub _dclass ; ; ;(double) ucrtbase._dclass
@ stub _dexp ; ; ;() ucrtbase._dexp
@ stub _dlog ; ; ;() ucrtbase._dlog
@ stub _dnorm ; ; ;() ucrtbase._dnorm
@ stub _dpcomp ; ; ;(double double) ucrtbase._dpcomp
@ stub _dpoly ; ; ;() ucrtbase._dpoly
@ stub _dscale ; ; ;() ucrtbase._dscale
@ stub _dsign ; ; ;(double) ucrtbase._dsign
@ stub _dsin ; ; ;() ucrtbase._dsin
@ stub _dtest ; ; ;(ptr) ucrtbase._dtest
@ stub _dunscale ; ; ;() ucrtbase._dunscale
@ stub _except1 ; ; ;() ucrtbase._except1
@ stub _fd_int ; ; ;() ucrtbase._fd_int
@ stub _fdclass ; ; ;(float) ucrtbase._fdclass
@ stub _fdexp ; ; ;() ucrtbase._fdexp
@ stub _fdlog ; ; ;() ucrtbase._fdlog
@ stub _fdnorm ; ; ;() ucrtbase._fdnorm
@ stub _fdopen ; ; ;(long str) ucrtbase._fdopen
@ stub _fdpcomp ; ; ;(float float) ucrtbase._fdpcomp
@ stub _fdpoly ; ; ;() ucrtbase._fdpoly
@ stub _fdscale ; ; ;() ucrtbase._fdscale
@ stub _fdsign ; ; ;(float) ucrtbase._fdsign
@ stub _fdsin ; ; ;() ucrtbase._fdsin
@ stub _fdtest ; ; ;(ptr) ucrtbase._fdtest
@ stub _fdunscale ; ; ;() ucrtbase._fdunscale
@ stub _finite ; ; ;(double) ucrtbase._finite
@ stub -arch=arm,x86_64 _finitef ; ; ;(float) ucrtbase._finitef
@ stub _fpclass ; ; ;(double) ucrtbase._fpclass
@ stub _fpclassf ; ; ;() ucrtbase._fpclassf
@ stub -arch=i386 -ret64 _ftol ; ; ;() ucrtbase._ftol
@ stub _get_FMA3_enable ; ; ;() ucrtbase._get_FMA3_enable
@ stub _hypot ; ; ;(double double) ucrtbase._hypot
@ stub _hypotf ; ; ;(float float) ucrtbase._hypotf
@ stub _isnan ; ; ;(double) ucrtbase._isnan
@ stub -arch=arm,x86_64 _isnanf ; ; ;(float) ucrtbase._isnanf
@ stub _j0 ; ; ;(double) ucrtbase._j0
@ stub _j1 ; ; ;(double) ucrtbase._j1
@ stub _jn ; ; ;(long double) ucrtbase._jn
@ stub _ld_int ; ; ;() ucrtbase._ld_int
@ stub _ldclass ; ; ;(double) ucrtbase._ldclass
@ stub _ldexp ; ; ;() ucrtbase._ldexp
@ stub _ldlog ; ; ;() ucrtbase._ldlog
@ stub _ldpcomp ; ; ;(double double) ucrtbase._ldpcomp
@ stub _ldpoly ; ; ;() ucrtbase._ldpoly
@ stub _ldscale ; ; ;() ucrtbase._ldscale
@ stub _ldsign ; ; ;(double) ucrtbase._ldsign
@ stub _ldsin ; ; ;() ucrtbase._ldsin
@ stub _ldtest ; ; ;(ptr) ucrtbase._ldtest
@ stub _ldunscale ; ; ;() ucrtbase._ldunscale
@ stub -arch=i386  _libm_sse2_acos_precise ; ; ;() ucrtbase._libm_sse2_acos_precise
@ stub -arch=i386  _libm_sse2_asin_precise ; ; ;() ucrtbase._libm_sse2_asin_precise
@ stub -arch=i386  _libm_sse2_atan_precise ; ; ;() ucrtbase._libm_sse2_atan_precise
@ stub -arch=i386  _libm_sse2_cos_precise ; ; ;() ucrtbase._libm_sse2_cos_precise
@ stub -arch=i386  _libm_sse2_exp_precise ; ; ;() ucrtbase._libm_sse2_exp_precise
@ stub -arch=i386  _libm_sse2_log10_precise ; ; ;() ucrtbase._libm_sse2_log10_precise
@ stub -arch=i386  _libm_sse2_log_precise ; ; ;() ucrtbase._libm_sse2_log_precise
@ stub -arch=i386  _libm_sse2_pow_precise ; ; ;() ucrtbase._libm_sse2_pow_precise
@ stub -arch=i386  _libm_sse2_sin_precise ; ; ;() ucrtbase._libm_sse2_sin_precise
@ stub -arch=i386  _libm_sse2_sqrt_precise ; ; ;() ucrtbase._libm_sse2_sqrt_precise
@ stub -arch=i386  _libm_sse2_tan_precise ; ; ;() ucrtbase._libm_sse2_tan_precise
@ stub _logb ; ; ;(double) ucrtbase._logb
@ stub -arch=arm,x86_64 _logbf ; ; ;(float) ucrtbase._logbf
@ stub _nextafter ; ; ;(double double) ucrtbase._nextafter
@ stub -arch=arm,x86_64 _nextafterf ; ; ;(float float) ucrtbase._nextafterf
@ stub _scalb ; ; ;(double long) ucrtbase._scalb
@ stub -arch=arm,x86_64 _scalbf ; ; ;(float long) ucrtbase._scalbf
@ stub -arch=win64 _set_FMA3_enable ; ; ;(long) ucrtbase._set_FMA3_enable
@ stub -arch=i386 _set_SSE2_enable ; ; ;(long) ucrtbase._set_SSE2_enable
@ stub _y0 ; ; ;(double) ucrtbase._y0
@ stub _y1 ; ; ;(double) ucrtbase._y1
@ stub _yn ; ; ;(long double) ucrtbase._yn
@ stub acos ; ; ;(double) ucrtbase.acos
@ stub -arch=arm,x86_64 acosf ; ; ;(float) ucrtbase.acosf
@ stub acosh ; ; ;() ucrtbase.acosh
@ stub acoshf ; ; ;() ucrtbase.acoshf
@ stub acoshl ; ; ;() ucrtbase.acoshl
@ stub asin ; ; ;(double) ucrtbase.asin
@ stub -arch=arm,x86_64 asinf ; ; ;(float) ucrtbase.asinf
@ stub asinh ; ; ;(double double) ucrtbase.asinh
@ stub asinhf ; ; ;(float float) ucrtbase.asinhf
@ stub asinhl ; ; ;(double double) ucrtbase.asinhl
@ stub atan ; ; ;(double) ucrtbase.atan
@ stub atan2 ; ; ;(double double) ucrtbase.atan2
@ stub -arch=arm,x86_64 atan2f ; ; ;(float float) ucrtbase.atan2f
@ stub -arch=arm,x86_64 atanf ; ; ;(float) ucrtbase.atanf
@ stub atanh ; ; ;() ucrtbase.atanh
@ stub atanhf ; ; ;() ucrtbase.atanhf
@ stub atanhl ; ; ;() ucrtbase.atanhl
@ stub cabs ; ; ;() ucrtbase.cabs
@ stub cabsf ; ; ;() ucrtbase.cabsf
@ stub cabsl ; ; ;() ucrtbase.cabsl
@ stub cacos ; ; ;() ucrtbase.cacos
@ stub cacosf ; ; ;() ucrtbase.cacosf
@ stub cacosh ; ; ;() ucrtbase.cacosh
@ stub cacoshf ; ; ;() ucrtbase.cacoshf
@ stub cacoshl ; ; ;() ucrtbase.cacoshl
@ stub cacosl ; ; ;() ucrtbase.cacosl
@ stub carg ; ; ;() ucrtbase.carg
@ stub cargf ; ; ;() ucrtbase.cargf
@ stub cargl ; ; ;() ucrtbase.cargl
@ stub casin ; ; ;() ucrtbase.casin
@ stub casinf ; ; ;() ucrtbase.casinf
@ stub casinh ; ; ;() ucrtbase.casinh
@ stub casinhf ; ; ;() ucrtbase.casinhf
@ stub casinhl ; ; ;() ucrtbase.casinhl
@ stub casinl ; ; ;() ucrtbase.casinl
@ stub catan ; ; ;() ucrtbase.catan
@ stub catanf ; ; ;() ucrtbase.catanf
@ stub catanh ; ; ;() ucrtbase.catanh
@ stub catanhf ; ; ;() ucrtbase.catanhf
@ stub catanhl ; ; ;() ucrtbase.catanhl
@ stub catanl ; ; ;() ucrtbase.catanl
@ stub cbrt ; ; ;(double) ucrtbase.cbrt
@ stub cbrtf ; ; ;(float) ucrtbase.cbrtf
@ stub cbrtl ; ; ;(double) ucrtbase.cbrtl
@ stub ccos ; ; ;() ucrtbase.ccos
@ stub ccosf ; ; ;() ucrtbase.ccosf
@ stub ccosh ; ; ;() ucrtbase.ccosh
@ stub ccoshf ; ; ;() ucrtbase.ccoshf
@ stub ccoshl ; ; ;() ucrtbase.ccoshl
@ stub ccosl ; ; ;() ucrtbase.ccosl
@ stub ceil ; ; ;(double) ucrtbase.ceil
@ stub -arch=arm,x86_64 ceilf ; ; ;(float) ucrtbase.ceilf
@ stub cexp ; ; ;() ucrtbase.cexp
@ stub cexpf ; ; ;() ucrtbase.cexpf
@ stub cexpl ; ; ;() ucrtbase.cexpl
@ stub cimag ; ; ;() ucrtbase.cimag
@ stub cimagf ; ; ;() ucrtbase.cimagf
@ stub cimagl ; ; ;() ucrtbase.cimagl
@ stub clog ; ; ;() ucrtbase.clog
@ stub clog10 ; ; ;() ucrtbase.clog10
@ stub clog10f ; ; ;() ucrtbase.clog10f
@ stub clog10l ; ; ;() ucrtbase.clog10l
@ stub clogf ; ; ;() ucrtbase.clogf
@ stub clogl ; ; ;() ucrtbase.clogl
@ stub conj ; ; ;() ucrtbase.conj
@ stub conjf ; ; ;() ucrtbase.conjf
@ stub conjl ; ; ;() ucrtbase.conjl
@ stub copysign ; ; ;(double double) ucrtbase.copysign
@ stub copysignf ; ; ;(float float) ucrtbase.copysignf
@ stub copysignl ; ; ;(double double) ucrtbase.copysignl
@ stub cos ; ; ;(double) ucrtbase.cos
@ stub -arch=arm,x86_64 cosf ; ; ;(float) ucrtbase.cosf
@ stub cosh ; ; ;(double) ucrtbase.cosh
@ stub -arch=arm,x86_64 coshf ; ; ;(float) ucrtbase.coshf
@ stub cpow ; ; ;() ucrtbase.cpow
@ stub cpowf ; ; ;() ucrtbase.cpowf
@ stub cpowl ; ; ;() ucrtbase.cpowl
@ stub cproj ; ; ;() ucrtbase.cproj
@ stub cprojf ; ; ;() ucrtbase.cprojf
@ stub cprojl ; ; ;() ucrtbase.cprojl
@ stub creal ; ; ;() ucrtbase.creal
@ stub crealf ; ; ;() ucrtbase.crealf
@ stub creall ; ; ;() ucrtbase.creall
@ stub csin ; ; ;() ucrtbase.csin
@ stub csinf ; ; ;() ucrtbase.csinf
@ stub csinh ; ; ;() ucrtbase.csinh
@ stub csinhf ; ; ;() ucrtbase.csinhf
@ stub csinhl ; ; ;() ucrtbase.csinhl
@ stub csinl ; ; ;() ucrtbase.csinl
@ stub csqrt ; ; ;() ucrtbase.csqrt
@ stub csqrtf ; ; ;() ucrtbase.csqrtf
@ stub csqrtl ; ; ;() ucrtbase.csqrtl
@ stub ctan ; ; ;() ucrtbase.ctan
@ stub ctanf ; ; ;() ucrtbase.ctanf
@ stub ctanh ; ; ;() ucrtbase.ctanh
@ stub ctanhf ; ; ;() ucrtbase.ctanhf
@ stub ctanhl ; ; ;() ucrtbase.ctanhl
@ stub ctanl ; ; ;() ucrtbase.ctanl
@ stub erf ; ; ;(double) ucrtbase.erf
@ stub erfc ; ; ;() ucrtbase.erfc
@ stub erfcf ; ; ;() ucrtbase.erfcf
@ stub erfcl ; ; ;() ucrtbase.erfcl
@ stub erff ; ; ;(double) ucrtbase.erff
@ stub erfl ; ; ;(double) ucrtbase.erfl
@ stub exp ; ; ;(double) ucrtbase.exp
@ stub exp2 ; ; ;(double) ucrtbase.exp2
@ stub exp2f ; ; ;(float) ucrtbase.exp2f
@ stub exp2l ; ; ;(double) ucrtbase.exp2l
@ stub -arch=arm,x86_64 expf ; ; ;(float) ucrtbase.expf
@ stub expm1 ; ; ;() ucrtbase.expm1
@ stub expm1f ; ; ;() ucrtbase.expm1f
@ stub expm1l ; ; ;() ucrtbase.expm1l
@ stub fabs ; ; ;(double) ucrtbase.fabs
@ stub -arch=arm fabsf ; ; ;(float) ucrtbase.fabsf
@ stub fdim ; ; ;() ucrtbase.fdim
@ stub fdimf ; ; ;() ucrtbase.fdimf
@ stub fdiml ; ; ;() ucrtbase.fdiml
@ stub floor ; ; ;(double) ucrtbase.floor
@ stub -arch=arm,x86_64 floorf ; ; ;(float) ucrtbase.floorf
@ stub fma ; ; ;() ucrtbase.fma
@ stub fmaf ; ; ;() ucrtbase.fmaf
@ stub fmal ; ; ;() ucrtbase.fmal
@ stub fmax ; ; ;(double double) ucrtbase.fmax
@ stub fmaxf ; ; ;(float float) ucrtbase.fmaxf
@ stub fmaxl ; ; ;(double double) ucrtbase.fmaxl
@ stub fmin ; ; ;(double double) ucrtbase.fmin
@ stub fminf ; ; ;(float float) ucrtbase.fminf
@ stub fminl ; ; ;(double double) ucrtbase.fminl
@ stub fmod ; ; ;(double double) ucrtbase.fmod
@ stub -arch=arm,x86_64 fmodf ; ; ;(float float) ucrtbase.fmodf
@ stub frexp ; ; ;(double ptr) ucrtbase.frexp
@ stub hypot ; ; ;(double double) ucrtbase.hypot
@ stub ilogb ; ; ;() ucrtbase.ilogb
@ stub ilogbf ; ; ;() ucrtbase.ilogbf
@ stub ilogbl ; ; ;() ucrtbase.ilogbl
@ stub ldexp ; ; ;(double long) ucrtbase.ldexp
@ stub lgamma ; ; ;(double) ucrtbase.lgamma
@ stub lgammaf ; ; ;(float) ucrtbase.lgammaf
@ stub lgammal ; ; ;(double) ucrtbase.lgammal
@ stub -ret64 llrint ; ; ;(double) ucrtbase.llrint
@ stub -ret64 llrintf ; ; ;(float) ucrtbase.llrintf
@ stub -ret64 llrintl ; ; ;(double) ucrtbase.llrintl
@ stub -ret64 llround ; ; ;(double) ucrtbase.llround
@ stub -ret64 llroundf ; ; ;(float) ucrtbase.llroundf
@ stub -ret64 llroundl ; ; ;(double) ucrtbase.llroundl
@ stub log ; ; ;(double) ucrtbase.log
@ stub log10 ; ; ;(double) ucrtbase.log10
@ stub -arch=arm,x86_64 log10f ; ; ;(float) ucrtbase.log10f
@ stub log1p ; ; ;() ucrtbase.log1p
@ stub log1pf ; ; ;() ucrtbase.log1pf
@ stub log1pl ; ; ;() ucrtbase.log1pl
@ stub log2 ; ; ;(double) ucrtbase.log2
@ stub log2f ; ; ;(float) ucrtbase.log2f
@ stub log2l ; ; ;(double) ucrtbase.log2l
@ stub logb ; ; ;() ucrtbase.logb
@ stub logbf ; ; ;() ucrtbase.logbf
@ stub logbl ; ; ;() ucrtbase.logbl
@ stub -arch=arm,x86_64 logf ; ; ;(float) ucrtbase.logf
@ stub lrint ; ; ;(double) ucrtbase.lrint
@ stub lrintf ; ; ;(float) ucrtbase.lrintf
@ stub lrintl ; ; ;(double) ucrtbase.lrintl
@ stub lround ; ; ;(double) ucrtbase.lround
@ stub lroundf ; ; ;(float) ucrtbase.lroundf
@ stub lroundl ; ; ;(double) ucrtbase.lroundl
@ stub modf ; ; ;(double ptr) ucrtbase.modf
@ stub -arch=arm,x86_64 modff ; ; ;(float ptr) ucrtbase.modff
@ stub nan ; ; ;() ucrtbase.nan
@ stub nanf ; ; ;() ucrtbase.nanf
@ stub nanl ; ; ;() ucrtbase.nanl
@ stub nearbyint ; ; ;() ucrtbase.nearbyint
@ stub nearbyintf ; ; ;() ucrtbase.nearbyintf
@ stub nearbyintl ; ; ;() ucrtbase.nearbyintl
@ stub nextafter ; ; ;(double double) ucrtbase.nextafter
@ stub nextafterf ; ; ;(float float) ucrtbase.nextafterf
@ stub nextafterl ; ; ;(double double) ucrtbase.nextafterl
@ stub nexttoward ; ; ;() ucrtbase.nexttoward
@ stub nexttowardf ; ; ;() ucrtbase.nexttowardf
@ stub nexttowardl ; ; ;() ucrtbase.nexttowardl
@ stub norm ; ; ;() ucrtbase.norm
@ stub normf ; ; ;() ucrtbase.normf
@ stub norml ; ; ;() ucrtbase.norml
@ stub pow ; ; ;(double double) ucrtbase.pow
@ stub -arch=arm,x86_64 powf ; ; ;(float float) ucrtbase.powf
@ stub remainder ; ; ;(double double) ucrtbase.remainder
@ stub remainderf ; ; ;(float float) ucrtbase.remainderf
@ stub remainderl ; ; ;(double double) ucrtbase.remainderl
@ stub remquo ; ; ;() ucrtbase.remquo
@ stub remquof ; ; ;() ucrtbase.remquof
@ stub remquol ; ; ;() ucrtbase.remquol
@ stub rint ; ; ;(double) ucrtbase.rint
@ stub rintf ; ; ;(float) ucrtbase.rintf
@ stub rintl ; ; ;(double) ucrtbase.rintl
@ stub round ; ; ;(double) ucrtbase.round
@ stub roundf ; ; ;(float) ucrtbase.roundf
@ stub roundl ; ; ;(double) ucrtbase.roundl
@ stub scalbln ; ; ;(double long) ucrtbase.scalbln
@ stub scalblnf ; ; ;(float long) ucrtbase.scalblnf
@ stub scalblnl ; ; ;(double long) ucrtbase.scalblnl
@ stub scalbn ; ; ;(double long) ucrtbase.scalbn
@ stub scalbnf ; ; ;(float long) ucrtbase.scalbnf
@ stub scalbnl ; ; ;(double long) ucrtbase.scalbnl
@ stub sin ; ; ;(double) ucrtbase.sin
@ stub -arch=arm,x86_64 sinf ; ; ;(float) ucrtbase.sinf
@ stub sinh ; ; ;(double) ucrtbase.sinh
@ stub -arch=arm,x86_64 sinhf ; ; ;(float) ucrtbase.sinhf
@ stub sqrt ; ; ;(double) ucrtbase.sqrt
@ stub -arch=arm,x86_64 sqrtf ; ; ;(float) ucrtbase.sqrtf
@ stub tan ; ; ;(double) ucrtbase.tan
@ stub -arch=arm,x86_64 tanf ; ; ;(float) ucrtbase.tanf
@ stub tanh ; ; ;(double) ucrtbase.tanh
@ stub -arch=arm,x86_64 tanhf ; ; ;(float) ucrtbase.tanhf
@ stub tgamma ; ; ;() ucrtbase.tgamma
@ stub tgammaf ; ; ;() ucrtbase.tgammaf
@ stub tgammal ; ; ;() ucrtbase.tgammal
@ stub trunc ; ; ;(double) ucrtbase.trunc
@ stub truncf ; ; ;(float) ucrtbase.truncf
@ stub truncl ; ; ;(double) ucrtbase.truncl
@ stub __p__mbcasemap ; ;() ucrtbase.__p__mbcasemap
@ stub __p__mbctype ; ;() ucrtbase.__p__mbctype
@ stub _ismbbalnum ; ;(long) ucrtbase._ismbbalnum
@ stub _ismbbalnum_l ; ;() ucrtbase._ismbbalnum_l
@ stub _ismbbalpha ; ;(long) ucrtbase._ismbbalpha
@ stub _ismbbalpha_l ; ;() ucrtbase._ismbbalpha_l
@ stub _ismbbblank ; ;() ucrtbase._ismbbblank
@ stub _ismbbblank_l ; ;() ucrtbase._ismbbblank_l
@ stub _ismbbgraph ; ;(long) ucrtbase._ismbbgraph
@ stub _ismbbgraph_l ; ;() ucrtbase._ismbbgraph_l
@ stub _ismbbkalnum ; ;(long) ucrtbase._ismbbkalnum
@ stub _ismbbkalnum_l ; ;() ucrtbase._ismbbkalnum_l
@ stub _ismbbkana ; ;(long) ucrtbase._ismbbkana
@ stub _ismbbkana_l ; ;() ucrtbase._ismbbkana_l
@ stub _ismbbkprint ; ;(long) ucrtbase._ismbbkprint
@ stub _ismbbkprint_l ; ;() ucrtbase._ismbbkprint_l
@ stub _ismbbkpunct ; ;(long) ucrtbase._ismbbkpunct
@ stub _ismbbkpunct_l ; ;() ucrtbase._ismbbkpunct_l
@ stub _ismbblead ; ;(long) ucrtbase._ismbblead
@ stub _ismbblead_l ; ;(long ptr) ucrtbase._ismbblead_l
@ stub _ismbbprint ; ;(long) ucrtbase._ismbbprint
@ stub _ismbbprint_l ; ;() ucrtbase._ismbbprint_l
@ stub _ismbbpunct ; ;(long) ucrtbase._ismbbpunct
@ stub _ismbbpunct_l ; ;() ucrtbase._ismbbpunct_l
@ stub _ismbbtrail ; ;(long) ucrtbase._ismbbtrail
@ stub _ismbbtrail_l ; ;() ucrtbase._ismbbtrail_l
@ stub _ismbcalnum ; ;(long) ucrtbase._ismbcalnum
@ stub _ismbcalnum_l ; ;() ucrtbase._ismbcalnum_l
@ stub _ismbcalpha ; ;(long) ucrtbase._ismbcalpha
@ stub _ismbcalpha_l ; ;() ucrtbase._ismbcalpha_l
@ stub _ismbcblank ; ;() ucrtbase._ismbcblank
@ stub _ismbcblank_l ; ;() ucrtbase._ismbcblank_l
@ stub _ismbcdigit ; ;(long) ucrtbase._ismbcdigit
@ stub _ismbcdigit_l ; ;() ucrtbase._ismbcdigit_l
@ stub _ismbcgraph ; ;(long) ucrtbase._ismbcgraph
@ stub _ismbcgraph_l ; ;() ucrtbase._ismbcgraph_l
@ stub _ismbchira ; ;(long) ucrtbase._ismbchira
@ stub _ismbchira_l ; ;() ucrtbase._ismbchira_l
@ stub _ismbckata ; ;(long) ucrtbase._ismbckata
@ stub _ismbckata_l ; ;() ucrtbase._ismbckata_l
@ stub _ismbcl0 ; ;(long) ucrtbase._ismbcl0
@ stub _ismbcl0_l ; ;() ucrtbase._ismbcl0_l
@ stub _ismbcl1 ; ;(long) ucrtbase._ismbcl1
@ stub _ismbcl1_l ; ;() ucrtbase._ismbcl1_l
@ stub _ismbcl2 ; ;(long) ucrtbase._ismbcl2
@ stub _ismbcl2_l ; ;() ucrtbase._ismbcl2_l
@ stub _ismbclegal ; ;(long) ucrtbase._ismbclegal
@ stub _ismbclegal_l ; ;() ucrtbase._ismbclegal_l
@ stub _ismbclower ; ;(long) ucrtbase._ismbclower
@ stub _ismbclower_l ; ;() ucrtbase._ismbclower_l
@ stub _ismbcprint ; ;(long) ucrtbase._ismbcprint
@ stub _ismbcprint_l ; ;() ucrtbase._ismbcprint_l
@ stub _ismbcpunct ; ;(long) ucrtbase._ismbcpunct
@ stub _ismbcpunct_l ; ;() ucrtbase._ismbcpunct_l
@ stub _ismbcspace ; ;(long) ucrtbase._ismbcspace
@ stub _ismbcspace_l ; ;() ucrtbase._ismbcspace_l
@ stub _ismbcsymbol ; ;(long) ucrtbase._ismbcsymbol
@ stub _ismbcsymbol_l ; ;() ucrtbase._ismbcsymbol_l
@ stub _ismbcupper ; ;(long) ucrtbase._ismbcupper
@ stub _ismbcupper_l ; ;() ucrtbase._ismbcupper_l
@ stub _ismbslead ; ;(ptr ptr) ucrtbase._ismbslead
@ stub _ismbslead_l ; ;() ucrtbase._ismbslead_l
@ stub _ismbstrail ; ;(ptr ptr) ucrtbase._ismbstrail
@ stub _ismbstrail_l ; ;() ucrtbase._ismbstrail_l
@ stub _mbbtombc ; ;(long) ucrtbase._mbbtombc
@ stub _mbbtombc_l ; ;() ucrtbase._mbbtombc_l
@ stub _mbbtype ; ;(long long) ucrtbase._mbbtype
@ stub _mbbtype_l ; ;() ucrtbase._mbbtype_l
@ stub _mbcasemap ; ;() ucrtbase._mbcasemap
@ stub _mbccpy ; ;(ptr str) ucrtbase._mbccpy
@ stub _mbccpy_l ; ;() ucrtbase._mbccpy_l
@ stub _mbccpy_s ; ;() ucrtbase._mbccpy_s
@ stub _mbccpy_s_l ; ;() ucrtbase._mbccpy_s_l
@ stub _mbcjistojms ; ;(long) ucrtbase._mbcjistojms
@ stub _mbcjistojms_l ; ;() ucrtbase._mbcjistojms_l
@ stub _mbcjmstojis ; ;(long) ucrtbase._mbcjmstojis
@ stub _mbcjmstojis_l ; ;() ucrtbase._mbcjmstojis_l
@ stub _mbclen ; ;(ptr) ucrtbase._mbclen
@ stub _mbclen_l ; ;() ucrtbase._mbclen_l
@ stub _mbctohira ; ;(long) ucrtbase._mbctohira
@ stub _mbctohira_l ; ;() ucrtbase._mbctohira_l
@ stub _mbctokata ; ;(long) ucrtbase._mbctokata
@ stub _mbctokata_l ; ;() ucrtbase._mbctokata_l
@ stub _mbctolower ; ;(long) ucrtbase._mbctolower
@ stub _mbctolower_l ; ;() ucrtbase._mbctolower_l
@ stub _mbctombb ; ;(long) ucrtbase._mbctombb
@ stub _mbctombb_l ; ;() ucrtbase._mbctombb_l
@ stub _mbctoupper ; ;(long) ucrtbase._mbctoupper
@ stub _mbctoupper_l ; ;() ucrtbase._mbctoupper_l
@ stub _mblen_l ; ;() ucrtbase._mblen_l
@ stub _mbsbtype ; ;(str long) ucrtbase._mbsbtype
@ stub _mbsbtype_l ; ;() ucrtbase._mbsbtype_l
@ stub _mbscat_s ; ;(ptr long str) ucrtbase._mbscat_s
@ stub _mbscat_s_l ; ;(ptr long str ptr) ucrtbase._mbscat_s_l
@ stub _mbschr ; ;(str long) ucrtbase._mbschr
@ stub _mbschr_l ; ;() ucrtbase._mbschr_l
@ stub _mbscmp ; ;(str str) ucrtbase._mbscmp
@ stub _mbscmp_l ; ;() ucrtbase._mbscmp_l
@ stub _mbscoll ; ;(str str) ucrtbase._mbscoll
@ stub _mbscoll_l ; ;(str str ptr) ucrtbase._mbscoll_l
@ stub _mbscpy_s ; ;(ptr long str) ucrtbase._mbscpy_s
@ stub _mbscpy_s_l ; ;(ptr long str ptr) ucrtbase._mbscpy_s_l
@ stub _mbscspn ; ;(str str) ucrtbase._mbscspn
@ stub _mbscspn_l ; ;() ucrtbase._mbscspn_l
@ stub _mbsdec ; ;(ptr ptr) ucrtbase._mbsdec
@ stub _mbsdec_l ; ;() ucrtbase._mbsdec_l
@ stub _mbsdup ; ;(str) ucrtbase._mbsdup
@ stub _mbsicmp ; ;(str str) ucrtbase._mbsicmp
@ stub _mbsicmp_l ; ;() ucrtbase._mbsicmp_l
@ stub _mbsicoll ; ;(str str) ucrtbase._mbsicoll
@ stub _mbsicoll_l ; ;(str str ptr) ucrtbase._mbsicoll_l
@ stub _mbsinc ; ;(str) ucrtbase._mbsinc
@ stub _mbsinc_l ; ;() ucrtbase._mbsinc_l
@ stub _mbslen ; ;(str) ucrtbase._mbslen
@ stub _mbslen_l ; ;() ucrtbase._mbslen_l
@ stub _mbslwr ; ;(str) ucrtbase._mbslwr
@ stub _mbslwr_l ; ;() ucrtbase._mbslwr_l
@ stub _mbslwr_s ; ;(str long) ucrtbase._mbslwr_s
@ stub _mbslwr_s_l ; ;() ucrtbase._mbslwr_s_l
@ stub _mbsnbcat ; ;(str str long) ucrtbase._mbsnbcat
@ stub _mbsnbcat_l ; ;() ucrtbase._mbsnbcat_l
@ stub _mbsnbcat_s ; ;(str long ptr long) ucrtbase._mbsnbcat_s
@ stub _mbsnbcat_s_l ; ;() ucrtbase._mbsnbcat_s_l
@ stub _mbsnbcmp ; ;(str str long) ucrtbase._mbsnbcmp
@ stub _mbsnbcmp_l ; ;() ucrtbase._mbsnbcmp_l
@ stub _mbsnbcnt ; ;(ptr long) ucrtbase._mbsnbcnt
@ stub _mbsnbcnt_l ; ;() ucrtbase._mbsnbcnt_l
@ stub _mbsnbcoll ; ;(str str long) ucrtbase._mbsnbcoll
@ stub _mbsnbcoll_l ; ;(str str long ptr) ucrtbase._mbsnbcoll_l
@ stub _mbsnbcpy ; ;(ptr str long) ucrtbase._mbsnbcpy
@ stub _mbsnbcpy_l ; ;() ucrtbase._mbsnbcpy_l
@ stub _mbsnbcpy_s ; ;(ptr long str long) ucrtbase._mbsnbcpy_s
@ stub _mbsnbcpy_s_l ; ;(ptr long str long ptr) ucrtbase._mbsnbcpy_s_l
@ stub _mbsnbicmp ; ;(str str long) ucrtbase._mbsnbicmp
@ stub _mbsnbicmp_l ; ;() ucrtbase._mbsnbicmp_l
@ stub _mbsnbicoll ; ;(str str long) ucrtbase._mbsnbicoll
@ stub _mbsnbicoll_l ; ;(str str long ptr) ucrtbase._mbsnbicoll_l
@ stub _mbsnbset ; ;(ptr long long) ucrtbase._mbsnbset
@ stub _mbsnbset_l ; ;() ucrtbase._mbsnbset_l
@ stub _mbsnbset_s ; ;() ucrtbase._mbsnbset_s
@ stub _mbsnbset_s_l ; ;() ucrtbase._mbsnbset_s_l
@ stub _mbsncat ; ;(str str long) ucrtbase._mbsncat
@ stub _mbsncat_l ; ;() ucrtbase._mbsncat_l
@ stub _mbsncat_s ; ;() ucrtbase._mbsncat_s
@ stub _mbsncat_s_l ; ;() ucrtbase._mbsncat_s_l
@ stub _mbsnccnt ; ;(str long) ucrtbase._mbsnccnt
@ stub _mbsnccnt_l ; ;() ucrtbase._mbsnccnt_l
@ stub _mbsncmp ; ;(str str long) ucrtbase._mbsncmp
@ stub _mbsncmp_l ; ;() ucrtbase._mbsncmp_l
@ stub _mbsncoll ; ;(str str long) ucrtbase._mbsncoll
@ stub _mbsncoll_l ; ;() ucrtbase._mbsncoll_l
@ stub _mbsncpy ; ;(ptr str long) ucrtbase._mbsncpy
@ stub _mbsncpy_l ; ;() ucrtbase._mbsncpy_l
@ stub _mbsncpy_s ; ;() ucrtbase._mbsncpy_s
@ stub _mbsncpy_s_l ; ;() ucrtbase._mbsncpy_s_l
@ stub _mbsnextc ; ;(str) ucrtbase._mbsnextc
@ stub _mbsnextc_l ; ;() ucrtbase._mbsnextc_l
@ stub _mbsnicmp ; ;(str str long) ucrtbase._mbsnicmp
@ stub _mbsnicmp_l ; ;() ucrtbase._mbsnicmp_l
@ stub _mbsnicoll ; ;(str str long) ucrtbase._mbsnicoll
@ stub _mbsnicoll_l ; ;() ucrtbase._mbsnicoll_l
@ stub _mbsninc ; ;(str long) ucrtbase._mbsninc
@ stub _mbsninc_l ; ;() ucrtbase._mbsninc_l
@ stub _mbsnlen ; ;() ucrtbase._mbsnlen
@ stub _mbsnlen_l ; ;() ucrtbase._mbsnlen_l
@ stub _mbsnset ; ;(ptr long long) ucrtbase._mbsnset
@ stub _mbsnset_l ; ;() ucrtbase._mbsnset_l
@ stub _mbsnset_s ; ;() ucrtbase._mbsnset_s
@ stub _mbsnset_s_l ; ;() ucrtbase._mbsnset_s_l
@ stub _mbspbrk ; ;(str str) ucrtbase._mbspbrk
@ stub _mbspbrk_l ; ;() ucrtbase._mbspbrk_l
@ stub _mbsrchr ; ;(str long) ucrtbase._mbsrchr
@ stub _mbsrchr_l ; ;() ucrtbase._mbsrchr_l
@ stub _mbsrev ; ;(str) ucrtbase._mbsrev
@ stub _mbsrev_l ; ;() ucrtbase._mbsrev_l
@ stub _mbsset ; ;(ptr long) ucrtbase._mbsset
@ stub _mbsset_l ; ;() ucrtbase._mbsset_l
@ stub _mbsset_s ; ;() ucrtbase._mbsset_s
@ stub _mbsset_s_l ; ;() ucrtbase._mbsset_s_l
@ stub _mbsspn ; ;(str str) ucrtbase._mbsspn
@ stub _mbsspn_l ; ;() ucrtbase._mbsspn_l
@ stub _mbsspnp ; ;(str str) ucrtbase._mbsspnp
@ stub _mbsspnp_l ; ;() ucrtbase._mbsspnp_l
@ stub _mbsstr ; ;(str str) ucrtbase._mbsstr
@ stub _mbsstr_l ; ;() ucrtbase._mbsstr_l
@ stub _mbstok ; ;(str str) ucrtbase._mbstok
@ stub _mbstok_l ; ;(str str ptr) ucrtbase._mbstok_l
@ stub _mbstok_s ; ;(str str ptr) ucrtbase._mbstok_s
@ stub _mbstok_s_l ; ;(str str ptr ptr) ucrtbase._mbstok_s_l
@ stub _mbstowcs_l ; ;(ptr str long ptr) ucrtbase._mbstowcs_l
@ stub _mbstowcs_s_l ; ;(ptr ptr long str long ptr) ucrtbase._mbstowcs_s_l
@ stub _mbstrlen ; ;(str) ucrtbase._mbstrlen
@ stub _mbstrlen_l ; ;(str ptr) ucrtbase._mbstrlen_l
@ stub _mbstrnlen ; ;() ucrtbase._mbstrnlen
@ stub _mbstrnlen_l ; ;() ucrtbase._mbstrnlen_l
@ stub _mbsupr ; ;(str) ucrtbase._mbsupr
@ stub _mbsupr_l ; ;() ucrtbase._mbsupr_l
@ stub _mbsupr_s ; ;(str long) ucrtbase._mbsupr_s
@ stub _mbsupr_s_l ; ;() ucrtbase._mbsupr_s_l
@ stub _mbtowc_l ; ;(ptr str long ptr) ucrtbase._mbtowc_l
@ stub _CreateFrameInfo ;(ptr ptr) ucrtbase._CreateFrameInfo
@ stub _CxxThrowException ;(long long) ucrtbase._CxxThrowException
@ stub -arch=i386 -norelay _EH_prolog ;() ucrtbase._EH_prolog
@ stub _FindAndUnlinkFrame ;(ptr) ucrtbase._FindAndUnlinkFrame
@ stub _GetImageBase
@ stub _GetThrowImageBase
@ stub _IsExceptionObjectToBeDestroyed ;(ptr) ucrtbase._IsExceptionObjectToBeDestroyed
@ stub _NLG_Dispatch2
@ stub _NLG_Return
@ stub _NLG_Return2
@ stub _SetImageBase
@ stub _SetThrowImageBase
@ stub _SetWinRTOutOfMemoryExceptionCallback
@ stub __AdjustPointer ;(ptr ptr) ucrtbase.__AdjustPointer
@ stub __BuildCatchObject
@ stub __BuildCatchObjectHelper
@ stub -arch=x86_64 __C_specific_handler ;(ptr long ptr ptr) ucrtbase.__C_specific_handler
@ stub -arch=i386,x86_64,arm __CxxDetectRethrow ;(ptr) ucrtbase.__CxxDetectRethrow
@ stub -arch=i386,x86_64,arm __CxxExceptionFilter ;(ptr ptr long ptr) ucrtbase.__CxxExceptionFilter
@ stub -arch=i386,x86_64,arm -norelay __CxxFrameHandler ;(ptr ptr ptr ptr) ucrtbase.__CxxFrameHandler
@ stub -arch=i386,x86_64,arm -norelay __CxxFrameHandler2 ;(ptr ptr ptr ptr) ucrtbase.__CxxFrameHandler2
@ stub -arch=i386,x86_64,arm -norelay __CxxFrameHandler3 ;(ptr ptr ptr ptr) ucrtbase.__CxxFrameHandler3
@ stub -arch=i386 __CxxLongjmpUnwind ;(ptr) ucrtbase.__CxxLongjmpUnwind
@ stub -arch=i386,x86_64,arm __CxxQueryExceptionSize ;() ucrtbase.__CxxQueryExceptionSize
@ stub __CxxRegisterExceptionObject ;(ptr ptr) ucrtbase.__CxxRegisterExceptionObject
@ stub __CxxUnregisterExceptionObject ;(ptr long) ucrtbase.__CxxUnregisterExceptionObject
@ stub __DestructExceptionObject ;(ptr) ucrtbase.__DestructExceptionObject
@ stub __FrameUnwindFilter
@ stub __GetPlatformExceptionInfo
@ stub __NLG_Dispatch2
@ stub __NLG_Return2
@ stub __RTCastToVoid ;(ptr) ucrtbase.__RTCastToVoid
@ stub __RTDynamicCast ;(ptr long ptr ptr long) ucrtbase.__RTDynamicCast
@ stub __RTtypeid ;(ptr) ucrtbase.__RTtypeid
@ stub __TypeMatch
@ stub __current_exception
@ stub __current_exception_context
@ stub __dcrt_get_wide_environment_from_os
@ stub __dcrt_initial_narrow_environment
@ stub __intrinsic_abnormal_termination
@ stub __intrinsic_setjmp
@ stub __intrinsic_setjmpex
@ stub __processing_throw
@ stub __report_gsfailure
@ stub __std_exception_copy ;(ptr ptr) ucrtbase.__std_exception_copy
@ stub __std_exception_destroy ;(ptr) ucrtbase.__std_exception_destroy
@ stub __std_type_info_compare ;(ptr ptr) ucrtbase.__std_type_info_compare
@ stub __std_type_info_destroy_list ;(ptr) ucrtbase.__std_type_info_destroy_list
@ stub __std_type_info_hash ;(ptr) ucrtbase.__std_type_info_hash
@ stub __std_type_info_name ;(ptr ptr) ucrtbase.__std_type_info_name
@ stub __unDName ;(ptr str long ptr ptr long) ucrtbase.__unDName
@ stub __unDNameEx ;(ptr str long ptr ptr ptr long) ucrtbase.__unDNameEx
@ stub __uncaught_exception ;() ucrtbase.__uncaught_exception
@ stub -arch=i386 -norelay _chkesp ;() ucrtbase._chkesp
@ stub -arch=i386 _except_handler2 ;(ptr ptr ptr ptr) ucrtbase._except_handler2
@ stub -arch=i386 _except_handler3 ;(ptr ptr ptr ptr) ucrtbase._except_handler3
@ stub -arch=i386 _except_handler4_common ;(ptr ptr ptr ptr ptr ptr) ucrtbase._except_handler4_common
@ stub _get_purecall_handler
@ stub _get_unexpected ;() ucrtbase._get_unexpected
@ stub -arch=i386 _global_unwind2 ;(ptr) ucrtbase._global_unwind2
@ stub _is_exception_typeof
@ stub -arch=x86_64 _local_unwind ;(ptr ptr) ucrtbase._local_unwind
@ stub -arch=i386 _local_unwind2 ;(ptr long) ucrtbase._local_unwind2
@ stub -arch=i386 _local_unwind4 ;(ptr ptr long) ucrtbase._local_unwind4
@ stub -arch=i386 _longjmpex ;(ptr long) ucrtbase._longjmpex
@ stub _o__CIacos
@ stub _o__CIasin
@ stub _o__CIatan
@ stub _o__CIatan2
@ stub _o__CIcos
@ stub _o__CIcosh
@ stub _o__CIexp
@ stub _o__CIfmod
@ stub _o__CIlog
@ stub _o__CIlog10
@ stub _o__CIpow
@ stub _o__CIsin
@ stub _o__CIsinh
@ stub _o__CIsqrt
@ stub _o__CItan
@ stub _o__CItanh
@ stub _o__Getdays
@ stub _o__Getmonths
@ stub _o__Gettnames
@ stub _o__Strftime
@ stub _o__W_Getdays
@ stub _o__W_Getmonths
@ stub _o__W_Gettnames
@ stub _o__Wcsftime
@ stub _o___acrt_iob_func
@ stub _o___conio_common_vcprintf
@ stub _o___conio_common_vcprintf_p
@ stub _o___conio_common_vcprintf_s
@ stub _o___conio_common_vcscanf
@ stub _o___conio_common_vcwprintf
@ stub _o___conio_common_vcwprintf_p
@ stub _o___conio_common_vcwprintf_s
@ stub _o___conio_common_vcwscanf
@ stub _o___daylight
@ stub _o___dstbias
@ stub _o___fpe_flt_rounds
@ stub _o___libm_sse2_acos
@ stub _o___libm_sse2_acosf
@ stub _o___libm_sse2_asin
@ stub _o___libm_sse2_asinf
@ stub _o___libm_sse2_atan
@ stub _o___libm_sse2_atan2
@ stub _o___libm_sse2_atanf
@ stub _o___libm_sse2_cos
@ stub _o___libm_sse2_cosf
@ stub _o___libm_sse2_exp
@ stub _o___libm_sse2_expf
@ stub _o___libm_sse2_log
@ stub _o___libm_sse2_log10
@ stub _o___libm_sse2_log10f
@ stub _o___libm_sse2_logf
@ stub _o___libm_sse2_pow
@ stub _o___libm_sse2_powf
@ stub _o___libm_sse2_sin
@ stub _o___libm_sse2_sinf
@ stub _o___libm_sse2_tan
@ stub _o___libm_sse2_tanf
@ stub _o___p___argc
@ stub _o___p___argv
@ stub _o___p___wargv
@ stub _o___p__acmdln
@ stub _o___p__commode
@ stub _o___p__environ
@ stub _o___p__fmode
@ stub _o___p__mbcasemap
@ stub _o___p__mbctype
@ stub _o___p__pgmptr
@ stub _o___p__wcmdln
@ stub _o___p__wenviron
@ stub _o___p__wpgmptr
@ stub _o___pctype_func
@ stub _o___pwctype_func
@ stub _o___stdio_common_vfprintf
@ stub _o___stdio_common_vfprintf_p
@ stub _o___stdio_common_vfprintf_s
@ stub _o___stdio_common_vfscanf
@ stub _o___stdio_common_vfwprintf
@ stub _o___stdio_common_vfwprintf_p
@ stub _o___stdio_common_vfwprintf_s
@ stub _o___stdio_common_vfwscanf
@ stub _o___stdio_common_vsnprintf_s
@ stub _o___stdio_common_vsnwprintf_s
@ stub _o___stdio_common_vsprintf
@ stub _o___stdio_common_vsprintf_p
@ stub _o___stdio_common_vsprintf_s
@ stub _o___stdio_common_vsscanf
@ stub _o___stdio_common_vswprintf
@ stub _o___stdio_common_vswprintf_p
@ stub _o___stdio_common_vswprintf_s
@ stub _o___stdio_common_vswscanf
@ stub _o___timezone
@ stub _o___tzname
@ stub _o___wcserror
@ stub _o__access
@ stub _o__access_s
@ stub _o__aligned_free
@ stub _o__aligned_malloc
@ stub _o__aligned_msize
@ stub _o__aligned_offset_malloc
@ stub _o__aligned_offset_realloc
@ stub _o__aligned_offset_recalloc
@ stub _o__aligned_realloc
@ stub _o__aligned_recalloc
@ stub _o__atodbl
@ stub _o__atodbl_l
@ stub _o__atof_l
@ stub _o__atoflt
@ stub _o__atoflt_l
@ stub _o__atoi64
@ stub _o__atoi64_l
@ stub _o__atoi_l
@ stub _o__atol_l
@ stub _o__atoldbl
@ stub _o__atoldbl_l
@ stub _o__atoll_l
@ stub _o__beep
@ stub _o__beginthread
@ stub _o__beginthreadex
@ stub _o__cabs
@ stub _o__callnewh
@ stub _o__calloc_base
@ stub _o__cgets
@ stub _o__cgets_s
@ stub _o__cgetws
@ stub _o__cgetws_s
@ stub _o__chdir
@ stub _o__chdrive
@ stub _o__chmod
@ stub _o__chsize
@ stub _o__chsize_s
@ stub _o__close
@ stub _o__commit
@ stub _o__configure_wide_argv
@ stub _o__cputs
@ stub _o__cputws
@ stub _o__creat
@ stub _o__create_locale
@ stub _o__ctime32_s
@ stub _o__ctime64_s
@ stub _o__cwait
@ stub _o__d_int
@ stub _o__dclass
@ stub _o__difftime32
@ stub _o__difftime64
@ stub _o__dlog
@ stub _o__dnorm
@ stub _o__dpcomp
@ stub _o__dpoly
@ stub _o__dscale
@ stub _o__dsign
@ stub _o__dsin
@ stub _o__dtest
@ stub _o__dunscale
@ stub _o__dup
@ stub _o__dup2
@ stub _o__dupenv_s
@ stub _o__ecvt
@ stub _o__ecvt_s
@ stub _o__endthread
@ stub _o__endthreadex
@ stub _o__eof
@ stub _o__errno
@ stub _o__except1
@ stub _o__execute_onexit_table
@ stub _o__execv
@ stub _o__execve
@ stub _o__execvp
@ stub _o__execvpe
@ stub _o__expand
@ stub _o__fclose_nolock
@ stub _o__fcloseall
@ stub _o__fcvt
@ stub _o__fcvt_s
@ stub _o__fd_int
@ stub _o__fdclass
@ stub _o__fdexp
@ stub _o__fdlog
@ stub _o__fdopen
@ stub _o__fdpcomp
@ stub _o__fdpoly
@ stub _o__fdscale
@ stub _o__fdsign
@ stub _o__fdsin
@ stub _o__fflush_nolock
@ stub _o__fgetc_nolock
@ stub _o__fgetchar
@ stub _o__fgetwc_nolock
@ stub _o__fgetwchar
@ stub _o__filelength
@ stub _o__filelengthi64
@ stub _o__fileno
@ stub _o__findclose
@ stub _o__findfirst32
@ stub _o__findfirst32i64
@ stub _o__findfirst64
@ stub _o__findfirst64i32
@ stub _o__findnext32
@ stub _o__findnext32i64
@ stub _o__findnext64
@ stub _o__findnext64i32
@ stub _o__flushall
@ stub _o__fpclass
@ stub _o__fpclassf
@ stub _o__fputc_nolock
@ stub _o__fputchar
@ stub _o__fputwc_nolock
@ stub _o__fputwchar
@ stub _o__fread_nolock
@ stub _o__fread_nolock_s
@ stub _o__free_base
@ stub _o__free_locale
@ stub _o__fseek_nolock
@ stub _o__fseeki64
@ stub _o__fseeki64_nolock
@ stub _o__fsopen
@ stub _o__fstat32
@ stub _o__fstat32i64
@ stub _o__fstat64
@ stub _o__fstat64i32
@ stub _o__ftell_nolock
@ stub _o__ftelli64
@ stub _o__ftelli64_nolock
@ stub _o__ftime32
@ stub _o__ftime32_s
@ stub _o__ftime64
@ stub _o__ftime64_s
@ stub _o__fullpath
@ stub _o__futime32
@ stub _o__futime64
@ stub _o__fwrite_nolock
@ stub _o__gcvt
@ stub _o__gcvt_s
@ stub _o__get_daylight
@ stub _o__get_doserrno
@ stub _o__get_dstbias
@ stub _o__get_errno
@ stub _o__get_fmode
@ stub _o__get_heap_handle
@ stub _o__get_invalid_parameter_handler
@ stub _o__get_narrow_winmain_command_line
@ stub _o__get_osfhandle
@ stub _o__get_pgmptr
@ stub _o__get_stream_buffer_pointers
@ stub _o__get_terminate
@ stub _o__get_thread_local_invalid_parameter_handler
@ stub _o__get_timezone
@ stub _o__get_tzname
@ stub _o__get_wide_winmain_command_line
@ stub _o__get_wpgmptr
@ stub _o__getc_nolock
@ stub _o__getch
@ stub _o__getch_nolock
@ stub _o__getche
@ stub _o__getche_nolock
@ stub _o__getcwd
@ stub _o__getdcwd
@ stub _o__getdiskfree
@ stub _o__getdllprocaddr
@ stub _o__getdrive
@ stub _o__getdrives
@ stub _o__getmbcp
@ stub _o__getsystime
@ stub _o__getw
@ stub _o__getwc_nolock
@ stub _o__getwch
@ stub _o__getwch_nolock
@ stub _o__getwche
@ stub _o__getwche_nolock
@ stub _o__getws
@ stub _o__getws_s
@ stub _o__gmtime32
@ stub _o__gmtime32_s
@ stub _o__gmtime64
@ stub _o__gmtime64_s
@ stub _o__heapchk
@ stub _o__heapmin
@ stub _o__hypot
@ stub _o__hypotf
@ stub _o__i64toa
@ stub _o__i64toa_s
@ stub _o__i64tow
@ stub _o__i64tow_s
@ stub _o__initialize_onexit_table
@ stub _o__invalid_parameter_noinfo
@ stub _o__invalid_parameter_noinfo_noreturn
@ stub _o__isatty
@ stub _o__isctype
@ stub _o__isctype_l
@ stub _o__isleadbyte_l
@ stub _o__ismbbalnum
@ stub _o__ismbbalnum_l
@ stub _o__ismbbalpha
@ stub _o__ismbbalpha_l
@ stub _o__ismbbblank
@ stub _o__ismbbblank_l
@ stub _o__ismbbgraph
@ stub _o__ismbbgraph_l
@ stub _o__ismbbkalnum
@ stub _o__ismbbkalnum_l
@ stub _o__ismbbkana
@ stub _o__ismbbkana_l
@ stub _o__ismbbkprint
@ stub _o__ismbbkprint_l
@ stub _o__ismbbkpunct
@ stub _o__ismbbkpunct_l
@ stub _o__ismbblead
@ stub _o__ismbblead_l
@ stub _o__ismbbprint
@ stub _o__ismbbprint_l
@ stub _o__ismbbpunct
@ stub _o__ismbbpunct_l
@ stub _o__ismbbtrail
@ stub _o__ismbbtrail_l
@ stub _o__ismbcalnum
@ stub _o__ismbcalnum_l
@ stub _o__ismbcalpha
@ stub _o__ismbcalpha_l
@ stub _o__ismbcblank
@ stub _o__ismbcblank_l
@ stub _o__ismbcdigit
@ stub _o__ismbcdigit_l
@ stub _o__ismbcgraph
@ stub _o__ismbcgraph_l
@ stub _o__ismbchira
@ stub _o__ismbchira_l
@ stub _o__ismbckata
@ stub _o__ismbckata_l
@ stub _o__ismbcl0
@ stub _o__ismbcl0_l
@ stub _o__ismbcl1
@ stub _o__ismbcl1_l
@ stub _o__ismbcl2
@ stub _o__ismbcl2_l
@ stub _o__ismbclegal
@ stub _o__ismbclegal_l
@ stub _o__ismbclower
@ stub _o__ismbclower_l
@ stub _o__ismbcprint
@ stub _o__ismbcprint_l
@ stub _o__ismbcpunct
@ stub _o__ismbcpunct_l
@ stub _o__ismbcspace
@ stub _o__ismbcspace_l
@ stub _o__ismbcsymbol
@ stub _o__ismbcsymbol_l
@ stub _o__ismbcupper
@ stub _o__ismbcupper_l
@ stub _o__ismbslead
@ stub _o__ismbslead_l
@ stub _o__ismbstrail
@ stub _o__ismbstrail_l
@ stub _o__iswctype_l
@ stub _o__itoa
@ stub _o__itoa_s
@ stub _o__itow
@ stub _o__itow_s
@ stub _o__j0
@ stub _o__j1
@ stub _o__jn
@ stub _o__kbhit
@ stub _o__ld_int
@ stub _o__ldclass
@ stub _o__ldexp
@ stub _o__ldlog
@ stub _o__ldpcomp
@ stub _o__ldpoly
@ stub _o__ldscale
@ stub _o__ldsign
@ stub _o__ldsin
@ stub _o__ldtest
@ stub _o__ldunscale
@ stub _o__lfind
@ stub _o__lfind_s
@ stub _o__libm_sse2_acos_precise
@ stub _o__libm_sse2_asin_precise
@ stub _o__libm_sse2_atan_precise
@ stub _o__libm_sse2_cos_precise
@ stub _o__libm_sse2_exp_precise
@ stub _o__libm_sse2_log10_precise
@ stub _o__libm_sse2_log_precise
@ stub _o__libm_sse2_pow_precise
@ stub _o__libm_sse2_sin_precise
@ stub _o__libm_sse2_sqrt_precise
@ stub _o__libm_sse2_tan_precise
@ stub _o__loaddll
@ stub _o__localtime32
@ stub _o__localtime32_s
@ stub _o__localtime64
@ stub _o__localtime64_s
@ stub _o__lock_file
@ stub _o__locking
@ stub _o__logb
@ stub _o__logbf
@ stub _o__lsearch
@ stub _o__lsearch_s
@ stub _o__lseek
@ stub _o__lseeki64
@ stub _o__ltoa
@ stub _o__ltoa_s
@ stub _o__ltow
@ stub _o__ltow_s
@ stub _o__makepath
@ stub _o__makepath_s
@ stub _o__malloc_base
@ stub _o__mbbtombc
@ stub _o__mbbtombc_l
@ stub _o__mbbtype
@ stub _o__mbbtype_l
@ stub _o__mbccpy
@ stub _o__mbccpy_l
@ stub _o__mbccpy_s
@ stub _o__mbccpy_s_l
@ stub _o__mbcjistojms
@ stub _o__mbcjistojms_l
@ stub _o__mbcjmstojis
@ stub _o__mbcjmstojis_l
@ stub _o__mbclen
@ stub _o__mbclen_l
@ stub _o__mbctohira
@ stub _o__mbctohira_l
@ stub _o__mbctokata
@ stub _o__mbctokata_l
@ stub _o__mbctolower
@ stub _o__mbctolower_l
@ stub _o__mbctombb
@ stub _o__mbctombb_l
@ stub _o__mbctoupper
@ stub _o__mbctoupper_l
@ stub _o__mblen_l
@ stub _o__mbsbtype
@ stub _o__mbsbtype_l
@ stub _o__mbscat_s
@ stub _o__mbscat_s_l
@ stub _o__mbschr
@ stub _o__mbschr_l
@ stub _o__mbscmp
@ stub _o__mbscmp_l
@ stub _o__mbscoll
@ stub _o__mbscoll_l
@ stub _o__mbscpy_s
@ stub _o__mbscpy_s_l
@ stub _o__mbscspn
@ stub _o__mbscspn_l
@ stub _o__mbsdec
@ stub _o__mbsdec_l
@ stub _o__mbsicmp
@ stub _o__mbsicmp_l
@ stub _o__mbsicoll
@ stub _o__mbsicoll_l
@ stub _o__mbsinc
@ stub _o__mbsinc_l
@ stub _o__mbslen
@ stub _o__mbslen_l
@ stub _o__mbslwr
@ stub _o__mbslwr_l
@ stub _o__mbslwr_s
@ stub _o__mbslwr_s_l
@ stub _o__mbsnbcat
@ stub _o__mbsnbcat_l
@ stub _o__mbsnbcat_s
@ stub _o__mbsnbcat_s_l
@ stub _o__mbsnbcmp
@ stub _o__mbsnbcmp_l
@ stub _o__mbsnbcnt
@ stub _o__mbsnbcnt_l
@ stub _o__mbsnbcoll
@ stub _o__mbsnbcoll_l
@ stub _o__mbsnbcpy
@ stub _o__mbsnbcpy_l
@ stub _o__mbsnbcpy_s
@ stub _o__mbsnbcpy_s_l
@ stub _o__mbsnbicmp
@ stub _o__mbsnbicmp_l
@ stub _o__mbsnbicoll
@ stub _o__mbsnbicoll_l
@ stub _o__mbsnbset
@ stub _o__mbsnbset_l
@ stub _o__mbsnbset_s
@ stub _o__mbsnbset_s_l
@ stub _o__mbsncat
@ stub _o__mbsncat_l
@ stub _o__mbsncat_s
@ stub _o__mbsncat_s_l
@ stub _o__mbsnccnt
@ stub _o__mbsnccnt_l
@ stub _o__mbsncmp
@ stub _o__mbsncmp_l
@ stub _o__mbsncoll
@ stub _o__mbsncoll_l
@ stub _o__mbsncpy
@ stub _o__mbsncpy_l
@ stub _o__mbsncpy_s
@ stub _o__mbsncpy_s_l
@ stub _o__mbsnextc
@ stub _o__mbsnextc_l
@ stub _o__mbsnicmp
@ stub _o__mbsnicmp_l
@ stub _o__mbsnicoll
@ stub _o__mbsnicoll_l
@ stub _o__mbsninc
@ stub _o__mbsninc_l
@ stub _o__mbsnlen
@ stub _o__mbsnlen_l
@ stub _o__mbsnset
@ stub _o__mbsnset_l
@ stub _o__mbsnset_s
@ stub _o__mbsnset_s_l
@ stub _o__mbspbrk
@ stub _o__mbspbrk_l
@ stub _o__mbsrchr
@ stub _o__mbsrchr_l
@ stub _o__mbsrev
@ stub _o__mbsrev_l
@ stub _o__mbsset
@ stub _o__mbsset_l
@ stub _o__mbsset_s
@ stub _o__mbsset_s_l
@ stub _o__mbsspn
@ stub _o__mbsspn_l
@ stub _o__mbsspnp
@ stub _o__mbsspnp_l
@ stub _o__mbsstr
@ stub _o__mbsstr_l
@ stub _o__mbstok
@ stub _o__mbstok_l
@ stub _o__mbstok_s
@ stub _o__mbstok_s_l
@ stub _o__mbstowcs_l
@ stub _o__mbstowcs_s_l
@ stub _o__mbstrlen
@ stub _o__mbstrlen_l
@ stub _o__mbstrnlen
@ stub _o__mbstrnlen_l
@ stub _o__mbsupr
@ stub _o__mbsupr_l
@ stub _o__mbsupr_s
@ stub _o__mbsupr_s_l
@ stub _o__mbtowc_l
@ stub _o__memicmp
@ stub _o__memicmp_l
@ stub _o__mkdir
@ stub _o__mkgmtime32
@ stub _o__mkgmtime64
@ stub _o__mktemp
@ stub _o__mktemp_s
@ stub _o__mktime32
@ stub _o__mktime64
@ stub _o__msize
@ stub _o__nextafter
@ stub _o__nextafterf
@ stub _o__open_osfhandle
@ stub _o__pclose
@ stub _o__pipe
@ stub _o__popen
@ stub _o__putc_nolock
@ stub _o__putch
@ stub _o__putch_nolock
@ stub _o__putenv
@ stub _o__putenv_s
@ stub _o__putw
@ stub _o__putwc_nolock
@ stub _o__putwch
@ stub _o__putwch_nolock
@ stub _o__putws
@ stub _o__read
@ stub _o__realloc_base
@ stub _o__recalloc
@ stub _o__register_onexit_function
@ stub _o__resetstkoflw
@ stub _o__rmdir
@ stub _o__rmtmp
@ stub _o__scalb
@ stub _o__scalbf
@ stub _o__searchenv
@ stub _o__searchenv_s
@ stub _o__set_abort_behavior
@ stub _o__set_doserrno
@ stub _o__set_errno
@ stub _o__set_invalid_parameter_handler
@ stub _o__set_new_handler
@ stub _o__set_new_mode
@ stub _o__set_thread_local_invalid_parameter_handler
@ stub _o__seterrormode
@ stub _o__setmbcp
@ stub _o__setmode
@ stub _o__setsystime
@ stub _o__sleep
@ stub _o__sopen
@ stub _o__sopen_dispatch
@ stub _o__sopen_s
@ stub _o__spawnv
@ stub _o__spawnve
@ stub _o__spawnvp
@ stub _o__spawnvpe
@ stub _o__splitpath
@ stub _o__splitpath_s
@ stub _o__stat32
@ stub _o__stat32i64
@ stub _o__stat64
@ stub _o__stat64i32
@ stub _o__strcoll_l
@ stub _o__strdate
@ stub _o__strdate_s
@ stub _o__strdup
@ stub _o__strerror
@ stub _o__strerror_s
@ stub _o__strftime_l
@ stub _o__stricmp
@ stub _o__stricmp_l
@ stub _o__stricoll
@ stub _o__stricoll_l
@ stub _o__strlwr
@ stub _o__strlwr_l
@ stub _o__strlwr_s
@ stub _o__strlwr_s_l
@ stub _o__strncoll
@ stub _o__strncoll_l
@ stub _o__strnicmp
@ stub _o__strnicmp_l
@ stub _o__strnicoll
@ stub _o__strnicoll_l
@ stub _o__strnset_s
@ stub _o__strset_s
@ stub _o__strtime
@ stub _o__strtime_s
@ stub _o__strtod_l
@ stub _o__strtof_l
@ stub _o__strtoi64
@ stub _o__strtoi64_l
@ stub _o__strtol_l
@ stub _o__strtold_l
@ stub _o__strtoll_l
@ stub _o__strtoui64
@ stub _o__strtoui64_l
@ stub _o__strtoul_l
@ stub _o__strtoull_l
@ stub _o__strupr
@ stub _o__strupr_l
@ stub _o__strupr_s
@ stub _o__strupr_s_l
@ stub _o__strxfrm_l
@ stub _o__swab
@ stub _o__tell
@ stub _o__telli64
@ stub _o__timespec32_get
@ stub _o__timespec64_get
@ stub _o__tolower
@ stub _o__tolower_l
@ stub _o__toupper
@ stub _o__toupper_l
@ stub _o__towlower_l
@ stub _o__towupper_l
@ stub _o__tzset
@ stub _o__ui64toa
@ stub _o__ui64toa_s
@ stub _o__ui64tow
@ stub _o__ui64tow_s
@ stub _o__ultoa
@ stub _o__ultoa_s
@ stub _o__ultow
@ stub _o__ultow_s
@ stub _o__umask
@ stub _o__umask_s
@ stub _o__ungetc_nolock
@ stub _o__ungetch
@ stub _o__ungetch_nolock
@ stub _o__ungetwc_nolock
@ stub _o__ungetwch
@ stub _o__ungetwch_nolock
@ stub _o__unlink
@ stub _o__unloaddll
@ stub _o__unlock_file
@ stub _o__utime32
@ stub _o__utime64
@ stub _o__waccess
@ stub _o__waccess_s
@ stub _o__wasctime
@ stub _o__wasctime_s
@ stub _o__wchdir
@ stub _o__wchmod
@ stub _o__wcreat
@ stub _o__wcreate_locale
@ stub _o__wcscoll_l
@ stub _o__wcsdup
@ stub _o__wcserror
@ stub _o__wcserror_s
@ stub _o__wcsftime_l
@ stub _o__wcsicmp
@ stub _o__wcsicmp_l
@ stub _o__wcsicoll
@ stub _o__wcsicoll_l
@ stub _o__wcslwr
@ stub _o__wcslwr_l
@ stub _o__wcslwr_s
@ stub _o__wcslwr_s_l
@ stub _o__wcsncoll
@ stub _o__wcsncoll_l
@ stub _o__wcsnicmp
@ stub _o__wcsnicmp_l
@ stub _o__wcsnicoll
@ stub _o__wcsnicoll_l
@ stub _o__wcsnset
@ stub _o__wcsnset_s
@ stub _o__wcsset
@ stub _o__wcsset_s
@ stub _o__wcstod_l
@ stub _o__wcstof_l
@ stub _o__wcstoi64
@ stub _o__wcstoi64_l
@ stub _o__wcstol_l
@ stub _o__wcstold_l
@ stub _o__wcstoll_l
@ stub _o__wcstombs_l
@ stub _o__wcstombs_s_l
@ stub _o__wcstoui64
@ stub _o__wcstoui64_l
@ stub _o__wcstoul_l
@ stub _o__wcstoull_l
@ stub _o__wcsupr
@ stub _o__wcsupr_l
@ stub _o__wcsupr_s
@ stub _o__wcsupr_s_l
@ stub _o__wcsxfrm_l
@ stub _o__wctime32
@ stub _o__wctime32_s
@ stub _o__wctime64
@ stub _o__wctime64_s
@ stub _o__wctomb_l
@ stub _o__wctomb_s_l
@ stub _o__wdupenv_s
@ stub _o__wexecv
@ stub _o__wexecve
@ stub _o__wexecvp
@ stub _o__wexecvpe
@ stub _o__wfdopen
@ stub _o__wfindfirst32
@ stub _o__wfindfirst32i64
@ stub _o__wfindfirst64
@ stub _o__wfindfirst64i32
@ stub _o__wfindnext32
@ stub _o__wfindnext32i64
@ stub _o__wfindnext64
@ stub _o__wfindnext64i32
@ stub _o__wfopen
@ stub _o__wfopen_s
@ stub _o__wfreopen
@ stub _o__wfreopen_s
@ stub _o__wfsopen
@ stub _o__wfullpath
@ stub _o__wgetcwd
@ stub _o__wgetdcwd
@ stub _o__wgetenv
@ stub _o__wgetenv_s
@ stub _o__wmakepath
@ stub _o__wmakepath_s
@ stub _o__wmkdir
@ stub _o__wmktemp
@ stub _o__wmktemp_s
@ stub _o__wperror
@ stub _o__wpopen
@ stub _o__wputenv
@ stub _o__wputenv_s
@ stub _o__wremove
@ stub _o__wrename
@ stub _o__write
@ stub _o__wrmdir
@ stub _o__wsearchenv
@ stub _o__wsearchenv_s
@ stub _o__wsetlocale
@ stub _o__wsopen_dispatch
@ stub _o__wsopen_s
@ stub _o__wspawnv
@ stub _o__wspawnve
@ stub _o__wspawnvp
@ stub _o__wspawnvpe
@ stub _o__wsplitpath
@ stub _o__wsplitpath_s
@ stub _o__wstat32
@ stub _o__wstat32i64
@ stub _o__wstat64
@ stub _o__wstat64i32
@ stub _o__wstrdate
@ stub _o__wstrdate_s
@ stub _o__wstrtime
@ stub _o__wstrtime_s
@ stub _o__wsystem
@ stub _o__wtmpnam_s
@ stub _o__wtof
@ stub _o__wtof_l
@ stub _o__wtoi
@ stub _o__wtoi64
@ stub _o__wtoi64_l
@ stub _o__wtoi_l
@ stub _o__wtol
@ stub _o__wtol_l
@ stub _o__wtoll
@ stub _o__wtoll_l
@ stub _o__wunlink
@ stub _o__wutime32
@ stub _o__wutime64
@ stub _o__y0
@ stub _o__y1
@ stub _o__yn
@ stub _o_abort
@ stub _o_acos
@ stub _o_acosf
@ stub _o_acosh
@ stub _o_acoshf
@ stub _o_acoshl
@ stub _o_asctime
@ stub _o_asctime_s
@ stub _o_asin
@ stub _o_asinf
@ stub _o_asinh
@ stub _o_asinhf
@ stub _o_asinhl
@ stub _o_atan
@ stub _o_atan2
@ stub _o_atan2f
@ stub _o_atanf
@ stub _o_atanh
@ stub _o_atanhf
@ stub _o_atanhl
@ stub _o_atof
@ stub _o_atoi
@ stub _o_atol
@ stub _o_atoll
@ stub _o_bsearch
@ stub _o_bsearch_s
@ stub _o_btowc
@ stub _o_calloc
@ stub _o_cbrt
@ stub _o_cbrtf
@ stub _o_ceil
@ stub _o_ceilf
@ stub _o_clearerr
@ stub _o_clearerr_s
@ stub _o_cos
@ stub _o_cosf
@ stub _o_cosh
@ stub _o_coshf
@ stub _o_erf
@ stub _o_erfc
@ stub _o_erfcf
@ stub _o_erfcl
@ stub _o_erff
@ stub _o_erfl
@ stub _o_exp
@ stub _o_exp2
@ stub _o_exp2f
@ stub _o_exp2l
@ stub _o_expf
@ stub _o_fabs
@ stub _o_fclose
@ stub _o_feof
@ stub _o_ferror
@ stub _o_fflush
@ stub _o_fgetc
@ stub _o_fgetpos
@ stub _o_fgets
@ stub _o_fgetwc
@ stub _o_fgetws
@ stub _o_floor
@ stub _o_floorf
@ stub _o_fma
@ stub _o_fmaf
@ stub _o_fmal
@ stub _o_fmod
@ stub _o_fmodf
@ stub _o_fopen
@ stub _o_fopen_s
@ stub _o_fputc
@ stub _o_fputs
@ stub _o_fputwc
@ stub _o_fputws
@ stub _o_fread
@ stub _o_fread_s
@ stub _o_free
@ stub _o_freopen
@ stub _o_freopen_s
@ stub _o_frexp
@ stub _o_fseek
@ stub _o_fsetpos
@ stub _o_ftell
@ stub _o_fwrite
@ stub _o_getc
@ stub _o_getchar
@ stub _o_getenv
@ stub _o_getenv_s
@ stub _o_gets
@ stub _o_gets_s
@ stub _o_getwc
@ stub _o_getwchar
@ stub _o_hypot
@ stub _o_is_wctype
@ stub _o_isalnum
@ stub _o_isalpha
@ stub _o_isblank
@ stub _o_iscntrl
@ stub _o_isdigit
@ stub _o_isgraph
@ stub _o_isleadbyte
@ stub _o_islower
@ stub _o_isprint
@ stub _o_ispunct
@ stub _o_isspace
@ stub _o_isupper
@ stub _o_iswalnum
@ stub _o_iswalpha
@ stub _o_iswascii
@ stub _o_iswblank
@ stub _o_iswcntrl
@ stub _o_iswctype
@ stub _o_iswdigit
@ stub _o_iswgraph
@ stub _o_iswlower
@ stub _o_iswprint
@ stub _o_iswpunct
@ stub _o_iswspace
@ stub _o_iswupper
@ stub _o_iswxdigit
@ stub _o_isxdigit
@ stub _o_ldexp
@ stub _o_lgamma
@ stub _o_lgammaf
@ stub _o_lgammal
@ stub _o_llrint
@ stub _o_llrintf
@ stub _o_llrintl
@ stub _o_llround
@ stub _o_llroundf
@ stub _o_llroundl
@ stub _o_localeconv
@ stub _o_log
@ stub _o_log10
@ stub _o_log10f
@ stub _o_log1p
@ stub _o_log1pf
@ stub _o_log1pl
@ stub _o_log2
@ stub _o_log2f
@ stub _o_log2l
@ stub _o_logb
@ stub _o_logbf
@ stub _o_logbl
@ stub _o_logf
@ stub _o_lrint
@ stub _o_lrintf
@ stub _o_lrintl
@ stub _o_lround
@ stub _o_lroundf
@ stub _o_lroundl
@ stub _o_malloc
@ stub _o_mblen
@ stub _o_mbrlen
@ stub _o_mbrtoc16
@ stub _o_mbrtoc32
@ stub _o_mbrtowc
@ stub _o_mbsrtowcs
@ stub _o_mbsrtowcs_s
@ stub _o_mbstowcs
@ stub _o_mbstowcs_s
@ stub _o_mbtowc
@ stub _o_memset
@ stub _o_modf
@ stub _o_modff
@ stub _o_nan
@ stub _o_nanf
@ stub _o_nanl
@ stub _o_nearbyint
@ stub _o_nearbyintf
@ stub _o_nearbyintl
@ stub _o_nextafter
@ stub _o_nextafterf
@ stub _o_nextafterl
@ stub _o_nexttoward
@ stub _o_nexttowardf
@ stub _o_nexttowardl
@ stub _o_pow
@ stub _o_powf
@ stub _o_putc
@ stub _o_putchar
@ stub _o_puts
@ stub _o_putwc
@ stub _o_putwchar
@ stub _o_qsort
@ stub _o_qsort_s
@ stub _o_raise
@ stub _o_rand
@ stub _o_rand_s
@ stub _o_realloc
@ stub _o_remainder
@ stub _o_remainderf
@ stub _o_remainderl
@ stub _o_remove
@ stub _o_remquo
@ stub _o_remquof
@ stub _o_remquol
@ stub _o_rewind
@ stub _o_rint
@ stub _o_rintf
@ stub _o_rintl
@ stub _o_round
@ stub _o_roundf
@ stub _o_roundl
@ stub _o_scalbln
@ stub _o_scalblnf
@ stub _o_scalblnl
@ stub _o_scalbn
@ stub _o_scalbnf
@ stub _o_scalbnl
@ stub _o_set_terminate
@ stub _o_setbuf
@ stub _o_setvbuf
@ stub _o_sin
@ stub _o_sinf
@ stub _o_sinh
@ stub _o_sinhf
@ stub _o_sqrt
@ stub _o_sqrtf
@ stub _o_srand
@ stub _o_strcat_s
@ stub _o_strcoll
@ stub _o_strcpy_s
@ stub _o_strerror
@ stub _o_strerror_s
@ stub _o_strftime
@ stub _o_strncat_s
@ stub _o_strncpy_s
@ stub _o_strtod
@ stub _o_strtof
@ stub _o_strtok
@ stub _o_strtok_s
@ stub _o_strtol
@ stub _o_strtold
@ stub _o_strtoll
@ stub _o_strtoul
@ stub _o_strtoull
@ stub _o_system
@ stub _o_tan
@ stub _o_tanf
@ stub _o_tanh
@ stub _o_tanhf
@ stub _o_terminate
@ stub _o_tgamma
@ stub _o_tgammaf
@ stub _o_tgammal
@ stub _o_tmpfile_s
@ stub _o_tmpnam_s
@ stub _o_tolower
@ stub _o_toupper
@ stub _o_towlower
@ stub _o_towupper
@ stub _o_ungetc
@ stub _o_ungetwc
@ stub _o_wcrtomb
@ stub _o_wcrtomb_s
@ stub _o_wcscat_s
@ stub _o_wcscoll
@ stub _o_wcscpy
@ stub _o_wcscpy_s
@ stub _o_wcsftime
@ stub _o_wcsncat_s
@ stub _o_wcsncpy_s
@ stub _o_wcsrtombs
@ stub _o_wcsrtombs_s
@ stub _o_wcstod
@ stub _o_wcstof
@ stub _o_wcstok
@ stub _o_wcstok_s
@ stub _o_wcstol
@ stub _o_wcstold
@ stub _o_wcstoll
@ stub _o_wcstombs
@ stub _o_wcstombs_s
@ stub _o_wcstoul
@ stub _o_wcstoull
@ stub _o_wctob
@ stub _o_wctomb
@ stub _o_wctomb_s
@ stub _o_wmemcpy_s
@ stub _o_wmemmove_s
@ stub _purecall ;() ucrtbase._purecall
@ stub -arch=i386 _seh_longjmp_unwind ;(ptr) ucrtbase._seh_longjmp_unwind
@ stub -arch=i386 _seh_longjmp_unwind4 ;(ptr) ucrtbase._seh_longjmp_unwind4
@ stub _set_purecall_handler ;(ptr) ucrtbase._set_purecall_handler
@ stub _set_se_translator ;(ptr) ucrtbase._set_se_translator
@ cdecl -arch=i386 -norelay _setjmp3(ptr long)
@ cdecl -arch=i386,x86_64,arm longjmp(ptr long) 
@ cdecl memchr(ptr long long) 
@ cdecl memcmp(ptr ptr long) 
@ cdecl memcpy(ptr ptr long) 
@ cdecl memmove(ptr ptr long) 
@ stub set_unexpected ;(ptr) ucrtbase.set_unexpected
@ stub -arch=arm,x86_64 -norelay -private setjmp ;(ptr) ucrtbase.setjmp
@ cdecl strchr(str long)
@ cdecl strrchr(str long)
@ cdecl strstr(str str) 
@ stub unexpected
@ cdecl wcschr(wstr long)
@ cdecl wcsrchr(wstr long)
@ cdecl wcsstr(wstr wstr)
@ stub _beep ;(long long) ucrtbase._beep
@ stub _cwait ;(ptr long long) ucrtbase._cwait
@ stub _execl ;(str str) ucrtbase._execl
@ stub _execle ;(str str) ucrtbase._execle
@ stub _execlp ;(str str) ucrtbase._execlp
@ stub _execlpe ;(str str) ucrtbase._execlpe
@ stub _execv ;(str ptr) ucrtbase._execv
@ stub _execve ;(str ptr ptr) ucrtbase._execve
@ stub _execvp ;(str ptr) ucrtbase._execvp
@ stub _execvpe ;(str ptr ptr) ucrtbase._execvpe
@ stub _loaddll ;(str) ucrtbase._loaddll
@ stub _spawnl ;(long str str) ucrtbase._spawnl
@ stub _spawnle ;(long str str) ucrtbase._spawnle
@ stub _spawnlp ;(long str str) ucrtbase._spawnlp
@ stub _spawnlpe ;(long str str) ucrtbase._spawnlpe
@ stub _spawnv ;(long str ptr) ucrtbase._spawnv
@ stub _spawnve ;(long str ptr ptr) ucrtbase._spawnve
@ stub _spawnvp ;(long str ptr) ucrtbase._spawnvp
@ stub _spawnvpe ;(long str ptr ptr) ucrtbase._spawnvpe
@ stub _unloaddll ;(long) ucrtbase._unloaddll
@ stub _wexecl ;(wstr wstr) ucrtbase._wexecl
@ stub _wexecle ;(wstr wstr) ucrtbase._wexecle
@ stub _wexeclp ;(wstr wstr) ucrtbase._wexeclp
@ stub _wexeclpe ;(wstr wstr) ucrtbase._wexeclpe
@ stub _wexecv ;(wstr ptr) ucrtbase._wexecv
@ stub _wexecve ;(wstr ptr ptr) ucrtbase._wexecve
@ stub _wexecvp ;(wstr ptr) ucrtbase._wexecvp
@ stub _wexecvpe ;(wstr ptr ptr) ucrtbase._wexecvpe
@ stub _wspawnl ;(long wstr wstr) ucrtbase._wspawnl
@ stub _wspawnle ;(long wstr wstr) ucrtbase._wspawnle
@ stub _wspawnlp ;(long wstr wstr) ucrtbase._wspawnlp
@ stub _wspawnlpe ;(long wstr wstr) ucrtbase._wspawnlpe
@ stub _wspawnv ;(long wstr ptr) ucrtbase._wspawnv
@ stub _wspawnve ;(long wstr ptr ptr) ucrtbase._wspawnve
@ stub _wspawnvp ;(long wstr ptr) ucrtbase._wspawnvp
@ stub _wspawnvpe ;(long wstr ptr ptr) ucrtbase._wspawnvpe
@ stub _Exit ;(long) ucrtbase._Exit
@ stub -arch=i386 __control87_2 ;(long long ptr ptr) ucrtbase.__control87_2
@ stub __doserrno ;() ucrtbase.__doserrno
@ stub __fpe_flt_rounds ;() ucrtbase.__fpe_flt_rounds
@ stub __fpecode ;() ucrtbase.__fpecode
@ stub __p___argc ;() ucrtbase.__p___argc
@ stub __p___argv ;() ucrtbase.__p___argv
@ stub __p___wargv ;() ucrtbase.__p___wargv
@ stub __p__acmdln ;() ucrtbase.__p__acmdln
@ stub __p__pgmptr ;() ucrtbase.__p__pgmptr
@ stub __p__wcmdln ;() ucrtbase.__p__wcmdln
@ stub __p__wpgmptr ;() ucrtbase.__p__wpgmptr
@ stub __pxcptinfoptrs ;() ucrtbase.__pxcptinfoptrs
@ stub __sys_errlist ;() ucrtbase.__sys_errlist
@ stub __sys_nerr ;() ucrtbase.__sys_nerr
@ stub __threadhandle ;() ucrtbase.__threadhandle
@ stub __threadid ;() ucrtbase.__threadid
@ cdecl __wcserror(wstr) msvcrt.__wcserror
@ cdecl __wcserror_s(ptr long wstr) 
@ stub _assert ;(str str long) ucrtbase._assert
@ stub _beginthread ;(ptr long ptr) ucrtbase._beginthread
@ stub _beginthreadex ;(ptr long ptr ptr long ptr) ucrtbase._beginthreadex
@ stub _c_exit ;() ucrtbase._c_exit
@ stub _cexit ;() ucrtbase._cexit
@ stub _clearfp ;() ucrtbase._clearfp
@ stub _configure_narrow_argv ;(long) ucrtbase._configure_narrow_argv
@ stub _configure_wide_argv ;(long) ucrtbase._configure_wide_argv
@ stub _control87 ;(long long) ucrtbase._control87
@ stub _controlfp ;(long long) ucrtbase._controlfp
@ stub _controlfp_s ;(ptr long long) ucrtbase._controlfp_s
@ stub _crt_at_quick_exit ;(long) ucrtbase._crt_at_quick_exit
@ stub _crt_atexit ;(ptr) ucrtbase._crt_atexit
@ stub _crt_debugger_hook ;(long) ucrtbase._crt_debugger_hook
@ stub _endthread ;() ucrtbase._endthread
@ stub _endthreadex ;(long) ucrtbase._endthreadex
@ cdecl _errno() 
@ stub _execute_onexit_table ;(ptr) ucrtbase._execute_onexit_table
@ stub _exit ;(long) ucrtbase._exit
@ stub -arch=i386,x86_64,arm _fpieee_flt ;(long ptr ptr) ucrtbase._fpieee_flt
@ stub _fpreset ;() ucrtbase._fpreset
@ stub _get_doserrno ;(ptr) ucrtbase._get_doserrno
@ stub _get_errno ;(ptr) ucrtbase._get_errno
@ stub _get_initial_narrow_environment ;() ucrtbase._get_initial_narrow_environment
@ stub _get_initial_wide_environment ;() ucrtbase._get_initial_wide_environment
@ stub _get_invalid_parameter_handler ;() ucrtbase._get_invalid_parameter_handler
@ stub _get_narrow_winmain_command_line ;() ucrtbase._get_narrow_winmain_command_line
@ stub _get_pgmptr ;(ptr) ucrtbase._get_pgmptr
@ stub _get_terminate ;() ucrtbase._get_terminate
@ stub _get_thread_local_invalid_parameter_handler ;() ucrtbase._get_thread_local_invalid_parameter_handler
@ stub _get_wide_winmain_command_line ;() ucrtbase._get_wide_winmain_command_line
@ stub _get_wpgmptr ;(ptr) ucrtbase._get_wpgmptr
@ stub _getdllprocaddr ;(long str long) ucrtbase._getdllprocaddr
@ stub _getpid ;() ucrtbase._getpid
@ stub _initialize_narrow_environment ;() ucrtbase._initialize_narrow_environment
@ stub _initialize_onexit_table ;(ptr) ucrtbase._initialize_onexit_table
@ stub _initialize_wide_environment ;() ucrtbase._initialize_wide_environment
@ stub _initterm ;(ptr ptr) ucrtbase._initterm
@ stub _initterm_e ;(ptr ptr) ucrtbase._initterm_e
@ stub _invalid_parameter_noinfo ;() ucrtbase._invalid_parameter_noinfo
@ stub _invalid_parameter_noinfo_noreturn ;() ucrtbase._invalid_parameter_noinfo_noreturn
@ stub _invoke_watson ;() ucrtbase._invoke_watson
@ stub _query_app_type ;() ucrtbase._query_app_type
@ stub _register_onexit_function ;(ptr ptr) ucrtbase._register_onexit_function
@ stub _register_thread_local_exe_atexit_callback ;(long) ucrtbase._register_thread_local_exe_atexit_callback
@ stub _resetstkoflw ;() ucrtbase._resetstkoflw
@ stub -arch=i386,x86_64,arm _seh_filter_dll ;(long ptr) ucrtbase._seh_filter_dll
@ stub _seh_filter_exe ;(long ptr) ucrtbase._seh_filter_exe
@ stub _set_abort_behavior ;(long long) ucrtbase._set_abort_behavior
@ stub _set_app_type ;(long) ucrtbase._set_app_type
@ stub _set_controlfp ;(long long) ucrtbase._set_controlfp
@ stub _set_doserrno ;(long) ucrtbase._set_doserrno
@ stub _set_errno ;(long) ucrtbase._set_errno
@ stub _set_error_mode ;(long) ucrtbase._set_error_mode
@ stub _set_invalid_parameter_handler ;(ptr) ucrtbase._set_invalid_parameter_handler
@ stub _set_new_handler ;(ptr) ucrtbase._set_new_handler
@ stub _set_thread_local_invalid_parameter_handler ;(ptr) ucrtbase._set_thread_local_invalid_parameter_handler
@ stub _seterrormode ;(long) ucrtbase._seterrormode
@ stub _sleep ;(long) ucrtbase._sleep
@ stub _statusfp ;() ucrtbase._statusfp
@ stub -arch=i386 _statusfp2 ;(ptr ptr) ucrtbase._statusfp2
@ cdecl _strerror(long)
;@ cdecl _strerror_s(str long long)
@ stub _wassert ;(wstr wstr long) ucrtbase._wassert
@ cdecl _wcserror(long) 
@ stub _wcserror_s ;(ptr long long) ucrtbase._wcserror_s
@ stub _wperror ;(wstr) ucrtbase._wperror
@ stub _wsystem ;(wstr) ucrtbase._wsystem
@ stub abort ;() ucrtbase.abort
@ stub exit ;(long) ucrtbase.exit
@ stub feclearexcept ;() ucrtbase.feclearexcept
@ stub fegetenv ;(ptr) ucrtbase.fegetenv
@ stub fegetexceptflag ;() ucrtbase.fegetexceptflag
@ stub fegetround ;() ucrtbase.fegetround
@ stub feholdexcept ;() ucrtbase.feholdexcept
@ stub fesetenv ;() ucrtbase.fesetenv
@ stub fesetexceptflag ;() ucrtbase.fesetexceptflag
@ stub fesetround ;() ucrtbase.fesetround
@ stub fetestexcept ;() ucrtbase.fetestexcept
@ stub perror ;(str) ucrtbase.perror
@ stub quick_exit ;() ucrtbase.quick_exit
@ stub raise ;(long) ucrtbase.raise
@ stub set_terminate ;(ptr) ucrtbase.set_terminate
@ stub signal ;(long long) ucrtbase.signal
@ cdecl strerror(long)
@ stub strerror_s ;(ptr long long) ucrtbase.strerror_s
@ stub system ;(str) ucrtbase.system
@ stub terminate ;() ucrtbase.terminate
@ stub __acrt_iob_func ;(long) ucrtbase.__acrt_iob_func
@ stub __p__commode ;() ucrtbase.__p__commode
@ stub __p__fmode ;() ucrtbase.__p__fmode
@ stub __stdio_common_vfprintf ;(int64 ptr str ptr ptr) ucrtbase.__stdio_common_vfprintf
@ stub __stdio_common_vfprintf_p ;() ucrtbase.__stdio_common_vfprintf_p
@ stub __stdio_common_vfprintf_s ;() ucrtbase.__stdio_common_vfprintf_s
@ stub __stdio_common_vfscanf ;(int64 ptr str ptr ptr) ucrtbase.__stdio_common_vfscanf
@ stub __stdio_common_vfwprintf ;(int64 ptr wstr ptr ptr) ucrtbase.__stdio_common_vfwprintf
@ stub __stdio_common_vfwprintf_p ;() ucrtbase.__stdio_common_vfwprintf_p
@ stub __stdio_common_vfwprintf_s ;() ucrtbase.__stdio_common_vfwprintf_s
@ stub __stdio_common_vfwscanf ;(int64 ptr wstr ptr ptr) ucrtbase.__stdio_common_vfwscanf
@ stub __stdio_common_vsnprintf_s ;(int64 ptr long long str ptr ptr) ucrtbase.__stdio_common_vsnprintf_s
@ stub __stdio_common_vsnwprintf_s ;(int64 ptr long long wstr ptr ptr) ucrtbase.__stdio_common_vsnwprintf_s
@ stub __stdio_common_vsprintf ;(int64 ptr long str ptr ptr) ucrtbase.__stdio_common_vsprintf
@ stub __stdio_common_vsprintf_p ;() ucrtbase.__stdio_common_vsprintf_p
@ stub __stdio_common_vsprintf_s ;(int64 ptr long str ptr ptr) ucrtbase.__stdio_common_vsprintf_s
@ stub __stdio_common_vsscanf ;(int64 ptr long str ptr ptr) ucrtbase.__stdio_common_vsscanf
@ stub __stdio_common_vswprintf ;(int64 ptr long wstr ptr ptr) ucrtbase.__stdio_common_vswprintf
@ stub __stdio_common_vswprintf_p ;(int64 ptr long wstr ptr ptr) ucrtbase.__stdio_common_vswprintf_p
@ stub __stdio_common_vswprintf_s ;(int64 ptr long wstr ptr ptr) ucrtbase.__stdio_common_vswprintf_s
@ stub __stdio_common_vswscanf ;(int64 ptr long wstr ptr ptr) ucrtbase.__stdio_common_vswscanf
@ stub _chsize ;(long long) ucrtbase._chsize
@ stub _chsize_s ;(long int64) ucrtbase._chsize_s
@ stub _close ;(long) ucrtbase._close
@ stub _commit ;(long) ucrtbase._commit
@ stub _creat ;(str long) ucrtbase._creat
@ stub _dup ;(long) ucrtbase._dup
@ stub _dup2 ;(long long) ucrtbase._dup2
@ stub _eof ;(long) ucrtbase._eof
@ stub _fclose_nolock ;(ptr) ucrtbase._fclose_nolock
@ stub _fcloseall ;() ucrtbase._fcloseall
@ stub _fflush_nolock ;(ptr) ucrtbase._fflush_nolock
@ stub _fgetc_nolock ;(ptr) ucrtbase._fgetc_nolock
@ stub _fgetchar ;() ucrtbase._fgetchar
@ stub _fgetwc_nolock ;(ptr) ucrtbase._fgetwc_nolock
@ stub _fgetwchar ;() ucrtbase._fgetwchar
@ stub _filelength ;(long) ucrtbase._filelength
@ stub -ret64 _filelengthi64 ;(long) ucrtbase._filelengthi64
@ stub _fileno ;(ptr) ucrtbase._fileno
@ stub _flushall ;() ucrtbase._flushall
@ stub _fputc_nolock ;(long ptr) ucrtbase._fputc_nolock
@ stub _fputchar ;(long) ucrtbase._fputchar
@ stub _fputwc_nolock ;(long ptr) ucrtbase._fputwc_nolock
@ stub _fputwchar ;(long) ucrtbase._fputwchar
@ stub _fread_nolock ;(ptr long long ptr) ucrtbase._fread_nolock
@ stub _fread_nolock_s ;(ptr long long long ptr) ucrtbase._fread_nolock_s
@ stub _fseek_nolock ;(ptr long long) ucrtbase._fseek_nolock
@ stub _fseeki64 ;(ptr int64 long) ucrtbase._fseeki64
@ stub _fseeki64_nolock ;(ptr int64 long) ucrtbase._fseeki64_nolock
@ stub _fsopen ;(str str long) ucrtbase._fsopen
@ stub _ftell_nolock ;(ptr) ucrtbase._ftell_nolock
@ stub -ret64 _ftelli64 ;(ptr) ucrtbase._ftelli64
@ stub -ret64 _ftelli64_nolock ;(ptr) ucrtbase._ftelli64_nolock
@ stub _fwrite_nolock ;(ptr long long ptr) ucrtbase._fwrite_nolock
@ stub _get_fmode ;(ptr) ucrtbase._get_fmode
@ stub _get_osfhandle ;(long) ucrtbase._get_osfhandle
@ stub _get_printf_count_output ;() ucrtbase._get_printf_count_output
@ stub _get_stream_buffer_pointers ;(ptr ptr ptr ptr) ucrtbase._get_stream_buffer_pointers
@ stub _getc_nolock ;(ptr) ucrtbase._getc_nolock
@ stub _getcwd ;(str long) ucrtbase._getcwd
@ stub _getdcwd ;(long str long) ucrtbase._getdcwd
@ stub _getmaxstdio ;() ucrtbase._getmaxstdio
@ stub _getw ;(ptr) ucrtbase._getw
@ stub _getwc_nolock ;(ptr) ucrtbase._getwc_nolock
@ stub _getws ;(ptr) ucrtbase._getws
@ stub _getws_s ;() ucrtbase.
@ stub _isatty ;(long) ucrtbase._isatty
@ stub _kbhit ;() ucrtbase._kbhit
@ stub _locking ;(long long long) ucrtbase._locking
@ stub _lseek ;(long long long) ucrtbase._lseek
@ stub -ret64 _lseeki64 ;(long int64 long) ucrtbase._lseeki64
@ stub _mktemp ;(str) ucrtbase._mktemp
@ stub _mktemp_s ;(str long) ucrtbase._mktemp_s
@ stub _open ;(str long) ucrtbase._open
@ stub _open_osfhandle ;(long long) ucrtbase._open_osfhandle
@ stub _pclose ;(ptr) ucrtbase._pclose
@ stub _pipe ;(ptr long long) ucrtbase._pipe
@ stub _popen ;(str str) ucrtbase._popen
@ stub _putc_nolock ;(long ptr) ucrtbase._putc_nolock
@ stub _putw ;(long ptr) ucrtbase._putw
@ stub _putwc_nolock ;(long ptr) ucrtbase._putwc_nolock
@ stub _putws ;(wstr) ucrtbase._putws
@ stub _read ;(long ptr long) ucrtbase._read
@ stub _rmtmp ;() ucrtbase._rmtmp
@ stub _set_fmode ;(long) ucrtbase._set_fmode
@ stub _set_printf_count_output ;(long) ucrtbase._set_printf_count_output
@ stub _setmaxstdio ;(long) ucrtbase._setmaxstdio
@ stub _setmode ;(long long) ucrtbase._setmode
@ stub _sopen ;(str long long) ucrtbase._sopen
@ stub _sopen_dispatch ;() ucrtbase._sopen_dispatch
@ stub _sopen_s ;(ptr str long long long) ucrtbase._sopen_s
@ stub _tell ;(long) ucrtbase._tell
@ stub -ret64 _telli64 ;(long) ucrtbase._telli64
@ stub _tempnam ;(str str) ucrtbase._tempnam
@ stub _ungetc_nolock ;(long ptr) ucrtbase._ungetc_nolock
@ stub _ungetwc_nolock ;(long ptr) ucrtbase._ungetwc_nolock
@ stub _wcreat ;(wstr long) ucrtbase._wcreat
@ stub _wfdopen ;(long wstr) ucrtbase._wfdopen
@ stub _wfopen ;(wstr wstr) ucrtbase._wfopen
@ stub _wfopen_s ;(ptr wstr wstr) ucrtbase._wfopen_s
@ stub _wfreopen ;(wstr wstr ptr) ucrtbase._wfreopen
@ stub _wfreopen_s ;(ptr wstr wstr ptr) ucrtbase._wfreopen_s
@ stub _wfsopen ;(wstr wstr long) ucrtbase._wfsopen
@ stub _wmktemp ;(wstr) ucrtbase._wmktemp
@ stub _wmktemp_s ;(wstr long) ucrtbase._wmktemp_s
@ stub _wopen ;(wstr long) ucrtbase._wopen
@ stub _wpopen ;(wstr wstr) ucrtbase._wpopen
@ stub _write ;(long ptr long) ucrtbase._write
@ stub _wsopen ;(wstr long long) ucrtbase._wsopen
@ stub _wsopen_dispatch ;() ucrtbase._wsopen_dispatch
@ stub _wsopen_s ;(ptr wstr long long long) ucrtbase._wsopen_s
@ stub _wtempnam ;(wstr wstr) ucrtbase._wtempnam
@ stub _wtmpnam ;(ptr) ucrtbase._wtmpnam
@ stub _wtmpnam_s ;(ptr long) ucrtbase._wtmpnam_s
@ stub clearerr ;(ptr) ucrtbase.clearerr
@ stub clearerr_s ;() ucrtbase.clearerr_s
@ stub fclose ;(ptr) ucrtbase.fclose
@ stub feof ;(ptr) ucrtbase.feof
@ stub ferror ;(ptr) ucrtbase.ferror
@ stub fflush ;(ptr) ucrtbase.fflush
@ stub fgetc ;(ptr) ucrtbase.fgetc
@ stub fgetpos ;(ptr ptr) ucrtbase.fgetpos
@ stub fgets ;(ptr long ptr) ucrtbase.fgets
@ stub fgetwc ;(ptr) ucrtbase.fgetwc
@ stub fgetws ;(ptr long ptr) ucrtbase.fgetws
@ stub fopen ;(str str) ucrtbase.fopen
@ stub fopen_s ;(ptr str str) ucrtbase.fopen_s
@ stub fputc ;(long ptr) ucrtbase.fputc
@ stub fputs ;(str ptr) ucrtbase.fputs
@ stub fputwc ;(long ptr) ucrtbase.fputwc
@ stub fputws ;(wstr ptr) ucrtbase.fputws
@ stub fread ;(ptr long long ptr) ucrtbase.fread
@ stub fread_s ;(ptr long long long ptr) ucrtbase.fread_s
@ stub freopen ;(str str ptr) ucrtbase.freopen
@ stub freopen_s ;(ptr str str ptr) ucrtbase.freopen_s
@ stub fseek ;(ptr long long) ucrtbase.fseek
@ stub fsetpos ;(ptr ptr) ucrtbase.fsetpos
@ stub ftell ;(ptr) ucrtbase.ftell
@ stub fwrite ;(ptr long long ptr) ucrtbase.fwrite
@ stub getc ;(ptr) ucrtbase.getc
@ stub getchar ;() ucrtbase.getchar
@ stub gets ;(str) ucrtbase.gets
@ stub gets_s ;() ucrtbase.gets_s
@ stub getwc ;(ptr) ucrtbase.getwc
@ stub getwchar ;() ucrtbase.getwchar
@ stub putc ;(long ptr) ucrtbase.putc
@ stub putchar ;(long) ucrtbase.putchar
@ stub puts ;(str) ucrtbase.puts
@ stub putwc ;(long ptr) ucrtbase.putwc
@ stub putwchar ;(long) ucrtbase.putwchar
@ stub rewind ;(ptr) ucrtbase.rewind
@ stub setbuf ;(ptr ptr) ucrtbase.setbuf
@ stub setvbuf ;(ptr str long long) ucrtbase.setvbuf
@ stub tmpfile ;() ucrtbase.tmpfile
@ stub tmpfile_s ;(ptr) ucrtbase.tmpfile_s
@ stub tmpnam ;(ptr) ucrtbase.tmpnam
@ stub tmpnam_s ;(ptr long) ucrtbase.tmpnam_s
@ stub ungetc ;(long ptr) ucrtbase.ungetc
@ stub ungetwc ;(long ptr) ucrtbase.ungetwc
@ stub __isascii ;(long) ucrtbase.__isascii
@ stub __iscsym ;(long) ucrtbase.__iscsym
@ stub __iscsymf ;(long) ucrtbase.__iscsymf
@ stub __iswcsym ;() ucrtbase.__iswcsym
@ stub __iswcsymf ;() ucrtbase.__iswcsymf
@ stub __strncnt ;() ucrtbase.__strncnt
@ stub __wcsncnt ;() ucrtbase.__wcsncnt
@ stub _isalnum_l ;(long ptr) ucrtbase._isalnum_l
@ stub _isalpha_l ;(long ptr) ucrtbase._isalpha_l
@ stub _isblank_l ;() ucrtbase._isblank_l
@ stub _iscntrl_l ;(long ptr) ucrtbase._iscntrl_l
@ stub _isctype ;(long long) ucrtbase._isctype
@ stub _isctype_l ;(long long ptr) ucrtbase._isctype_l
@ stub _isdigit_l ;(long ptr) ucrtbase._isdigit_l
@ stub _isgraph_l ;(long ptr) ucrtbase._isgraph_l
@ stub _isleadbyte_l ;(long ptr) ucrtbase._isleadbyte_l
@ stub _islower_l ;(long ptr) ucrtbase._islower_l
@ stub _isprint_l ;(long ptr) ucrtbase._isprint_l
@ stub _ispunct_l ;() ucrtbase._ispunct_l
@ stub _isspace_l ;(long ptr) ucrtbase._isspace_l
@ stub _isupper_l ;(long ptr) ucrtbase._isupper_l
@ stub _iswalnum_l ;() ucrtbase._iswalnum_l
@ stub _iswalpha_l ;(long ptr) ucrtbase._iswalpha_l
@ stub _iswblank_l ;() ucrtbase._iswblank_l
@ stub _iswcntrl_l ;() ucrtbase._iswcntrl_l
@ stub _iswcsym_l ;() ucrtbase._iswcsym_l
@ stub _iswcsymf_l ;() ucrtbase._iswcsymf_l
@ stub _iswctype_l ;() ucrtbase._iswctype_l
@ stub _iswdigit_l ;(long ptr) ucrtbase._iswdigit_l
@ stub _iswgraph_l ;() ucrtbase._iswgraph_l
@ stub _iswlower_l ;() ucrtbase._iswlower_l
@ stub _iswprint_l ;() ucrtbase._iswprint_l
@ stub _iswpunct_l ;(long ptr) ucrtbase._iswpunct_l
@ stub _iswspace_l ;(long ptr) ucrtbase._iswspace_l
@ stub _iswupper_l ;() ucrtbase._iswupper_l
@ stub _iswxdigit_l ;() ucrtbase._iswxdigit_l
@ stub _isxdigit_l ;(long ptr) ucrtbase._isxdigit_l
@ cdecl _memccpy(ptr ptr long long)
@ stub _memicmp ;(str str long) ucrtbase._memicmp
@ stub _memicmp_l ;() ucrtbase._memicmp_l
@ stub _strcoll_l ;(str str ptr) ucrtbase._strcoll_l
@ cdecl _strdup(str) 
@ stub _stricmp ;(str str) ucrtbase._stricmp
@ stub _stricmp_l ;(str str ptr) ucrtbase._stricmp_l
@ stub _stricoll ;(str str) ucrtbase._stricoll
@ stub _stricoll_l ;(str str ptr) ucrtbase._stricoll_l
@ cdecl _strlwr(str) 
@ stub _strlwr_l ;(str ptr) ucrtbase._strlwr_l
@ stub _strlwr_s ;(ptr long) ucrtbase._strlwr_s
@ stub _strlwr_s_l ;(ptr long ptr) ucrtbase._strlwr_s_l
@ stub _strncoll ;(str str long) ucrtbase._strncoll
@ stub _strncoll_l ;(str str long ptr) ucrtbase._strncoll_l
@ stub _strnicmp ;(str str long) ucrtbase._strnicmp
@ stub _strnicmp_l ;(str str long ptr) ucrtbase._strnicmp_l
@ stub _strnicoll ;(str str long) ucrtbase._strnicoll
@ stub _strnicoll_l ;(str str long ptr) ucrtbase._strnicoll_l
@ cdecl _strnset(str long long) 
@ stub _strnset_s ;(str long long long) ucrtbase._strnset_s
@ cdecl _strrev(str) 
@ cdecl _strset(str long)
@ stub _strset_s ;() ucrtbase._strset_s
@ cdecl _strupr(str)
@ cdecl _strupr_l(str ptr) 
@ stub _strupr_s ;(str long) ucrtbase._strupr_s
@ stub _strupr_s_l ;(str long ptr) ucrtbase._strupr_s_l
@ stub _strxfrm_l ;(ptr str long ptr) ucrtbase._strxfrm_l
@ stub _tolower ;(long) ucrtbase._tolower
@ stub _tolower_l ;(long ptr) ucrtbase._tolower_l
@ stub _toupper ;(long) ucrtbase._toupper
@ stub _toupper_l ;(long ptr) ucrtbase._toupper_l
@ cdecl _towlower_l(long ptr) MSVCRT__towlower_l
@ cdecl _towupper_l(long ptr) MSVCRT__towupper_l
@ stub _wcscoll_l ;(wstr wstr ptr) ucrtbase._wcscoll_l
@ cdecl _wcsdup(wstr) 
@ stub _wcsicmp ;(wstr wstr) ucrtbase._wcsicmp
@ stub _wcsicmp_l ;(wstr wstr ptr) ucrtbase._wcsicmp_l
@ stub _wcsicoll ;(wstr wstr) ucrtbase._wcsicoll
@ stub _wcsicoll_l ;(wstr wstr ptr) ucrtbase._wcsicoll_l
@ cdecl _wcslwr(wstr)
@ cdecl _wcslwr_l(wstr ptr) MSVCRT__wcslwr_l
@ stub _wcslwr_s ;(wstr long) ucrtbase._wcslwr_s
@ stub _wcslwr_s_l ;(wstr long ptr) ucrtbase._wcslwr_s_l
@ stub _wcsncoll ;(wstr wstr long) ucrtbase._wcsncoll
@ stub _wcsncoll_l ;(wstr wstr long ptr) ucrtbase._wcsncoll_l
@ stub _wcsnicmp ;(wstr wstr long) ucrtbase._wcsnicmp
@ stub _wcsnicmp_l ;(wstr wstr long ptr) ucrtbase._wcsnicmp_l
@ stub _wcsnicoll ;(wstr wstr long) ucrtbase._wcsnicoll
@ stub _wcsnicoll_l ;(wstr wstr long ptr) ucrtbase._wcsnicoll_l
@ cdecl _wcsnset(wstr long long)
@ stub _wcsnset_s ;() ucrtbase._wcsnset_s
@ cdecl _wcsrev(wstr)
@ cdecl _wcsset(wstr long) 
@ stub _wcsset_s ;(wstr long long) ucrtbase._wcsset_s
@ cdecl _wcsupr(wstr)
@ cdecl _wcsupr_l(wstr ptr) MSVCRT__wcsupr_l
@ stub _wcsupr_s ;(wstr long) ucrtbase._wcsupr_s
@ stub _wcsupr_s_l ;(wstr long ptr) ucrtbase._wcsupr_s_l
@ stub _wcsxfrm_l ;(ptr wstr long ptr) ucrtbase._wcsxfrm_l
@ stub _wctype ;() ucrtbase._wctype
@ stub is_wctype ;(long long) ucrtbase.is_wctype
@ stub isalnum ;(long) ucrtbase.isalnum
@ stub isalpha ;(long) ucrtbase.isalpha
@ stub isblank ;() ucrtbase.isblank
@ stub iscntrl ;(long) ucrtbase.iscntrl
@ stub isdigit ;(long) ucrtbase.isdigit
@ stub isgraph ;(long) ucrtbase.isgraph
@ stub isleadbyte ;(long) ucrtbase.isleadbyte
@ stub islower ;(long) ucrtbase.islower
@ stub isprint ;(long) ucrtbase.isprint
@ stub ispunct ;(long) ucrtbase.ispunct
@ stub isspace ;(long) ucrtbase.isspace
@ stub isupper ;(long) ucrtbase.isupper
@ stub iswalnum ;(long) ucrtbase.iswalnum
@ stub iswalpha ;(long) ucrtbase.iswalpha
@ stub iswascii ;(long) ucrtbase.iswascii
@ stub iswblank ;() ucrtbase.iswblank
@ stub iswcntrl ;(long) ucrtbase.iswcntrl
@ stub iswctype ;(long long) ucrtbase.iswctype
@ stub iswdigit ;(long) ucrtbase.iswdigit
@ stub iswgraph ;(long) ucrtbase.iswgraph
@ stub iswlower ;(long) ucrtbase.iswlower
@ stub iswprint ;(long) ucrtbase.iswprint
@ stub iswpunct ;(long) ucrtbase.iswpunct
@ stub iswspace ;(long) ucrtbase.iswspace
@ stub iswupper ;(long) ucrtbase.iswupper
@ stub iswxdigit ;(long) ucrtbase.iswxdigit
@ stub isxdigit ;(long) ucrtbase.isxdigit
@ stub mblen ;(ptr long) ucrtbase.mblen
@ stub mbrlen ;(ptr long ptr) ucrtbase.mbrlen
@ stub memcpy_s ;(ptr long ptr long) ucrtbase.memcpy_s
@ stub memmove_s ;(ptr long ptr long) ucrtbase.memmove_s
@ cdecl memset(ptr long long) 
@ cdecl strcat(str str) 
@ stub strcat_s ;(str long str) ucrtbase.strcat_s
@ stub strcmp ;(str str) ucrtbase.strcmp
@ stub strcoll ;(str str) ucrtbase.strcoll
@ cdecl strcpy(ptr str) 
@ stub strcpy_s ;(ptr long str) ucrtbase.strcpy_s
@ stub strcspn ;(str str) ucrtbase.strcspn
@ stub strlen ;(str) ucrtbase.strlen
@ cdecl strncat(str str long)
@ stub strncat_s ;(str long str long) ucrtbase.strncat_s
@ stub strncmp ;(str str long) ucrtbase.strncmp
@ cdecl strncpy(ptr str long)
@ stub strncpy_s ;(ptr long str long) ucrtbase.strncpy_s
@ stub strnlen ;(str long) ucrtbase.strnlen
@ cdecl strpbrk(str str)
@ stub strspn ;(str str) ucrtbase.strspn
@ cdecl strtok(str str)
@ stub strtok_s ;(ptr str ptr) ucrtbase.strtok_s
@ stub strxfrm ;(ptr str long) ucrtbase.strxfrm
@ stub tolower ;(long) ucrtbase.tolower
@ stub toupper ;(long) ucrtbase.toupper
@ stub towctrans ;() ucrtbase.towctrans
@ cdecl towlower(long)
@ cdecl towupper(long)
@ cdecl wcscat(wstr wstr)
@ stub wcscat_s ;(wstr long wstr) ucrtbase.wcscat_s
@ stub wcscmp ;(wstr wstr) ucrtbase.wcscmp
@ stub wcscoll ;(wstr wstr) ucrtbase.wcscoll
@ cdecl wcscpy(ptr wstr)
@ stub wcscpy_s ;(ptr long wstr) ucrtbase.wcscpy_s
@ stub wcscspn ;(wstr wstr) ucrtbase.wcscspn
@ stub wcslen ;(wstr) ucrtbase.wcslen
@ cdecl wcsncat(wstr wstr long)
@ stub wcsncat_s ;(wstr long wstr long) ucrtbase.wcsncat_s
@ stub wcsncmp ;(wstr wstr long) ucrtbase.wcsncmp
@ cdecl wcsncpy(ptr wstr long)
@ stub wcsncpy_s ;(ptr long wstr long) ucrtbase.wcsncpy_s
@ stub wcsnlen ;(wstr long) ucrtbase.wcsnlen
@ cdecl wcspbrk(wstr wstr)
@ stub wcsspn ;(wstr wstr) ucrtbase.wcsspn
@ cdecl wcstok(wstr wstr)
@ stub wcstok_s ;(ptr wstr ptr) ucrtbase.wcstok_s
@ stub wcsxfrm ;(ptr wstr long) ucrtbase.wcsxfrm
@ stub wctype ;() ucrtbase.wctype
@ stub wmemcpy_s ;(ptr long ptr long) ucrtbase.wmemcpy_s
@ stub wmemmove_s ;(ptr long ptr long) ucrtbase.wmemmove_s
@ stub _Getdays ;() ucrtbase._Getdays
@ stub _Getmonths ;() ucrtbase._Getmonths
@ stub _Gettnames ;() ucrtbase._Gettnames
@ stub _Strftime ;(str long str ptr ptr) ucrtbase._Strftime
@ stub _W_Getdays ;() ucrtbase._W_Getdays
@ stub _W_Getmonths ;() ucrtbase._W_Getmonths
@ stub _W_Gettnames ;() ucrtbase._W_Gettnames
@ stub _Wcsftime ;(wstr long wstr lptr long) ucrtbase._Wcsftime
@ stub __daylight ;() ucrtbase.__daylight
@ stub __dstbias ;() ucrtbase.__dstbias
@ stub __timezone ;() ucrtbase.__timezone
@ stub __tzname ;() ucrtbase.__tzname
@ stub _ctime32 ;(ptr) ucrtbase._ctime32
@ stub _ctime32_s ;(str long ptr) ucrtbase._ctime32_s
@ stub _ctime64 ;(ptr) ucrtbase._ctime64
@ stub _ctime64_s ;(str long ptr) ucrtbase._ctime64_s
@ stub _difftime32 ;(long long) ucrtbase._difftime32
@ stub _difftime64 ;(long long) ucrtbase._difftime64
@ stub _ftime32 ;(ptr) ucrtbase._ftime32
@ stub _ftime32_s ;(ptr) ucrtbase._ftime32_s
@ stub _ftime64 ;(ptr) ucrtbase._ftime64
@ stub _ftime64_s ;(ptr) ucrtbase._ftime64_s
@ stub _futime32 ;(long ptr) ucrtbase._futime32
@ stub _futime64 ;(long ptr) ucrtbase._futime64
@ stub _get_daylight ;(ptr) ucrtbase._get_daylight
@ stub _get_dstbias ;(ptr) ucrtbase._get_dstbias
@ stub _get_timezone ;(ptr) ucrtbase._get_timezone
@ stub _get_tzname ;(ptr str long long) ucrtbase._get_tzname
@ stub _getsystime ;(ptr) ucrtbase._getsystime
@ stub _gmtime32 ;(ptr) ucrtbase._gmtime32
@ stub _gmtime32_s ;(ptr ptr) ucrtbase._gmtime32_s
@ stub _gmtime64 ;(ptr) ucrtbase._gmtime64
@ stub _gmtime64_s ;(ptr ptr) ucrtbase._gmtime64_s
@ stub _localtime32 ;(ptr) ucrtbase._localtime32
@ stub _localtime32_s ;(ptr ptr) ucrtbase._localtime32_s
@ stub _localtime64 ;(ptr) ucrtbase._localtime64
@ stub _localtime64_s ;(ptr ptr) ucrtbase._localtime64_s
@ stub _mkgmtime32 ;(ptr) ucrtbase._mkgmtime32
@ stub _mkgmtime64 ;(ptr) ucrtbase._mkgmtime64
@ stub _mktime32 ;(ptr) ucrtbase._mktime32
@ stub _mktime64 ;(ptr) ucrtbase._mktime64
@ stub _setsystime ;(ptr long) ucrtbase._setsystime
@ stub _strdate ;(ptr) ucrtbase._strdate
@ stub _strdate_s ;(ptr long) ucrtbase._strdate_s
@ stub _strftime_l ;(str long str ptr ptr) ucrtbase._strftime_l
@ stub _strtime ;(ptr) ucrtbase._strtime
@ stub _strtime_s ;(ptr long) ucrtbase._strtime_s
@ stub _time32 ;(ptr) ucrtbase._time32
@ stub _time64 ;(ptr) ucrtbase._time64
@ stub _timespec32_get ;(ptr long) ucrtbase._timespec32_get
@ stub _timespec64_get ;(ptr long) ucrtbase._timespec64_get
@ stub _tzset ;() ucrtbase._tzset
@ stub _utime32 ;(str ptr) ucrtbase._utime32
@ stub _utime64 ;(str ptr) ucrtbase._utime64
@ stub _wasctime ;(ptr) ucrtbase._wasctime
@ stub _wasctime_s ;(ptr long ptr) ucrtbase._wasctime_s
@ stub _wcsftime_l ;(ptr long wstr ptr ptr) ucrtbase._wcsftime_l
@ stub _wctime32 ;(ptr) ucrtbase._wctime32
@ stub _wctime32_s ;(ptr long ptr) ucrtbase._wctime32_s
@ stub _wctime64 ;(ptr) ucrtbase._wctime64
@ stub _wctime64_s ;(ptr long ptr) ucrtbase._wctime64_s
@ stub _wstrdate ;(ptr) ucrtbase._wstrdate
@ stub _wstrdate_s ;(ptr long) ucrtbase._wstrdate_s
@ stub _wstrtime ;(ptr) ucrtbase._wstrtime
@ stub _wstrtime_s ;(ptr long) ucrtbase._wstrtime_s
@ stub _wutime32 ;(wstr ptr) ucrtbase._wutime32
@ stub _wutime64 ;(wstr ptr) ucrtbase._wutime64
@ stub asctime ;(ptr) ucrtbase.asctime
@ stub asctime_s ;(ptr long ptr) ucrtbase.asctime_s
@ stub clock ;() ucrtbase.clock
@ stub strftime ;(str long str ptr) ucrtbase.strftime
@ stub wcsftime ;(ptr long wstr ptr) ucrtbase.wcsftime
@ cdecl -ret64 _abs64(int64)
@ cdecl _byteswap_uint64(int64)
@ cdecl _byteswap_ulong(long) 
@ cdecl _byteswap_ushort(long) 
@ stub _lfind ;(ptr ptr ptr long ptr) ucrtbase._lfind
@ stub _lfind_s ;() ucrtbase._lfind_s
@ cdecl _lrotl(long long)
@ cdecl _lrotr(long long)
@ stub _lsearch ;(ptr ptr ptr long ptr) ucrtbase._lsearch
@ stub _lsearch_s ;() ucrtbase._lsearch_s
@ cdecl _rotl(long long) 
@ cdecl -ret64 _rotl64(int64 long) 
@ cdecl _rotr(long long)
@ cdecl -ret64 _rotr64(int64 long) 
@ stub _swab ;(str str long) ucrtbase._swab
@ stub abs ;(long) ucrtbase.abs
@ stub bsearch ;(ptr ptr long long ptr) ucrtbase.bsearch
@ stub bsearch_s ;(ptr ptr long long ptr ptr) ucrtbase.bsearch_s
@ stub -ret64 div ;(long long) ucrtbase.div
@ stub imaxabs ;() ucrtbase.imaxabs
@ stub imaxdiv ;() ucrtbase.imaxdiv
@ stub labs ;(long) ucrtbase.labs
@ stub ldiv ;(long long) ucrtbase.ldiv
@ stub -ret64 llabs ;(int64) ucrtbase.llabs
@ stub lldiv ;() ucrtbase.lldiv
@ stub qsort ;(ptr long long ptr) ucrtbase.qsort
@ stub qsort_s ;(ptr long long ptr ptr) ucrtbase.qsort_s
@ stub rand ;() ucrtbase.rand
@ stub rand_s ;(ptr) ucrtbase.rand_s
@ stub srand ;(long) ucrtbase.srand

