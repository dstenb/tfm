#ifndef _UI_H_
#define _UI_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef HAVE_NCURSESW_NCURSES_H
#include <ncursesw/ncurses.h>
#else
#include <ncurses.h>
#endif

void ui_init(void);

void ui_close(void);

int ui_getchar(void);

void ui_print(WINDOW *win, int y, int x, int l, int attr, const char *fmt, ...);

void ui_printline(WINDOW *win, int y, int attr, const char *fmt, ...);

void ui_vprint(WINDOW *win, int y, int x, int l, int attr, 
		const char *fmt, va_list val);

#endif
