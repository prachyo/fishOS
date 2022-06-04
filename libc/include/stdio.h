#ifndef _STDIO_H
#define _STDIO_H 1

#include <sys/cdefs.h>

#define EOF (-1)

#ifdef __cplusplus
extern "C" {
#endif

int pprintf(const char* __restrict, ...);
int putchar(int);

#ifdef __cplusplus
}
#endif

#endif

