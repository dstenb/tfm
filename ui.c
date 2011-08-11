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

void ui_printline(WINDOW *win, int y, int attr, const char *fmt, ...)
{
	va_list val;
	char *buf;
	int i;
	int win_x;
	int win_y;

	getmaxyx(win, win_y, win_x);

	if (y >= win_y)
		return;

	va_start(val, fmt);

	if (!(buf = calloc(win_x + 1, sizeof(char))))
		oom();

	for (i = vsnprintf(buf, win_x, fmt, val); i < win_x; i++)
		buf[i] = ' ';
	
	wattron(win, attr);
	mvwprintw(win, y, 0, buf);
	wattroff(win, attr);

	free(buf);
	va_end(val);
}
