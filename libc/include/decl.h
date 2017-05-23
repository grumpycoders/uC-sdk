#pragma once

#ifdef __cplusplus
#define BEGIN_DECL extern "C" {
#define END_DECL }
#define ucsdk_static_assert static_assert
#else
#define BEGIN_DECL
#define END_DECL
#define ucsdk_static_assert _Static_assert
#endif
