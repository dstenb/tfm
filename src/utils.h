#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "ui.h"

#define ARRSIZE(x) (int)(sizeof(x) / sizeof(*x))

#define streq(s1, s2) (strcmp(s1, s2) == 0)

/* prints message to stderr and exit */
void die(const char *fmt, ...);

/* prints out-of-memory message and exits */
void oom(void);

void prevdir(char *path);

/* string duplicate */
char *strsdup(const char *str);

char *strsize(size_t size);

#endif
