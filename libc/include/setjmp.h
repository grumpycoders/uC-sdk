#ifndef __SETJMP_H__
#define __SETJMP_H__

#include <stdint.h>

typedef struct {
    uint32_t buf[32];
} jmp_buf;

int setjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

#endif
