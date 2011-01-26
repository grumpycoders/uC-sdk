#include <stdarg.h>
#include <string.h>
#include <stdint.h>
#include <BoardConsole.h>
#include <osdebug.h>

static const char hex_conv[] = "0123456789ABCDEF";
static void dbgput(const void * _str, int n) {
    const char * str = (const char *) _str;
    while(n--)
        BoardConsolePutc(*(str++));
}

void osDbgPrintf(const char * str, ...) {
    if (!str)
        return;

    va_list ap;
    const char * percent;
    const char * ptr = str;
    int entry_size;
    char tmp_n_conv[33], * tmp_conv_ptr;

    int seen_something;

    int arg_c;
    char * arg_s;
    unsigned long arg_u = 0;
    long arg_i;
    uintptr_t arg_p;
    int str_size, i;

    va_start(ap, str);

    while ((percent = strchr(ptr, '%'))) {
        dbgput(ptr, percent - ptr);
        ptr = percent + 1;
        entry_size = 0;
        while ((*ptr >= '0') && (*ptr <= '9')) {
            entry_size *= 10;
            entry_size += *ptr - '0';
            ptr++;
        }
        switch (*ptr) {
        case '%':
            dbgput("%", 1);
            break;
        case 'c':
            arg_c = va_arg(ap, int);
            dbgput(&arg_c, 1);
            break;
        case 's':
            arg_s = va_arg(ap, char *);
            if (arg_s) {
                for (str_size = strlen(arg_s); str_size < entry_size; str_size++)
                    dbgput(" ", 1);
                dbgput(arg_s, strlen(arg_s));
            } else {
                for (str_size = 0; str_size < entry_size; str_size++)
                    dbgput(" ", 1);
            }
            break;
        case 'i':
        case 'd':
            arg_i = va_arg(ap, long);
            if (arg_i < 0) {
                dbgput("-", 1);
                arg_u = -arg_i;
            } else {
                arg_u = arg_i;
            }
        case 'u':
            if (*ptr == 'u')
                arg_u = va_arg(ap, unsigned long);
            tmp_conv_ptr = tmp_n_conv + 32;
            *tmp_conv_ptr = 0;
            do {
                *--tmp_conv_ptr = hex_conv[arg_u % 10];
                arg_u /= 10;
            } while (arg_u);
            dbgput(tmp_conv_ptr, strlen(tmp_conv_ptr));
            break;
        case 'p':
            arg_p = va_arg(ap, uintptr_t);
            if (arg_p) {
                dbgput("0x", 2);
                for (i = sizeof(arg_p) * 2 - 1; i >= 0; i--) {
                    dbgput(&hex_conv[(arg_p >> (i << 2)) & 15], 1);
                }
            } else {
                dbgput("(nil)", 4);
            }
            break;
        case 'x':
            arg_u = va_arg(ap, unsigned long);
            seen_something = 0;
            for (i = sizeof(arg_p) * 2 - 1; i >= 0; i--) {
                if (!seen_something && ((arg_u >> (i << 2)) == 0))
                    continue;
                dbgput(&hex_conv[(arg_u >> (i << 2)) & 15], 1);
                seen_something = 1;
            }
            if (!seen_something)
                dbgput("0", 1);
            break;
        case 0:  // malformed format string with a trailing %.
            dbgput("%", 1);
            return;
        default:
            dbgput("<unsupported format: %", 22);
            dbgput(ptr, 1);
            dbgput(">", 1);
            break;
        }
        ptr++;
    }

    if (*ptr)
      dbgput(ptr, strlen(ptr));
}
