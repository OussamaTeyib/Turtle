# 0 "MauritaniaFlag.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "MauritaniaFlag.c"
# 1 "turtle.h" 1





typedef struct Turtle Turtle;

typedef void (*cmdFunction) (Turtle *, void *);

typedef struct {
    cmdFunction cmd;
    void *params;
} Command;

struct Turtle {
    HWND hwnd;
    HDC hdc;
    double angle;
    Command *cmdQueue;
    int nCmd;
    int maxCmd;
};



void init(Turtle *t);
void done(Turtle *t);
void PostCommand(Turtle *t, cmdFunction cmd, void *params);
void __forward(Turtle *t, void *params);
void __left(Turtle *t, void *params);
void __goto(Turtle *t, void *params);

typedef struct
{
    int distance;
} ForwardParams;
# 45 "turtle.h"
typedef struct
{
    double angle;
} LeftParams;
# 64 "turtle.h"
typedef struct
{
    int x, y;
} GotoParams;
# 2 "MauritaniaFlag.c" 2

int main(void)
{
    Turtle t;
    init(&t);

    do { GotoParams gotoParams = {(-150), (-50)}; PostCommand(&t, __goto, &gotoParams); } while (0);
    for (int i = 0; i < 4; i++)
    {
        do { ForwardParams forwardParams = {(300 - 100 * (i % 2))}; PostCommand(&t, __forward, &forwardParams); } while (0);
        do { LeftParams leftParams = {(90)}; PostCommand(&t, __left, &leftParams); } while (0);
    }

    do { GotoParams gotoParams = {(-150), (-20)}; PostCommand(&t, __goto, &gotoParams); } while (0);
    for (int i = 0; i < 3; i++)
    {
        do { ForwardParams forwardParams = {(300 -160 * (i%2))}; PostCommand(&t, __forward, &forwardParams); } while (0);
        do { LeftParams leftParams = {(90)}; PostCommand(&t, __left, &leftParams); } while (0);
    }

    do { GotoParams gotoParams = {(-20), (40)}; PostCommand(&t, __goto, &gotoParams); } while (0);
    do { LeftParams leftParams = {(126)}; PostCommand(&t, __left, &leftParams); } while (0);
    for (int i = 0; i < 5; i++)
    {
        do { ForwardParams forwardParams = {(60)}; PostCommand(&t, __forward, &forwardParams); } while (0);
        do { LeftParams leftParams = {(144)}; PostCommand(&t, __left, &leftParams); } while (0);
    }

    done(&t);
    return 0;
}
# 0 "turtle.c"
# 0 "<built-in>"
# 0 "<command-line>"
# 1 "turtle.c"
# 1 "turtle.h" 1





typedef struct Turtle Turtle;

typedef void (*cmdFunction) (Turtle *, void *);

typedef struct {
    cmdFunction cmd;
    void *params;
} Command;

struct Turtle {
    HWND hwnd;
    HDC hdc;
    double angle;
    Command *cmdQueue;
    int nCmd;
    int maxCmd;
};



void init(Turtle *t);
void done(Turtle *t);
void PostCommand(Turtle *t, cmdFunction cmd, void *params);
void __forward(Turtle *t, void *params);
void __left(Turtle *t, void *params);
void __goto(Turtle *t, void *params);

typedef struct
{
    int distance;
} ForwardParams;
# 45 "turtle.h"
typedef struct
{
    double angle;
} LeftParams;
# 64 "turtle.h"
typedef struct
{
    int x, y;
} GotoParams;
# 2 "turtle.c" 2
# 1 "C:/msys64/mingw64/include/math.h" 1 3
# 10 "C:/msys64/mingw64/include/math.h" 3
       
# 11 "C:/msys64/mingw64/include/math.h" 3


# 1 "C:/msys64/mingw64/include/crtdefs.h" 1 3
# 10 "C:/msys64/mingw64/include/crtdefs.h" 3
# 1 "C:/msys64/mingw64/include/corecrt.h" 1 3
# 10 "C:/msys64/mingw64/include/corecrt.h" 3
# 1 "C:/msys64/mingw64/include/_mingw.h" 1 3
# 10 "C:/msys64/mingw64/include/_mingw.h" 3
# 1 "C:/msys64/mingw64/include/_mingw_mac.h" 1 3
# 98 "C:/msys64/mingw64/include/_mingw_mac.h" 3
             
# 107 "C:/msys64/mingw64/include/_mingw_mac.h" 3
             
# 11 "C:/msys64/mingw64/include/_mingw.h" 2 3
# 1 "C:/msys64/mingw64/include/_mingw_secapi.h" 1 3
# 12 "C:/msys64/mingw64/include/_mingw.h" 2 3
# 282 "C:/msys64/mingw64/include/_mingw.h" 3
# 1 "C:/msys64/mingw64/include/vadefs.h" 1 3
# 9 "C:/msys64/mingw64/include/vadefs.h" 3
# 1 "C:/msys64/mingw64/include/_mingw.h" 1 3
# 661 "C:/msys64/mingw64/include/_mingw.h" 3
# 1 "C:/msys64/mingw64/include/sdks/_mingw_ddk.h" 1 3
# 662 "C:/msys64/mingw64/include/_mingw.h" 2 3
# 10 "C:/msys64/mingw64/include/vadefs.h" 2 3




#pragma pack(push,_CRT_PACKING)
# 24 "C:/msys64/mingw64/include/vadefs.h" 3
  
# 24 "C:/msys64/mingw64/include/vadefs.h" 3
 typedef __builtin_va_list __gnuc_va_list;






  typedef __gnuc_va_list va_list;
# 103 "C:/msys64/mingw64/include/vadefs.h" 3
#pragma pack(pop)
# 283 "C:/msys64/mingw64/include/_mingw.h" 2 3
# 580 "C:/msys64/mingw64/include/_mingw.h" 3
void __attribute__((__cdecl__)) __debugbreak(void);
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) void __attribute__((__cdecl__)) __debugbreak(void)
{

  __asm__ __volatile__("int {$}3":);







}
# 601 "C:/msys64/mingw64/include/_mingw.h" 3
void __attribute__((__cdecl__)) __attribute__ ((__noreturn__)) __fastfail(unsigned int code);
extern __inline__ __attribute__((__always_inline__,__gnu_inline__)) void __attribute__((__cdecl__)) __attribute__ ((__noreturn__)) __fastfail(unsigned int code)
{

  __asm__ __volatile__("int {$}0x29"::"c"(code));
# 615 "C:/msys64/mingw64/include/_mingw.h" 3
  __builtin_unreachable();
}
# 641 "C:/msys64/mingw64/include/_mingw.h" 3
const char *__mingw_get_crt_info (void);
# 11 "C:/msys64/mingw64/include/corecrt.h" 2 3




#pragma pack(push,_CRT_PACKING)
# 35 "C:/msys64/mingw64/include/corecrt.h" 3
__extension__ typedef unsigned long long size_t;
# 45 "C:/msys64/mingw64/include/corecrt.h" 3
__extension__ typedef long long ssize_t;






typedef size_t rsize_t;
# 62 "C:/msys64/mingw64/include/corecrt.h" 3
__extension__ typedef long long intptr_t;
# 75 "C:/msys64/mingw64/include/corecrt.h" 3
__extension__ typedef unsigned long long uintptr_t;
# 88 "C:/msys64/mingw64/include/corecrt.h" 3
__extension__ typedef long long ptrdiff_t;
# 98 "C:/msys64/mingw64/include/corecrt.h" 3
typedef unsigned short wchar_t;







typedef unsigned short wint_t;
typedef unsigned short wctype_t;





typedef int errno_t;




typedef long __time32_t;




__extension__ typedef long long __time64_t;
# 138 "C:/msys64/mingw64/include/corecrt.h" 3
typedef __time64_t time_t;
# 430 "C:/msys64/mingw64/include/corecrt.h" 3
struct threadlocaleinfostruct;
struct threadmbcinfostruct;
typedef struct threadlocaleinfostruct *pthreadlocinfo;
typedef struct threadmbcinfostruct *pthreadmbcinfo;
struct __lc_time_data;

typedef struct localeinfo_struct {
  pthreadlocinfo locinfo;
  pthreadmbcinfo mbcinfo;
} _locale_tstruct,*_locale_t;



typedef struct tagLC_ID {
  unsigned short wLanguage;
  unsigned short wCountry;
  unsigned short wCodePage;
} LC_ID,*LPLC_ID;




typedef struct threadlocaleinfostruct {





  int refcount;
  unsigned int lc_codepage;
  unsigned int lc_collate_cp;
  unsigned long lc_handle[6];
  LC_ID lc_id[6];
  struct {
    char *locale;
    wchar_t *wlocale;
    int *refcount;
    int *wrefcount;
  } lc_category[6];
  int lc_clike;
  int mb_cur_max;
  int *lconv_intl_refcount;
  int *lconv_num_refcount;
  int *lconv_mon_refcount;
  struct lconv *lconv;
  int *ctype1_refcount;
  unsigned short *ctype1;
  const unsigned short *pctype;
  const unsigned char *pclmap;
  const unsigned char *pcumap;
  struct __lc_time_data *lc_time_curr;

} threadlocinfo;
# 501 "C:/msys64/mingw64/include/corecrt.h" 3
#pragma pack(pop)
# 11 "C:/msys64/mingw64/include/crtdefs.h" 2 3
# 14 "C:/msys64/mingw64/include/math.h" 2 3

struct _exception;

#pragma pack(push,_CRT_PACKING)
# 119 "C:/msys64/mingw64/include/math.h" 3
  typedef union __mingw_dbl_type_t {
    double x;
    unsigned long long val;
    __extension__ struct {
      unsigned int low, high;
    } lh;
  } __mingw_dbl_type_t;

  typedef union __mingw_flt_type_t {
    float x;
    unsigned int val;
  } __mingw_flt_type_t;

  typedef union __mingw_ldbl_type_t
  {
    long double x;
    __extension__ struct {
      unsigned int low, high;
      int sign_exponent : 16;
      int res1 : 16;
      int res0 : 32;
    } lh;
  } __mingw_ldbl_type_t;
# 150 "C:/msys64/mingw64/include/math.h" 3
  extern double * __imp__HUGE;
# 163 "C:/msys64/mingw64/include/math.h" 3
  struct _exception {
    int type;
    const char *name;
    double arg1;
    double arg2;
    double retval;
  };

  void __mingw_raise_matherr (int typ, const char *name, double a1, double a2,
         double rslt);
  void __mingw_setusermatherr (int (__attribute__((__cdecl__)) *)(struct _exception *));
  __attribute__ ((__dllimport__)) void __setusermatherr(int (__attribute__((__cdecl__)) *)(struct _exception *));



  double __attribute__((__cdecl__)) sin(double _X);
  double __attribute__((__cdecl__)) cos(double _X);
  double __attribute__((__cdecl__)) tan(double _X);
  double __attribute__((__cdecl__)) sinh(double _X);
  double __attribute__((__cdecl__)) cosh(double _X);
  double __attribute__((__cdecl__)) tanh(double _X);
  double __attribute__((__cdecl__)) asin(double _X);
  double __attribute__((__cdecl__)) acos(double _X);
  double __attribute__((__cdecl__)) atan(double _X);
  double __attribute__((__cdecl__)) atan2(double _Y,double _X);
  double __attribute__((__cdecl__)) exp(double _X);
  double __attribute__((__cdecl__)) log(double _X);
  double __attribute__((__cdecl__)) log10(double _X);
  double __attribute__((__cdecl__)) pow(double _X,double _Y);
  double __attribute__((__cdecl__)) sqrt(double _X);
  double __attribute__((__cdecl__)) ceil(double _X);
  double __attribute__((__cdecl__)) floor(double _X);


  extern float __attribute__((__cdecl__)) fabsf (float x);
  extern long double __attribute__((__cdecl__)) fabsl (long double);
  extern double __attribute__((__cdecl__)) fabs (double _X);
# 238 "C:/msys64/mingw64/include/math.h" 3
  double __attribute__((__cdecl__)) ldexp(double _X,int _Y);
  double __attribute__((__cdecl__)) frexp(double _X,int *_Y);
  double __attribute__((__cdecl__)) modf(double _X,double *_Y);
  double __attribute__((__cdecl__)) fmod(double _X,double _Y);

  void __attribute__((__cdecl__)) sincos (double __x, double *p_sin, double *p_cos);
  void __attribute__((__cdecl__)) sincosl (long double __x, long double *p_sin, long double *p_cos);
  void __attribute__((__cdecl__)) sincosf (float __x, float *p_sin, float *p_cos);



  int __attribute__((__cdecl__)) abs(int _X);
  long __attribute__((__cdecl__)) labs(long _X);



  double __attribute__((__cdecl__)) atof(const char *_String);
  double __attribute__((__cdecl__)) _atof_l(const char *_String,_locale_t _Locale);
# 265 "C:/msys64/mingw64/include/math.h" 3
  struct _complex {
    double x;
    double y;
  };


  double __attribute__((__cdecl__)) _cabs(struct _complex _ComplexA);
  double __attribute__((__cdecl__)) _hypot(double _X,double _Y);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _j0(double _X);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _j1(double _X);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _jn(int _X,double _Y);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _y0(double _X);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _y1(double _X);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _yn(int _X,double _Y);


  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _matherr (struct _exception *);
# 292 "C:/msys64/mingw64/include/math.h" 3
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _chgsign (double _X);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _copysign (double _Number,double _Sign);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _logb (double);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _nextafter (double, double);
  __attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) _scalb (double, long);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _finite (double);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fpclass (double);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _isnan (double);






__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) j0 (double) ;
__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) j1 (double) ;
__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) jn (int, double) ;
__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) y0 (double) ;
__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) y1 (double) ;
__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) yn (int, double) ;

__attribute__ ((__dllimport__)) double __attribute__((__cdecl__)) chgsign (double);
# 322 "C:/msys64/mingw64/include/math.h" 3
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) finite (double);
  __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) fpclass (double);
# 367 "C:/msys64/mingw64/include/math.h" 3
typedef float float_t;
typedef double double_t;
# 402 "C:/msys64/mingw64/include/math.h" 3
  extern int __attribute__((__cdecl__)) __fpclassifyl (long double);
  extern int __attribute__((__cdecl__)) __fpclassifyf (float);
  extern int __attribute__((__cdecl__)) __fpclassify (double);
# 515 "C:/msys64/mingw64/include/math.h" 3
  extern int __attribute__((__cdecl__)) __isnan (double);
  extern int __attribute__((__cdecl__)) __isnanf (float);
  extern int __attribute__((__cdecl__)) __isnanl (long double);
# 602 "C:/msys64/mingw64/include/math.h" 3
  extern int __attribute__((__cdecl__)) __signbit (double);
  extern int __attribute__((__cdecl__)) __signbitf (float);
  extern int __attribute__((__cdecl__)) __signbitl (long double);
# 659 "C:/msys64/mingw64/include/math.h" 3
  extern float __attribute__((__cdecl__)) sinf(float _X);
  extern long double __attribute__((__cdecl__)) sinl(long double);

  extern float __attribute__((__cdecl__)) cosf(float _X);
  extern long double __attribute__((__cdecl__)) cosl(long double);

  extern float __attribute__((__cdecl__)) tanf(float _X);
  extern long double __attribute__((__cdecl__)) tanl(long double);
  extern float __attribute__((__cdecl__)) asinf(float _X);
  extern long double __attribute__((__cdecl__)) asinl(long double);

  extern float __attribute__((__cdecl__)) acosf (float);
  extern long double __attribute__((__cdecl__)) acosl (long double);

  extern float __attribute__((__cdecl__)) atanf (float);
  extern long double __attribute__((__cdecl__)) atanl (long double);

  extern float __attribute__((__cdecl__)) atan2f (float, float);
  extern long double __attribute__((__cdecl__)) atan2l (long double, long double);


  extern float __attribute__((__cdecl__)) sinhf(float _X);



  extern long double __attribute__((__cdecl__)) sinhl(long double);

  extern float __attribute__((__cdecl__)) coshf(float _X);



  extern long double __attribute__((__cdecl__)) coshl(long double);

  extern float __attribute__((__cdecl__)) tanhf(float _X);



  extern long double __attribute__((__cdecl__)) tanhl(long double);



  extern double __attribute__((__cdecl__)) acosh (double);
  extern float __attribute__((__cdecl__)) acoshf (float);
  extern long double __attribute__((__cdecl__)) acoshl (long double);


  extern double __attribute__((__cdecl__)) asinh (double);
  extern float __attribute__((__cdecl__)) asinhf (float);
  extern long double __attribute__((__cdecl__)) asinhl (long double);


  extern double __attribute__((__cdecl__)) atanh (double);
  extern float __attribute__((__cdecl__)) atanhf (float);
  extern long double __attribute__((__cdecl__)) atanhl (long double);



  extern float __attribute__((__cdecl__)) expf(float _X);



  extern long double __attribute__((__cdecl__)) expl(long double);


  extern double __attribute__((__cdecl__)) exp2(double);
  extern float __attribute__((__cdecl__)) exp2f(float);
  extern long double __attribute__((__cdecl__)) exp2l(long double);



  extern double __attribute__((__cdecl__)) expm1(double);
  extern float __attribute__((__cdecl__)) expm1f(float);
  extern long double __attribute__((__cdecl__)) expm1l(long double);


  extern float frexpf(float _X,int *_Y);



  extern long double __attribute__((__cdecl__)) frexpl(long double,int *);




  extern int __attribute__((__cdecl__)) ilogb (double);
  extern int __attribute__((__cdecl__)) ilogbf (float);
  extern int __attribute__((__cdecl__)) ilogbl (long double);


  extern float __attribute__((__cdecl__)) ldexpf(float _X,int _Y);



  extern long double __attribute__((__cdecl__)) ldexpl (long double, int);


  extern float __attribute__((__cdecl__)) logf (float);
  extern long double __attribute__((__cdecl__)) logl(long double);


  extern float __attribute__((__cdecl__)) log10f (float);
  extern long double __attribute__((__cdecl__)) log10l(long double);


  extern double __attribute__((__cdecl__)) log1p(double);
  extern float __attribute__((__cdecl__)) log1pf(float);
  extern long double __attribute__((__cdecl__)) log1pl(long double);


  extern double __attribute__((__cdecl__)) log2 (double);
  extern float __attribute__((__cdecl__)) log2f (float);
  extern long double __attribute__((__cdecl__)) log2l (long double);


  extern double __attribute__((__cdecl__)) logb (double);
  extern float __attribute__((__cdecl__)) logbf (float);
  extern long double __attribute__((__cdecl__)) logbl (long double);
# 862 "C:/msys64/mingw64/include/math.h" 3
  extern float __attribute__((__cdecl__)) modff (float, float*);
  extern long double __attribute__((__cdecl__)) modfl (long double, long double*);


  extern double __attribute__((__cdecl__)) scalbn (double, int);
  extern float __attribute__((__cdecl__)) scalbnf (float, int);
  extern long double __attribute__((__cdecl__)) scalbnl (long double, int);

  extern double __attribute__((__cdecl__)) scalbln (double, long);
  extern float __attribute__((__cdecl__)) scalblnf (float, long);
  extern long double __attribute__((__cdecl__)) scalblnl (long double, long);



  extern double __attribute__((__cdecl__)) cbrt (double);
  extern float __attribute__((__cdecl__)) cbrtf (float);
  extern long double __attribute__((__cdecl__)) cbrtl (long double);


  extern double __attribute__((__cdecl__)) hypot (double, double) ;
  extern float __attribute__((__cdecl__)) hypotf (float x, float y);



  extern long double __attribute__((__cdecl__)) hypotl (long double, long double);


  extern float __attribute__((__cdecl__)) powf(float _X,float _Y);



  extern long double __attribute__((__cdecl__)) powl (long double, long double);


  extern float __attribute__((__cdecl__)) sqrtf (float);
  extern long double sqrtl(long double);


  extern double __attribute__((__cdecl__)) erf (double);
  extern float __attribute__((__cdecl__)) erff (float);
  extern long double __attribute__((__cdecl__)) erfl (long double);


  extern double __attribute__((__cdecl__)) erfc (double);
  extern float __attribute__((__cdecl__)) erfcf (float);
  extern long double __attribute__((__cdecl__)) erfcl (long double);


  extern double __attribute__((__cdecl__)) lgamma (double);
  extern float __attribute__((__cdecl__)) lgammaf (float);
  extern long double __attribute__((__cdecl__)) lgammal (long double);

  extern int signgam;


  extern double __attribute__((__cdecl__)) tgamma (double);
  extern float __attribute__((__cdecl__)) tgammaf (float);
  extern long double __attribute__((__cdecl__)) tgammal (long double);


  extern float __attribute__((__cdecl__)) ceilf (float);
  extern long double __attribute__((__cdecl__)) ceill (long double);


  extern float __attribute__((__cdecl__)) floorf (float);
  extern long double __attribute__((__cdecl__)) floorl (long double);


  extern double __attribute__((__cdecl__)) nearbyint ( double);
  extern float __attribute__((__cdecl__)) nearbyintf (float);
  extern long double __attribute__((__cdecl__)) nearbyintl (long double);



extern double __attribute__((__cdecl__)) rint (double);
extern float __attribute__((__cdecl__)) rintf (float);
extern long double __attribute__((__cdecl__)) rintl (long double);


extern long __attribute__((__cdecl__)) lrint (double);
extern long __attribute__((__cdecl__)) lrintf (float);
extern long __attribute__((__cdecl__)) lrintl (long double);

__extension__ long long __attribute__((__cdecl__)) llrint (double);
__extension__ long long __attribute__((__cdecl__)) llrintf (float);
__extension__ long long __attribute__((__cdecl__)) llrintl (long double);
# 1029 "C:/msys64/mingw64/include/math.h" 3
  extern double __attribute__((__cdecl__)) round (double);
  extern float __attribute__((__cdecl__)) roundf (float);
  extern long double __attribute__((__cdecl__)) roundl (long double);


  extern long __attribute__((__cdecl__)) lround (double);
  extern long __attribute__((__cdecl__)) lroundf (float);
  extern long __attribute__((__cdecl__)) lroundl (long double);
  __extension__ long long __attribute__((__cdecl__)) llround (double);
  __extension__ long long __attribute__((__cdecl__)) llroundf (float);
  __extension__ long long __attribute__((__cdecl__)) llroundl (long double);



  extern double __attribute__((__cdecl__)) trunc (double);
  extern float __attribute__((__cdecl__)) truncf (float);
  extern long double __attribute__((__cdecl__)) truncl (long double);


  extern float __attribute__((__cdecl__)) fmodf (float, float);
  extern long double __attribute__((__cdecl__)) fmodl (long double, long double);


  extern double __attribute__((__cdecl__)) remainder (double, double);
  extern float __attribute__((__cdecl__)) remainderf (float, float);
  extern long double __attribute__((__cdecl__)) remainderl (long double, long double);


  extern double __attribute__((__cdecl__)) remquo(double, double, int *);
  extern float __attribute__((__cdecl__)) remquof(float, float, int *);
  extern long double __attribute__((__cdecl__)) remquol(long double, long double, int *);


  extern double __attribute__((__cdecl__)) copysign (double, double);
  extern float __attribute__((__cdecl__)) copysignf (float, float);
  extern long double __attribute__((__cdecl__)) copysignl (long double, long double);
# 1086 "C:/msys64/mingw64/include/math.h" 3
  extern double __attribute__((__cdecl__)) nan(const char *tagp);
  extern float __attribute__((__cdecl__)) nanf(const char *tagp);
  extern long double __attribute__((__cdecl__)) nanl(const char *tagp);
# 1097 "C:/msys64/mingw64/include/math.h" 3
  extern double __attribute__((__cdecl__)) nextafter (double, double);
  extern float __attribute__((__cdecl__)) nextafterf (float, float);
  extern long double __attribute__((__cdecl__)) nextafterl (long double, long double);


  extern double __attribute__((__cdecl__)) nexttoward (double, long double);
  extern float __attribute__((__cdecl__)) nexttowardf (float, long double);
  extern long double __attribute__((__cdecl__)) nexttowardl (long double, long double);



  extern double __attribute__((__cdecl__)) fdim (double x, double y);
  extern float __attribute__((__cdecl__)) fdimf (float x, float y);
  extern long double __attribute__((__cdecl__)) fdiml (long double x, long double y);







  extern double __attribute__((__cdecl__)) fmax (double, double);
  extern float __attribute__((__cdecl__)) fmaxf (float, float);
  extern long double __attribute__((__cdecl__)) fmaxl (long double, long double);


  extern double __attribute__((__cdecl__)) fmin (double, double);
  extern float __attribute__((__cdecl__)) fminf (float, float);
  extern long double __attribute__((__cdecl__)) fminl (long double, long double);



  extern double __attribute__((__cdecl__)) fma (double, double, double);
  extern float __attribute__((__cdecl__)) fmaf (float, float, float);
  extern long double __attribute__((__cdecl__)) fmal (long double, long double, long double);
# 1180 "C:/msys64/mingw64/include/math.h" 3
   __attribute__ ((__dllimport__)) float __attribute__((__cdecl__)) _copysignf (float _Number,float _Sign);
   __attribute__ ((__dllimport__)) float __attribute__((__cdecl__)) _chgsignf (float _X);
   __attribute__ ((__dllimport__)) float __attribute__((__cdecl__)) _logbf(float _X);
   __attribute__ ((__dllimport__)) float __attribute__((__cdecl__)) _nextafterf(float _X,float _Y);
   __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _finitef(float _X);
   __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _isnanf(float _X);
   __attribute__ ((__dllimport__)) int __attribute__((__cdecl__)) _fpclassf(float _X);



   extern long double __attribute__((__cdecl__)) _chgsignl (long double);
# 1580 "C:/msys64/mingw64/include/math.h" 3
#pragma pack(pop)
# 3 "turtle.c" 2


# 4 "turtle.c"
static void die(char *msg);
static void CreateCanvas(Turtle *t);
static void SetCentre(Turtle *t);
static void ExecuteCommands(Turtle *t);

static void die(char *msg)
{
    MessageBox(NULL, msg, "ERROR", MB_ICONEXCLAMATION | MB_OK);
    exit(EXIT_FAILURE);
}

void init(Turtle *t)
{
    t->angle = 0;

    t->maxCmd = 50;
    t->cmdQueue = malloc(t->maxCmd * sizeof (Command));
    t->nCmd = 0;
}

static void CreateCanvas(Turtle *t)
{
    char *className = "MainWindow";
    HINSTANCE hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = {0};
    wc.style = CS_VREDRAW | CS_HREDRAW;
    wc.lpfnWndProc = DefWindowProc;
    wc.hInstance = hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    wc.lpszClassName = className;

    if (!RegisterClass(&wc))
        die("Initialization Failed! \nClass NOT Registred.");

    HWND hwnd = CreateWindow(className, "Turtle in C", WS_OVERLAPPEDWINDOW, 250, 100, 820, 470, NULL, NULL, hInstance, NULL);

    if (!hwnd)
        die("Initialization Failed! \nWindow NOT Created");

    ShowWindow(hwnd, SW_SHOWDEFAULT);
    UpdateWindow(hwnd);

    t->hwnd = hwnd;
    t->hdc = GetDC(hwnd);
    SetCentre(t);

    ExecuteCommands(t);

    MSG msg;
    while (GetMessage(&msg, t->hwnd, 0, 0) > 0)
    {
        if (msg.message == WM_PAINT)
        {
            SetCentre(t);
            ExecuteCommands(t);
        }
        DispatchMessage(&msg);
    }

    ReleaseDC(t->hwnd, t->hdc);
    UnregisterClass(className, hInstance);
}

void done(Turtle *t)
{
    CreateCanvas(t);
    free(t->cmdQueue);
}

void PostCommand(Turtle *t, cmdFunction cmd, void *params)
{
    if (t->nCmd == t->maxCmd)
    {
        t->maxCmd *= 2;
        Command *cmdQueue = realloc(t->cmdQueue, t->maxCmd * sizeof (Command));
        if (cmdQueue)
            t->cmdQueue = cmdQueue;
    }

    Command command = {cmd, params};
    t->cmdQueue[t->nCmd] = command;
    t->nCmd++;
}

static void ExecuteCommands(Turtle *t)
{
    for (int i = 0; i < t->nCmd; i++)
    {
         Command command = t->cmdQueue[i];
         command.cmd(t, command.params);
    }
}

static void SetCentre(Turtle *t)
{
    RECT rect;
    GetClientRect(t->hwnd, &rect);
    MoveToEx(t->hdc, rect.right / 2, rect.bottom / 2, NULL);
}

void __forward(Turtle *t, void *params)
{
    ForwardParams *forwardParams = (ForwardParams *) params;

    POINT curPos;
    GetCurrentPositionEx(t->hdc, &curPos);

    double alpha = t->angle * 
# 113 "turtle.c" 3
                             3.14159265358979323846 
# 113 "turtle.c"
                                  / 180;

    POINT end;
    end.x = curPos.x + forwardParams->distance * cos(alpha);
    end.y = curPos.y - forwardParams->distance * sin(alpha);

    LineTo(t->hdc, end.x, end.y);
    MoveToEx(t->hdc, end.x, end.y, NULL);
}

void __left(Turtle *t, void *params)
{
    LeftParams *leftParams = (LeftParams *) params;
    t->angle += leftParams->angle;
}

void __goto(Turtle *t, void *params)
{
    t->angle = 0;
    GotoParams *gotoParams = (GotoParams *) params;

    RECT rect;
    GetClientRect(t->hwnd, &rect);
    POINT newPos;
    newPos.x = rect.right / 2 + gotoParams->x;
    newPos.y = rect.bottom / 2 - gotoParams->y;
    MoveToEx(t->hdc, newPos.x, newPos.y, NULL);
}
