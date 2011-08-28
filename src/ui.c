#include "ui.h"

void
ui_init()
{
	initscr();
	keypad(stdscr, 1);
	nonl();
	cbreak();
	noecho();
	curs_set(0);

	start_color();
	use_default_colors();

	mousemask(ALL_MOUSE_EVENTS, NULL);

	refresh();
}

void
ui_close()
{
	endwin();
}

int
ui_getchar()
{
	return getch();
}

void
ui_vprint(WINDOW *win, int y, int x, int l, int attr, 
		const char *fmt, va_list val)
{
	char buf[l + 1];
	int i;

	for (i = vsnprintf(buf, l, fmt, val); i < l; i++) 
		buf[i] = ' ';
	buf[l] = '\0';

	wattron(win, attr);
	mvwprintw(win, y, x, buf);
	wattroff(win, attr);
}

void
ui_print(WINDOW *win, int y, int x, int l, int attr, const char *fmt, ...)
{
	va_list val;
	
	va_start(val, fmt);
	ui_vprint(win, y, x, l, attr, fmt, val);
	va_end(val);
}

void
ui_printline(WINDOW *win, int y, int attr, const char *fmt, ...)
{
	va_list val;
	int win_x;
	int win_y;

	getmaxyx(win, win_y, win_x);
	va_start(val, fmt);
	ui_vprint(win, y, 0, win_x, attr, fmt, val);
	va_end(val);
}
