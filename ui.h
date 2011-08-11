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

#include "utils.h"

void ui_init();

void ui_close();

int ui_getchar();

void ui_printline(WINDOW *win, int y, int attr, const char *fmt, ...);

#endif
