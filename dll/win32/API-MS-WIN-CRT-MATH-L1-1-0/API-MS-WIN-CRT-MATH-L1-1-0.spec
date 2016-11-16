@ cdecl -arch=i386 _CIacos() ucrtbase._CIacos
@ cdecl -arch=i386 _CIasin() ucrtbase._CIasin
@ cdecl -arch=i386 _CIatan() ucrtbase._CIatan
@ cdecl -arch=i386 _CIatan2() ucrtbase._CIatan2
@ cdecl -arch=i386 _CIcos() ucrtbase._CIcos
@ cdecl -arch=i386 _CIcosh() ucrtbase._CIcosh
@ cdecl -arch=i386 _CIexp() ucrtbase._CIexp
@ cdecl -arch=i386 _CIfmod() ucrtbase._CIfmod
@ cdecl -arch=i386 _CIlog() ucrtbase._CIlog
@ cdecl -arch=i386 _CIlog10() ucrtbase._CIlog10
@ cdecl -arch=i386 _CIpow() ucrtbase._CIpow
@ cdecl -arch=i386 _CIsin() ucrtbase._CIsin
@ cdecl -arch=i386 _CIsinh() ucrtbase._CIsinh
@ cdecl -arch=i386 _CIsqrt() ucrtbase._CIsqrt
@ cdecl -arch=i386 _CItan() ucrtbase._CItan
@ cdecl -arch=i386 _CItanh() ucrtbase._CItanh
@ cdecl _Cbuild(long long long) ucrtbase._Cbuild
@ cdecl _Cmulcc(double double) ucrtbase._Cmulcc
@ cdecl _Cmulcr(long) ucrtbase._Cmulcr
@ cdecl _FCbuild(long long) ucrtbase._FCbuild
@ cdecl _FCmulcc(long long long long) ucrtbase._FCmulcc
@ cdecl _FCmulcr(long long long long) ucrtbase._FCmulcr
@ cdecl _LCbuild(long double long double) ucrtbase._LCbuild
@ cdecl _LCmulcc(long long) ucrtbase._LCmulcc
@ cdecl _LCmulcr(long str long long long double) ucrtbase._LCmulcr
@ cdecl -arch=i386 __libm_sse2_acos() ucrtbase.__libm_sse2_acos
@ cdecl -arch=i386 __libm_sse2_acosf() ucrtbase.__libm_sse2_acosf
@ cdecl -arch=i386 __libm_sse2_asin() ucrtbase.__libm_sse2_asin
@ cdecl -arch=i386 __libm_sse2_asinf() ucrtbase.__libm_sse2_asinf
@ cdecl -arch=i386 __libm_sse2_atan() ucrtbase.__libm_sse2_atan
@ cdecl -arch=i386 __libm_sse2_atan2() ucrtbase.__libm_sse2_atan2
@ cdecl -arch=i386 __libm_sse2_atanf() ucrtbase.__libm_sse2_atanf
@ cdecl -arch=i386 __libm_sse2_cos() ucrtbase.__libm_sse2_cos
@ cdecl -arch=i386 __libm_sse2_cosf() ucrtbase.__libm_sse2_cosf
@ cdecl -arch=i386 __libm_sse2_exp() ucrtbase.__libm_sse2_exp
@ cdecl -arch=i386 __libm_sse2_expf() ucrtbase.__libm_sse2_expf
@ cdecl -arch=i386 __libm_sse2_log() ucrtbase.__libm_sse2_log
@ cdecl -arch=i386 __libm_sse2_log10() ucrtbase.__libm_sse2_log10
@ cdecl -arch=i386 __libm_sse2_log10f() ucrtbase.__libm_sse2_log10f
@ cdecl -arch=i386 __libm_sse2_logf() ucrtbase.__libm_sse2_logf
@ cdecl -arch=i386 __libm_sse2_pow() ucrtbase.__libm_sse2_pow
@ cdecl -arch=i386 __libm_sse2_powf() ucrtbase.__libm_sse2_powf
@ cdecl -arch=i386 __libm_sse2_sin() ucrtbase.__libm_sse2_sin
@ cdecl -arch=i386 __libm_sse2_sinf() ucrtbase.__libm_sse2_sinf
@ cdecl -arch=i386 __libm_sse2_tan() ucrtbase.__libm_sse2_tan
@ cdecl -arch=i386 __libm_sse2_tanf() ucrtbase.__libm_sse2_tanf
@ cdecl __setusermatherr(ptr) ucrtbase.__setusermatherr
@ cdecl _cabs(long) ucrtbase._cabs
@ cdecl _chgsign(double) ucrtbase._chgsign
@ cdecl _chgsignf(float) ucrtbase._chgsignf
@ cdecl _copysign(double double) ucrtbase._copysign
@ cdecl _copysignf(float float) ucrtbase._copysignf
@ cdecl _d_int() ucrtbase._d_int
@ cdecl _dclass(double) ucrtbase._dclass
@ cdecl _dexp() ucrtbase._dexp
@ cdecl _dlog() ucrtbase._dlog
@ cdecl _dnorm() ucrtbase._dnorm
@ cdecl _dpcomp(double double) ucrtbase._dpcomp
@ cdecl _dpoly() ucrtbase._dpoly
@ cdecl _dscale() ucrtbase._dscale
@ cdecl _dsign(double) ucrtbase._dsign
@ cdecl _dsin() ucrtbase._dsin
@ cdecl _dtest(ptr) ucrtbase._dtest
@ cdecl _dunscale() ucrtbase._dunscale
@ cdecl _except1() ucrtbase._except1
@ cdecl _fd_int() ucrtbase._fd_int
@ cdecl _fdclass(float) ucrtbase._fdclass
@ cdecl _fdexp() ucrtbase._fdexp
@ cdecl _fdlog() ucrtbase._fdlog
@ cdecl _fdnorm() ucrtbase._fdnorm
@ cdecl _fdopen(long str) ucrtbase._fdopen
@ cdecl _fdpcomp(float float) ucrtbase._fdpcomp
@ cdecl _fdpoly() ucrtbase._fdpoly
@ cdecl _fdscale() ucrtbase._fdscale
@ cdecl _fdsign(float) ucrtbase._fdsign
@ cdecl _fdsin() ucrtbase._fdsin
@ cdecl _fdtest(ptr) ucrtbase._fdtest
@ cdecl _fdunscale() ucrtbase._fdunscale
@ cdecl _finite(double) ucrtbase._finite
@ cdecl -arch=arm,x86_64 _finitef(float) ucrtbase._finitef
@ cdecl _fpclass(double) ucrtbase._fpclass
@ cdecl _fpclassf() ucrtbase._fpclassf
@ cdecl -arch=i386 -ret64 _ftol() ucrtbase._ftol
@ cdecl _get_FMA3_enable() ucrtbase._get_FMA3_enable
@ cdecl _hypot(double double) ucrtbase._hypot
@ cdecl _hypotf(float float) ucrtbase._hypotf
@ cdecl _isnan(double) ucrtbase._isnan
@ cdecl -arch=arm,x86_64 _isnanf(float) ucrtbase._isnanf
@ cdecl _j0(double) ucrtbase._j0
@ cdecl _j1(double) ucrtbase._j1
@ cdecl _jn(long double) ucrtbase._jn
@ cdecl _ld_int() ucrtbase._ld_int
@ cdecl _ldclass(double) ucrtbase._ldclass
@ cdecl _ldexp() ucrtbase._ldexp
@ cdecl _ldlog() ucrtbase._ldlog
@ cdecl _ldpcomp(double double) ucrtbase._ldpcomp
@ cdecl _ldpoly() ucrtbase._ldpoly
@ cdecl _ldscale() ucrtbase._ldscale
@ cdecl _ldsign(double) ucrtbase._ldsign
@ cdecl _ldsin() ucrtbase._ldsin
@ cdecl _ldtest(ptr) ucrtbase._ldtest
@ cdecl _ldunscale() ucrtbase._ldunscale
@ cdecl -arch=i386  _libm_sse2_acos_precise() ucrtbase._libm_sse2_acos_precise
@ cdecl -arch=i386  _libm_sse2_asin_precise() ucrtbase._libm_sse2_asin_precise
@ cdecl -arch=i386  _libm_sse2_atan_precise() ucrtbase._libm_sse2_atan_precise
@ cdecl -arch=i386  _libm_sse2_cos_precise() ucrtbase._libm_sse2_cos_precise
@ cdecl -arch=i386  _libm_sse2_exp_precise() ucrtbase._libm_sse2_exp_precise
@ cdecl -arch=i386  _libm_sse2_log10_precise() ucrtbase._libm_sse2_log10_precise
@ cdecl -arch=i386  _libm_sse2_log_precise() ucrtbase._libm_sse2_log_precise
@ cdecl -arch=i386  _libm_sse2_pow_precise() ucrtbase._libm_sse2_pow_precise
@ cdecl -arch=i386  _libm_sse2_sin_precise() ucrtbase._libm_sse2_sin_precise
@ cdecl -arch=i386  _libm_sse2_sqrt_precise() ucrtbase._libm_sse2_sqrt_precise
@ cdecl -arch=i386  _libm_sse2_tan_precise() ucrtbase._libm_sse2_tan_precise
@ cdecl _logb(double) ucrtbase._logb
@ cdecl -arch=arm,x86_64 _logbf(float) ucrtbase._logbf
@ cdecl _nextafter(double double) ucrtbase._nextafter
@ cdecl -arch=arm,x86_64 _nextafterf(float float) ucrtbase._nextafterf
@ cdecl _scalb(double long) ucrtbase._scalb
@ cdecl -arch=arm,x86_64 _scalbf(float long) ucrtbase._scalbf
@ cdecl -arch=win64 _set_FMA3_enable(long) ucrtbase._set_FMA3_enable
@ cdecl -arch=i386 _set_SSE2_enable(long) ucrtbase._set_SSE2_enable
@ cdecl _y0(double) ucrtbase._y0
@ cdecl _y1(double) ucrtbase._y1
@ cdecl _yn(long double) ucrtbase._yn
@ cdecl acos(double) ucrtbase.acos
@ cdecl -arch=arm,x86_64 acosf(float) ucrtbase.acosf
@ cdecl acosh() ucrtbase.acosh
@ cdecl acoshf() ucrtbase.acoshf
@ cdecl acoshl() ucrtbase.acoshl
@ cdecl asin(double) ucrtbase.asin
@ cdecl -arch=arm,x86_64 asinf(float) ucrtbase.asinf
@ cdecl asinh(double double) ucrtbase.asinh
@ cdecl asinhf(float float) ucrtbase.asinhf
@ cdecl asinhl(double double) ucrtbase.asinhl
@ cdecl atan(double) ucrtbase.atan
@ cdecl atan2(double double) ucrtbase.atan2
@ cdecl -arch=arm,x86_64 atan2f(float float) ucrtbase.atan2f
@ cdecl -arch=arm,x86_64 atanf(float) ucrtbase.atanf
@ cdecl atanh() ucrtbase.atanh
@ cdecl atanhf() ucrtbase.atanhf
@ cdecl atanhl() ucrtbase.atanhl
@ cdecl cabs() ucrtbase.cabs
@ cdecl cabsf() ucrtbase.cabsf
@ cdecl cabsl() ucrtbase.cabsl
@ cdecl cacos() ucrtbase.cacos
@ cdecl cacosf() ucrtbase.cacosf
@ cdecl cacosh() ucrtbase.cacosh
@ cdecl cacoshf() ucrtbase.cacoshf
@ cdecl cacoshl() ucrtbase.cacoshl
@ cdecl cacosl() ucrtbase.cacosl
@ cdecl carg() ucrtbase.carg
@ cdecl cargf() ucrtbase.cargf
@ cdecl cargl() ucrtbase.cargl
@ cdecl casin() ucrtbase.casin
@ cdecl casinf() ucrtbase.casinf
@ cdecl casinh() ucrtbase.casinh
@ cdecl casinhf() ucrtbase.casinhf
@ cdecl casinhl() ucrtbase.casinhl
@ cdecl casinl() ucrtbase.casinl
@ cdecl catan() ucrtbase.catan
@ cdecl catanf() ucrtbase.catanf
@ cdecl catanh() ucrtbase.catanh
@ cdecl catanhf() ucrtbase.catanhf
@ cdecl catanhl() ucrtbase.catanhl
@ cdecl catanl() ucrtbase.catanl
@ cdecl cbrt(double) ucrtbase.cbrt
@ cdecl cbrtf(float) ucrtbase.cbrtf
@ cdecl cbrtl(double) ucrtbase.cbrtl
@ cdecl ccos() ucrtbase.ccos
@ cdecl ccosf() ucrtbase.ccosf
@ cdecl ccosh() ucrtbase.ccosh
@ cdecl ccoshf() ucrtbase.ccoshf
@ cdecl ccoshl() ucrtbase.ccoshl
@ cdecl ccosl() ucrtbase.ccosl
@ cdecl ceil(double) ucrtbase.ceil
@ cdecl -arch=arm,x86_64 ceilf(float) ucrtbase.ceilf
@ cdecl cexp() ucrtbase.cexp
@ cdecl cexpf() ucrtbase.cexpf
@ cdecl cexpl() ucrtbase.cexpl
@ cdecl cimag() ucrtbase.cimag
@ cdecl cimagf() ucrtbase.cimagf
@ cdecl cimagl() ucrtbase.cimagl
@ cdecl clog() ucrtbase.clog
@ cdecl clog10() ucrtbase.clog10
@ cdecl clog10f() ucrtbase.clog10f
@ cdecl clog10l() ucrtbase.clog10l
@ cdecl clogf() ucrtbase.clogf
@ cdecl clogl() ucrtbase.clogl
@ cdecl conj() ucrtbase.conj
@ cdecl conjf() ucrtbase.conjf
@ cdecl conjl() ucrtbase.conjl
@ cdecl copysign(double double) ucrtbase.copysign
@ cdecl copysignf(float float) ucrtbase.copysignf
@ cdecl copysignl(double double) ucrtbase.copysignl
@ cdecl cos(double) ucrtbase.cos
@ cdecl -arch=arm,x86_64 cosf(float) ucrtbase.cosf
@ cdecl cosh(double) ucrtbase.cosh
@ cdecl -arch=arm,x86_64 coshf(float) ucrtbase.coshf
@ cdecl cpow() ucrtbase.cpow
@ cdecl cpowf() ucrtbase.cpowf
@ cdecl cpowl() ucrtbase.cpowl
@ cdecl cproj() ucrtbase.cproj
@ cdecl cprojf() ucrtbase.cprojf
@ cdecl cprojl() ucrtbase.cprojl
@ cdecl creal() ucrtbase.creal
@ cdecl crealf() ucrtbase.crealf
@ cdecl creall() ucrtbase.creall
@ cdecl csin() ucrtbase.csin
@ cdecl csinf() ucrtbase.csinf
@ cdecl csinh() ucrtbase.csinh
@ cdecl csinhf() ucrtbase.csinhf
@ cdecl csinhl() ucrtbase.csinhl
@ cdecl csinl() ucrtbase.csinl
@ cdecl csqrt() ucrtbase.csqrt
@ cdecl csqrtf() ucrtbase.csqrtf
@ cdecl csqrtl() ucrtbase.csqrtl
@ cdecl ctan() ucrtbase.ctan
@ cdecl ctanf() ucrtbase.ctanf
@ cdecl ctanh() ucrtbase.ctanh
@ cdecl ctanhf() ucrtbase.ctanhf
@ cdecl ctanhl() ucrtbase.ctanhl
@ cdecl ctanl() ucrtbase.ctanl
@ cdecl erf(double) ucrtbase.erf
@ cdecl erfc() ucrtbase.erfc
@ cdecl erfcf() ucrtbase.erfcf
@ cdecl erfcl() ucrtbase.erfcl
@ cdecl erff(double) ucrtbase.erff
@ cdecl erfl(double) ucrtbase.erfl
@ cdecl exp(double) ucrtbase.exp
@ cdecl exp2(double) ucrtbase.exp2
@ cdecl exp2f(float) ucrtbase.exp2f
@ cdecl exp2l(double) ucrtbase.exp2l
@ cdecl -arch=arm,x86_64 expf(float) ucrtbase.expf
@ cdecl expm1() ucrtbase.expm1
@ cdecl expm1f() ucrtbase.expm1f
@ cdecl expm1l() ucrtbase.expm1l
@ cdecl fabs(double) ucrtbase.fabs
@ cdecl -arch=arm fabsf(float) ucrtbase.fabsf
@ cdecl fdim() ucrtbase.fdim
@ cdecl fdimf() ucrtbase.fdimf
@ cdecl fdiml() ucrtbase.fdiml
@ cdecl floor(double) ucrtbase.floor
@ cdecl -arch=arm,x86_64 floorf(float) ucrtbase.floorf
@ cdecl fma() ucrtbase.fma
@ cdecl fmaf() ucrtbase.fmaf
@ cdecl fmal() ucrtbase.fmal
@ cdecl fmax(double double) ucrtbase.fmax
@ cdecl fmaxf(float float) ucrtbase.fmaxf
@ cdecl fmaxl(double double) ucrtbase.fmaxl
@ cdecl fmin(double double) ucrtbase.fmin
@ cdecl fminf(float float) ucrtbase.fminf
@ cdecl fminl(double double) ucrtbase.fminl
@ cdecl fmod(double double) ucrtbase.fmod
@ cdecl -arch=arm,x86_64 fmodf(float float) ucrtbase.fmodf
@ cdecl frexp(double ptr) ucrtbase.frexp
@ cdecl hypot(double double) ucrtbase.hypot
@ cdecl ilogb() ucrtbase.ilogb
@ cdecl ilogbf() ucrtbase.ilogbf
@ cdecl ilogbl() ucrtbase.ilogbl
@ cdecl ldexp(double long) ucrtbase.ldexp
@ cdecl lgamma(double) ucrtbase.lgamma
@ cdecl lgammaf(float) ucrtbase.lgammaf
@ cdecl lgammal(double) ucrtbase.lgammal
@ cdecl -ret64 llrint(double) ucrtbase.llrint
@ cdecl -ret64 llrintf(float) ucrtbase.llrintf
@ cdecl -ret64 llrintl(double) ucrtbase.llrintl
@ cdecl -ret64 llround(double) ucrtbase.llround
@ cdecl -ret64 llroundf(float) ucrtbase.llroundf
@ cdecl -ret64 llroundl(double) ucrtbase.llroundl
@ cdecl log(double) ucrtbase.log
@ cdecl log10(double) ucrtbase.log10
@ cdecl -arch=arm,x86_64 log10f(float) ucrtbase.log10f
@ cdecl log1p() ucrtbase.log1p
@ cdecl log1pf() ucrtbase.log1pf
@ cdecl log1pl() ucrtbase.log1pl
@ cdecl log2(double) ucrtbase.log2
@ cdecl log2f(float) ucrtbase.log2f
@ cdecl log2l(double) ucrtbase.log2l
@ cdecl logb() ucrtbase.logb
@ cdecl logbf() ucrtbase.logbf
@ cdecl logbl() ucrtbase.logbl
@ cdecl -arch=arm,x86_64 logf(float) ucrtbase.logf
@ cdecl lrint(double) ucrtbase.lrint
@ cdecl lrintf(float) ucrtbase.lrintf
@ cdecl lrintl(double) ucrtbase.lrintl
@ cdecl lround(double) ucrtbase.lround
@ cdecl lroundf(float) ucrtbase.lroundf
@ cdecl lroundl(double) ucrtbase.lroundl
@ cdecl modf(double ptr) ucrtbase.modf
@ cdecl -arch=arm,x86_64 modff(float ptr) ucrtbase.modff
@ cdecl nan() ucrtbase.nan
@ cdecl nanf() ucrtbase.nanf
@ cdecl nanl() ucrtbase.nanl
@ cdecl nearbyint() ucrtbase.nearbyint
@ cdecl nearbyintf() ucrtbase.nearbyintf
@ cdecl nearbyintl() ucrtbase.nearbyintl
@ cdecl nextafter(double double) ucrtbase.nextafter
@ cdecl nextafterf(float float) ucrtbase.nextafterf
@ cdecl nextafterl(double double) ucrtbase.nextafterl
@ cdecl nexttoward() ucrtbase.nexttoward
@ cdecl nexttowardf() ucrtbase.nexttowardf
@ cdecl nexttowardl() ucrtbase.nexttowardl
@ cdecl norm() ucrtbase.norm
@ cdecl normf() ucrtbase.normf
@ cdecl norml() ucrtbase.norml
@ cdecl pow(double double) ucrtbase.pow
@ cdecl -arch=arm,x86_64 powf(float float) ucrtbase.powf
@ cdecl remainder(double double) ucrtbase.remainder
@ cdecl remainderf(float float) ucrtbase.remainderf
@ cdecl remainderl(double double) ucrtbase.remainderl
@ cdecl remquo() ucrtbase.remquo
@ cdecl remquof() ucrtbase.remquof
@ cdecl remquol() ucrtbase.remquol
@ cdecl rint(double) ucrtbase.rint
@ cdecl rintf(float) ucrtbase.rintf
@ cdecl rintl(double) ucrtbase.rintl
@ cdecl round(double) ucrtbase.round
@ cdecl roundf(float) ucrtbase.roundf
@ cdecl roundl(double) ucrtbase.roundl
@ cdecl scalbln(double long) ucrtbase.scalbln
@ cdecl scalblnf(float long) ucrtbase.scalblnf
@ cdecl scalblnl(double long) ucrtbase.scalblnl
@ cdecl scalbn(double long) ucrtbase.scalbn
@ cdecl scalbnf(float long) ucrtbase.scalbnf
@ cdecl scalbnl(double long) ucrtbase.scalbnl
@ cdecl sin(double) ucrtbase.sin
@ cdecl -arch=arm,x86_64 sinf(float) ucrtbase.sinf
@ cdecl sinh(double) ucrtbase.sinh
@ cdecl -arch=arm,x86_64 sinhf(float) ucrtbase.sinhf
@ cdecl sqrt(double) ucrtbase.sqrt
@ cdecl -arch=arm,x86_64 sqrtf(float) ucrtbase.sqrtf
@ cdecl tan(double) ucrtbase.tan
@ cdecl -arch=arm,x86_64 tanf(float) ucrtbase.tanf
@ cdecl tanh(double) ucrtbase.tanh
@ cdecl -arch=arm,x86_64 tanhf(float) ucrtbase.tanhf
@ cdecl tgamma() ucrtbase.tgamma
@ cdecl tgammaf() ucrtbase.tgammaf
@ cdecl tgammal() ucrtbase.tgammal
@ cdecl trunc(double) ucrtbase.trunc
@ cdecl truncf(float) ucrtbase.truncf
@ cdecl truncl(double) ucrtbase.truncl
