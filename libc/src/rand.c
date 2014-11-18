#include <stdlib.h>

unsigned int _seed = 2891583007UL;

int rand_r(unsigned int * seed) {
    unsigned int a = *seed;
    a = (a ^ 61) ^ (a >> 16);
    a = a + (a << 3);
    a = a ^ (a >> 4);
    a = a * 668265263UL;
    a = a ^ (a >> 15);
    a *= 3148259783UL;
    *seed = a;
    return (int) a;
}
