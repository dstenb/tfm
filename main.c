#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <locale.h>
#include <pthread.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>

#include "dwindow.h"
#include "list.h"
#include "state.h"
#include "theme.h"
#include "ui.h"
#include "utils.h"

static void atexit_handler();
static void draw(const char *topbar);
static int get_attr(const finfo *fp);
static char *get_conf_dir();
static void handle_resize();
static void main_loop();
static void print_file(const finfo *fp, int y, int selected);
static void print_filebar(const finfo *fp, int y);
static void *update_loop(void *v);
static void usage();

static char *cmdname = NULL;
static wdata_t data;

static int g_i = 0; /* TODO */

void
atexit_handler()
{
	ui_close();
	states_clear();
	dwindow_free(data.win[0]);
	dwindow_free(data.win[1]);
}

void draw(const char *topbar)
{
	int i;
	int y, x;
	int stop;
	finfo *tmp;

	getmaxyx(stdscr, y, x);

	erase();
	ui_printline(stdscr, 0, 
			A_BOLD | COLOR_PAIR(C_TOPBAR), 
			"%s (%i / %i): %s (%i)", data.wsel ? data.wsel->path : "", 
			data.wsel->sel.i + 1, data.wsel->size,
			topbar, g_i);

	if (data.wsel && data.wsel->sel.p) {

		tmp = data.wsel->start.p;
		i = 1;
		stop = y - 2;

		while (tmp && i < stop) {
			print_file(tmp, i, tmp == data.wsel->sel.p);
			i++;
			tmp = tmp->next;
		}

		print_filebar(data.wsel->sel.p, y - 2);
		states_setbuf(&data);
		ui_printline(stdscr, y - 1, COLOR_PAIR(C_BOTTOMBAR),
				"%s", data.buf);
	}

	refresh();
}

int
get_attr(const finfo *fp)
{
	if (fp->flags & DIRECTORY)
		return COLOR_PAIR(C_DIRECTORY);
	else if (fp->flags & SYMLINK)
		return COLOR_PAIR(C_SYMLINK);
	else
		return COLOR_PAIR(C_FILE);
}

char *
get_conf_dir()
{
	char buf[PATH_MAX];
	char *home;
	char *path = NULL;

	if ((home = getenv("HOME"))) {
		snprintf(buf, sizeof(buf), "%s/.tfm", home);
		if (!(path = strdup(buf)))
			oom();
	}

	return path;
}

void handle_resize()
{
	int i;
	int y, x;

	getmaxyx(stdscr, y, x);

	for (i = 0; i < 2; i++) {
		data.win[i]->winsize = y - 3;
	}
}

void
main_loop()
{
	int i;

	handle_resize();

	for (;;) {
		draw("Main loop");
		i = ui_getchar();

		if (i == KEY_RESIZE) {
			handle_resize();
		}
		g_i = i; /* TODO */
		states_handlekey(&data, i);
	}
}

void
print_file(const finfo *fp, int y, int selected)
{
	int attr = selected ? (int) (COLOR_PAIR(C_SELECTED) | A_BOLD) : get_attr(fp);

	if (F_ISDIR(fp))
		ui_printline(stdscr, y, attr, " %s/", fp->name); 
	else
		ui_printline(stdscr, y, attr, " %s", fp->name);
}

void
print_filebar(const finfo *fp, int y)
{
	char *sstr = strsize(fp->size);
	struct passwd *u = getpwuid(fp->uid);
	struct group *g = getgrgid(fp->gid);

	ui_printline(stdscr, y, 
			COLOR_PAIR(C_FILEBAR) | A_BOLD, 
			" %c%s%s%s %20s %s %20s %20s",
			F_ISDIR(fp) ? 'd' : '-',
			strperm(fp->perms.u),
			strperm(fp->perms.g),
			strperm(fp->perms.o),
			fp->name, sstr, 
			u ? u->pw_name : "", 
			g ? g->gr_name : "");

	free(sstr);
}

void *
update_loop(void *v)
{
	wdata_t *data = v;

	for (;;) {
		sleep(10);
	/*	draw("Update loop");
	*/}
	
	return NULL;
}

void
usage()
{
	die("Usage: %s [-h]\n", cmdname);
}

int
main(int argc, char **argv)
{
	char *paths[] = { NULL, NULL };
	char *confdir;
	int i;
	pthread_t u_tid;

	cmdname = argv[0];
	for (i = 1; i < argc && *argv[i] == '-'; i++) {
		if (streq(argv[i], "-h")) {
			usage();
		}
	}

	paths[0] = (argc > i) ? argv[i] : ".";
	paths[1] = (argc > (i + 1)) ? argv[i + 1] : paths[0];

	atexit(atexit_handler);

	if (!setlocale(LC_CTYPE, ""))
		die("couldn't set locale\n");

	/* setup windows */
	for (i = 0; i < 2; i++) {
		data.win[i] = dwindow_create();
		data.win[i]->cmp = finfocmp_name;
		dwindow_read(data.win[i], paths[i]);
	}

	data.wsel = data.win[0];

	data.buf = calloc(100, sizeof(char));
	data.bufsize = 100;
	*data.buf = '\0';

	if (pthread_create(&u_tid, NULL, update_loop, NULL) != 0)
		die("pthread_create: %s\n", strerror(errno));

	/* setup ui */
	ui_init();
	theme_init_default();

	if ((confdir = get_conf_dir())) {
		chdir(confdir);
		theme_read_from_file("theme");
		free(confdir);
	}

	states_push(list_state());
	main_loop();
	ui_close();

	return 0;
}
