#include <errno.h>
#include <locale.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "config.h"
#include "draw.h"
#include "dwindow.h"
#include "list.h"
#include "message.h"
#include "program.h"
#include "state.h"
#include "theme.h"
#include "ui.h"
#include "utils.h"

static void atexit_handler(void);
static void handle_resize(void);
static void main_loop(void);
static void read_config_files(void);
static void update(struct dwindow *);
static void *update_loop(void *);
static void usage(void);

static char *cmdname = NULL;
static struct wdata data;

void atexit_handler()
{
	config_close();
	program_close();
	ui_close();
	states_clear();
	dwindow_free(data.win[0]);
	dwindow_free(data.win[1]);
}

/* get terminal size and fix window sizes */
void handle_resize()
{
	wdata_lock_mutex(&data);
	wdata_handle_resize(&data);
	wdata_unlock_mutex(&data);
}

void main_loop()
{
	MEVENT event;
	int i;

	wdata_lock_mutex(&data);
	wdata_set_view(&data, config()->view);
	wdata_unlock_mutex(&data);

	for (;;) {
		wdata_lock_mutex(&data);
		draw(&data);
		wdata_unlock_mutex(&data);

		i = ui_getchar();

		if (i == KEY_RESIZE) {
			handle_resize();
		} else if (i == KEY_MOUSE) {
			if (getmouse(&event) == OK) {
				wdata_lock_mutex(&data);
				states_handlemouse(&data, &event);
				wdata_unlock_mutex(&data);

			}
		} else {
			wdata_lock_mutex(&data);
			states_handlekey(&data, i);
			wdata_unlock_mutex(&data);
		}
	}
}

/* tries to read all configuration files */
void read_config_files()
{
	char *confdir;
	char path[PATH_MAX + 1];

	if ((confdir = get_conf_dir())) {

		/* (try to) read main configuration file */
		snprintf(path, sizeof(path), "%s/config", confdir);
		config_read(path);

		/* (try to) read theme file */
		if (config()->theme) {
			if (*config()->theme == '/') {
				snprintf(path, sizeof(path), "%s",
					 config()->theme);
			} else {
				snprintf(path, sizeof(path), "%s/%s",
					 confdir, config()->theme);
			}

			printf("theme_read: %s\n", path);
			theme_read_from_file(path);
		}

		/* (try to) read program file */
		if (config()->programs) {
			if (*config()->programs == '/') {
				snprintf(path, sizeof(path), "%s",
					 config()->programs);
			} else {
				snprintf(path, sizeof(path), "%s/%s",
					 confdir, config()->programs);
			}

			printf("program_read: %s\n", path);
			program_read_from_file(path);

		}

		free(confdir);
	}
}

/* reloads the dwindow if its been modified since last read */
void update(struct dwindow *dwin)
{
	time_t otime;
	int err;

	if (dwin->dirinfo) {
		otime = dwin->dirinfo->mtime;
		err = finfo_stat(dwin->dirinfo);

		if (err || dwin->dirinfo->mtime > otime) {
			dwindow_reload(dwin);
		}
	}
}

void *update_loop(void *v)
{
	struct wdata *d = v;

	for (;;) {
		sleep(1);

		wdata_lock_mutex(d);
		update(d->win[0]);
		update(d->win[1]);
		draw(d);
		wdata_unlock_mutex(d);
	}

	return NULL;
}

void usage()
{
	die("Usage: %s [-h]\n", cmdname);
}

int main(int argc, char **argv)
{
	char *paths[] = { NULL, NULL };
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

	config_init();
	program_init();
	read_config_files();

	/* setup windows */
	for (i = 0; i < 2; i++) {
		data.win[i] = dwindow_create();
		dwindow_set_sort(data.win[i], config()->sort);
		dwindow_read(data.win[i], paths[i]);
	}

	data.wsel = data.win[0];

	/* setup ui */
	ui_init();
	theme_init();

	/* set default state */
	states_push(list_state());

	/* start update thread */
	pthread_mutex_init(&data.mutex, NULL);
	if (pthread_create(&u_tid, NULL, update_loop, &data) != 0)
		die("pthread_create: %s\n", strerror(errno));

	main_loop();
	ui_close();

	return 0;
}
