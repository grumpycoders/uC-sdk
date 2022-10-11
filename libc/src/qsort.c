#include <stdio.h>
#include <stdlib.h>

void qsort(void *base, size_t num, size_t size, int (*cmp)(const void *, const void *)) {
    size_t gap, i, j, k;
    if ((num > 1) && (size > 0)) {
        gap = 0;
        while (gap < (num - 1) / 3) gap = 3 * gap + 1;
        gap *= size;
        num *= size;
        while (gap != 0) {
            i = gap;
            while (i < num) {
                j = i;
                while (j >= gap) {
                    char *a, *b;
                    j -= gap;
                    a = j + ((char *)base);
                    b = a + gap;
                    if ((*cmp)(a, b) <= 0) break;
                    k = size;
                    while (k--) {
                        char tmp = *a;
                        *a++ = *b;
                        *b++ = tmp;
                    }
                }
                i += size;
            }
            gap = (gap - size) / 3;
        }
    }
}
