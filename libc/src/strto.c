#include <limits.h>
#include <stdlib.h>
#include <ctype.h>

long int strtol(const char * nptr, char ** endptr, int base) {
    if (base < 0 || base == 1 || base >= 37) {
        set_errno(EINVAL);
        return 0;
    }

    int guess_base = 0;
    int got_prefix = 0;
    int got_something = 0;
    int negative = 0;
    unsigned long int r = 0;
    unsigned long int o = 0;

    if (base == 0) {
        base = 10;
        guess_base = 1;
    }

    char c;

    while ((c = *nptr++)) {
        if (isspace(c) && !got_something) continue;
        if (((c == '+') || (c == '-')) && !got_something) {
            got_something = 1;
            if (c == '-') negative = 1;
            continue;
        }
        if ((c == '0') && (r == 0) && !got_prefix && guess_base) {
            got_prefix = 1;
            got_something = 1;
            base = 8;
            continue;
        }
        if ((c == 'x') && (r == 0) && got_prefix && guess_base) {
            got_something = 1;
            base = 16;
            guess_base = 0;
            continue;
        }
        if ((c >= '0') && (c <= '9')) {
            c -= '0';
        } else if ((c >= 'a') && (c <= 'z')) {
            c -= 'a' + 10;
        } else if ((c >= 'A') && (c <= 'Z')) {
            c -= 'A' + 10;
        } else {
            break;
        }
        if (c >= base) break;
        got_something = 1;
        r *= base;
        r += c;
        if ((o > r) || (r >= LONG_MAX)) {
            if (negative) {
                r = LONG_MIN;
                negative = 0;
            } else {
                r = LONG_MAX;
            }
            set_errno(ERANGE);
            break;
        }
        o = r;
    }

    if (endptr) *endptr = (char *) nptr;
    return negative ? -r : r;
}
