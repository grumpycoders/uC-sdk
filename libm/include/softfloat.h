#pragma once

static inline float acosf(float x) { return acos(x); }
static inline float asinf(float x) { return asin(x); }
static inline float atanf(float x) { return atan(x); }
static inline float atan2f(float x, float y) { return atan2f(x, y); }
static inline float cosf(float x) { return cos(x); }
static inline float sinf(float x) { return sin(x); }
static inline float tanf(float x) { return tan(x); }

static inline float coshf(float x) { return cosh(x); }
static inline float sinhf(float x) { return sinh(x); }
static inline float tanhf(float x) { return tanh(x); }

static inline float expf(float x) { return exp(x); }
static inline float frexpf(float x, int * p) { return frexp(x, p); }
static inline float ldexpf(float x, int i) { return ldexp(x, i); }
static inline float logf(float x) { return log(x); }
static inline float log10f(float x) { return log10(x); }
static inline float modff(float x, float * p) { double v = *p; float r = modf(x, &v); *p = v; return r; }

static inline float powf(float x, float y) { return pow(x, y); }
#ifndef SOFTFLOAT_SQRTF
static inline float sqrtf(float x) { return sqrt(x); }
#endif

static inline float ceilf(float x) { return ceil(x); }
static inline float fabsf(float x) { return fabs(x); }
static inline float floorf(float x) { return floor(x); }
static inline float fmodf(float x, float y) { return fmod(x, y); }

static inline float erff(float x) { return erf(x); }
static inline float erfcf(float x) { return erfc(x); }
static inline float gammaf(float x) { return gamma(x); }
static inline float hypotf(float x, float y) { return hypot(x, y); }
static inline int isnanf(float x) { return isnan(x); }
static inline int finitef(float x) { return finite(x); }
static inline float lgammaf(float x) { return lgamma(x); }

static inline float acoshf(float x) { return acosh(x); }
static inline float asinhf(float x) { return asinh(x); }
static inline float atanhf(float x) { return atanh(x); }
static inline float cbrtf(float x) { return cbrt(x); }
static inline float logbf(float x) { return logb(x); }
static inline float nextafterf(float x, float y) { return nextafter(x, y); }
static inline float remainderf(float x, float y) { return remainder(x, y); }
#ifdef _SCALB_INT
static inline float scalbf(float x, int i) { return scalb(x, i); }
#else
static inline float scalbf(float x, float y) { return scalb(x, y); }
#endif
