#ifndef _UTILS_H_
#define _UTILS_H_

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define DIE_UI_FIX 1 /* remove this if you don't want
			the ui to close when using die(),
			e.g. for saner dependencies when
			testing and such */

#ifdef DIE_UI_FIX
#include "ui.h"
#endif

#define ARRSIZE(x) (int)(sizeof(x) / sizeof(*x))

#define streq(s1, s2) (strcmp(s1, s2) == 0)

/* prints message to stderr and exit */
void die(const char *fmt, ...);

/* prints out-of-memory message and exits */
void oom(void);

void prevdir(char *path);

/* spawn a process. returns 0 if successful, or else errno */
int spawn(char **argv, int foreground);

/* string duplicate */
char *strsdup(const char *str);

char *strsize(size_t size);

/* count number of words in a string */
int strwcnt(const char *);

/* return allocated string of first word of the given string */
char *strfw(const char *);

/* replace first word in a string */
void strrfw(char *, size_t, const char *);

#endif
