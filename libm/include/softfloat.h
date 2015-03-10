#pragma once

static __inline__ float acosf(float x) { return acos(x); }
static __inline__ float asinf(float x) { return asin(x); }
static __inline__ float atanf(float x) { return atan(x); }
static __inline__ float atan2f(float x, float y) { return atan2f(x, y); }
static __inline__ float cosf(float x) { return cos(x); }
static __inline__ float sinf(float x) { return sin(x); }
static __inline__ float tanf(float x) { return tan(x); }

static __inline__ float coshf(float x) { return cosh(x); }
static __inline__ float sinhf(float x) { return sinh(x); }
static __inline__ float tanhf(float x) { return tanh(x); }

static __inline__ float expf(float x) { return exp(x); }
static __inline__ float frexpf(float x, int * p) { return frexp(x, p); }
static __inline__ float ldexpf(float x, int i) { return ldexp(x, i); }
static __inline__ float logf(float x) { return log(x); }
static __inline__ float log10f(float x) { return log10(x); }
static __inline__ float modff(float x, float * p) { double v = *p; float r = modf(x, &v); *p = v; return r; }

static __inline__ float powf(float x, float y) { return pow(x, y); }
#ifndef SOFTFLOAT_SQRTF
static __inline__ float sqrtf(float x) { return sqrt(x); }
#endif

static __inline__ float ceilf(float x) { return ceil(x); }
static __inline__ float fabsf(float x) { return fabs(x); }
static __inline__ float floorf(float x) { return floor(x); }
static __inline__ float fmodf(float x, float y) { return fmod(x, y); }

static __inline__ float erff(float x) { return erf(x); }
static __inline__ float erfcf(float x) { return erfc(x); }
static __inline__ float gammaf(float x) { return gamma(x); }
static __inline__ float hypotf(float x, float y) { return hypot(x, y); }
static __inline__ int isnanf(float x) { return isnan(x); }
static __inline__ int finitef(float x) { return finite(x); }
static __inline__ float lgammaf(float x) { return lgamma(x); }

static __inline__ float acoshf(float x) { return acosh(x); }
static __inline__ float asinhf(float x) { return asinh(x); }
static __inline__ float atanhf(float x) { return atanh(x); }
static __inline__ float cbrtf(float x) { return cbrt(x); }
static __inline__ float logbf(float x) { return logb(x); }
static __inline__ float nextafterf(float x, float y) { return nextafter(x, y); }
static __inline__ float remainderf(float x, float y) { return remainder(x, y); }
#ifdef _SCALB_INT
static __inline__ float scalbf(float x, int i) { return scalb(x, i); }
#else
static __inline__ float scalbf(float x, float y) { return scalb(x, y); }
#endif
