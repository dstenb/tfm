#include "draw.h"

static void draw_dwin(WINDOW *, struct dwindow *, int);
static int get_attr(const struct finfo *, int);
static void print_file(WINDOW *, const struct finfo *, int, int, int);
static void print_filebar(const struct finfo *);

void print_file(WINDOW * win, const struct finfo *fp, int y, int w,
		int selected)
{
	char buf[w / 2];
	char *sstr;
	char timestr[64];
	int attr = get_attr(fp, selected);

	if (F_ISDIR(fp))
		snprintf(buf, w / 2, " %s/", fp->name);
	else
		snprintf(buf, w / 2, " %s", fp->name);

	strftime(timestr, sizeof(timestr), config()->timefmt,
		 localtime(&fp->mtime));
	sstr = strsize(fp->size);

	ui_printline(win, y, attr, "%*s %*s %*s", -(w / 2) + 1, buf,
		     (w / 4) - 1, sstr, (w / 4) - 1, timestr);

	free(sstr);
}

void print_filebar(const struct finfo *fp)
{
	int y, x;
	char *sstr;
	struct passwd *u;
	struct group *g;

	getmaxyx(stdscr, y, x);
	y -= 2;

	if (fp) {
		sstr = strsize(fp->size);
		u = getpwuid(fp->uid);
		g = getgrgid(fp->gid);

		ui_printline(stdscr, y,
			     COLOR_PAIR(C_FILEBAR) | A_BOLD,
			     " %*s %c%s%s%s %s %20s:%-20s",
			     1 - (x / 2),
			     fp->name,
			     F_ISDIR(fp) ? 'd' : '-',
			     strperm(fp->perms.u),
			     strperm(fp->perms.g),
			     strperm(fp->perms.o),
			     sstr, u ? u->pw_name : "", g ? g->gr_name : "");

		free(sstr);
	} else {
		ui_printline(stdscr, y, COLOR_PAIR(C_FILEBAR), "");
	}
}

int get_attr(const struct finfo *fp, int selected)
{
	if (selected)
		return COLOR_PAIR(C_SELECTED) | A_BOLD;
	else if (F_ISDIR(fp))
		return COLOR_PAIR(C_DIRECTORY) | A_BOLD;
	else if (F_ISLNK(fp))
		return COLOR_PAIR(C_SYMLINK);
	return COLOR_PAIR(C_FILE);
}

void draw_dwin(WINDOW * win, struct dwindow *dwin, int selected)
{
	struct finfo *fp;
	int i;
	int y, x;

	werase(win);
	getmaxyx(win, y, x);

	ui_printline(win, 0,
		     COLOR_PAIR(C_TOPBAR) | (selected ? A_BOLD : A_NORMAL),
		     " [%i / %i] %s", dwin->sel.i + 1, dwin->size, dwin->path);

	fp = dwin->start.p;

	for (i = 1; i < y; i++) {
		if (fp) {
			print_file(win, fp, i, x, fp == dwin->sel.p);
			fp = fp->next;
		} else {
			ui_printline(win, i, COLOR_PAIR(C_FILE), "");
		}
	}

	wrefresh(win);

}

void draw(struct wdata *data)
{
	WINDOW *win[2] = { NULL, NULL };
	int y, x;

	getmaxyx(stdscr, y, x);

	if (data->view == V_VERTICAL) {
		win[0] = newwin(y - 2, x / 2, 0, 0);
		win[1] = newwin(y - 2, x / 2, 0, x / 2);

		draw_dwin(win[0], data->win[0], data->win[0] == data->wsel);
		draw_dwin(win[1], data->win[1], data->win[1] == data->wsel);
	} else if (data->view == V_HORIZONTAL) {
		win[0] = newwin((y - 2) / 2, x, 0, 0);
		win[1] = newwin((y - 2) / 2, x, (y - 2) / 2, 0);

		if (y % 2)
			ui_printline(stdscr, (y - 3), COLOR_PAIR(C_FILE), "");
		draw_dwin(win[0], data->win[0], data->win[0] == data->wsel);
		draw_dwin(win[1], data->win[1], data->win[1] == data->wsel);
	} else {
		draw_dwin(stdscr, data->wsel, 1);
	}

	print_filebar(data->wsel->sel.p);
	print_message(y - 1);

	refresh();

	if (data->view == V_VERTICAL || data->view == V_HORIZONTAL) {
		delwin(win[0]);
		delwin(win[1]);
	}
}
