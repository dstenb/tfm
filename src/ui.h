#ifndef _UI_H_
#define _UI_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef HAVE_NCURSESW_NCURSES_H
#include <ncursesw/ncurses.h>
#else
#include <ncurses.h>
#endif

void ui_close(void);

int ui_getchar(void);

void ui_init(void);

void ui_print(WINDOW *, int, int, int, int, const char *, ...);

void ui_printline(WINDOW *, int, int, const char *, ...);

void ui_vprint(WINDOW *, int, int, int, int, const char *, va_list);

#endif
