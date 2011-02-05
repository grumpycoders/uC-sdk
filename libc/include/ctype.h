#ifndef __CTYPE_H__
#define __CTYPE_H__

static inline int isascii(int c) { return (c & 0x80) == 0; }
static inline int isblank(int c) { return c == ' ' || c == '\t'; }
static inline int isdigit(int c) { return c >= '0' && c <= '9'; }
static inline int iscntrl(int c) { return c < 32; }
static inline int islower(int c) { return c >= 'a' && c <= 'z'; }
static inline int isspace(int c) { return c == ' ' || c == '\f' || c == '\n' || c == '\r' || c == '\t' || c == '\v'; }
static inline int isupper(int c) { return c >= 'A' && c <= 'Z'; }
static inline int isxdigit(int c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

static inline int isalpha(int c) { return isupper(c) || islower(c); }
static inline int isalnum(int c) { return isalpha(c) || isdigit(c); }
static inline int isgraph(int c) { return !iscntrl(c) && !isspace(c); }
static inline int isprint(int c) { return !iscntrl(c); }
static inline int ispunct(int c) { return !iscntrl(c) && !isspace(c) && !isalnum(c); }


#endif
