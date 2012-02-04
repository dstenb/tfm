#include "commands.h"

void cmd_action(struct wdata *data, const struct arg *arg)
{
	(void)arg;
	if (data->wsel->sel.p) {
		if (F_ISDIR(data->wsel->sel.p))
			dwindow_read(data->wsel, data->wsel->sel.p->path);
		else
			program_spawn(data->wsel->sel.p->path);
	}
}

void cmd_go_down(struct wdata *data, const struct arg *arg)
{
	(void)arg;
	dwindow_set_selected(data->wsel, data->wsel->sel.i + 1);
	set_message(M_INFO, "");
}

void cmd_go_end(struct wdata *data, const struct arg *arg)
{
	(void)arg;
	dwindow_set_selected(data->wsel, data->wsel->size);
	set_message(M_INFO, "");
}

void cmd_go_home(struct wdata *data, const struct arg *arg)
{
	(void)arg;
	dwindow_set_selected(data->wsel, 0);
	set_message(M_INFO, "");
}

void cmd_go_up(struct wdata *data, const struct arg *arg)
{
	(void)arg;
	if (data->wsel->sel.i > 0) {
		dwindow_set_selected(data->wsel, data->wsel->sel.i - 1);
	}
	set_message(M_INFO, "");
}

void cmd_mkdir(struct wdata *data, const struct arg *arg)
{
	if (data->wsel->path && arg->s) {
		if (chdir(data->wsel->path) || mkdir(arg->s, 0755)) {
			set_message(M_ERROR, "couldn't create \"%s\" (%s)",
					arg->s, strerror(errno));
			return;
		}
	}
}

void cmd_quit(struct wdata *data, const struct arg *arg)
{
	(void)data;
	(void)arg;
	exit(EXIT_SUCCESS);
}

void cmd_set_dotfiles(struct wdata *data, const struct arg *arg)
{
	if (arg->i < 0 || arg->i > 1)
		return;

	dwindow_show_dotfiles(data->wsel, arg->i);

	set_message(M_INFO, "showing dotfiles: %s",
		    arg->i ? "enabled" : "disabled");
}

void cmd_set_path(struct wdata *data, const struct arg *arg)
{
	if (data->wsel->path)
		chdir(data->wsel->path);
	dwindow_read(data->wsel, arg->s);
}

void cmd_set_selected(struct wdata *data, const struct arg *arg)
{
	dwindow_set_selected(data->wsel, arg->i);
	set_message(M_INFO, "");
}

void cmd_set_selected_r(struct wdata *data, const struct arg *arg)
{
	dwindow_set_selected(data->wsel, data->wsel->sel.i + arg->i);
	set_message(M_INFO, "");
}

void cmd_set_sort(struct wdata *data, const struct arg *arg)
{
	if (arg->i < 0 || arg->i >= N_SORTMETHODS)
		return;
	set_message(M_INFO, "");

	dwindow_set_sort(data->wsel, arg->i);

	set_message(M_INFO, "sorted by: %s", strsort(data->wsel->sort));
}

void cmd_set_view(struct wdata *data, const struct arg *arg)
{
	if (arg->i < 0 || arg->i >= N_VIEWS)
		return;

	wdata_set_view(data, arg->i);
}

void cmd_set_win(struct wdata *data, const struct arg *arg)
{
	if (arg->i < 0 || arg->i >= 2)
		return;

	data->wsel = data->win[arg->i];
}

void cmd_shell(struct wdata *data, const struct arg *arg)
{
	char *shell = getenv("SHELL");
	(void)arg;

	if (!shell)
		shell = "/bin/sh";

	if (data->wsel->path) {
		ui_close();
		chdir(data->wsel->path);
		system(shell);
		ui_init();
	}
}

void cmd_toggle_dotfiles(struct wdata *data, const struct arg *arg)
{
	struct arg parg;
	(void)arg;

	parg.i = !data->wsel->show_dot;
	cmd_set_dotfiles(data, &parg);
}

void cmd_toggle_sort(struct wdata *data, const struct arg *arg)
{
	struct arg parg;
	(void)arg;

	parg.i = (data->wsel->sort + 1) % N_SORTMETHODS;
	cmd_set_sort(data, &parg);
}

void cmd_toggle_view(struct wdata *data, const struct arg *arg)
{
	struct arg parg;
	(void)arg;

	parg.i = (data->view + 1) % N_VIEWS;
	cmd_set_view(data, &parg);
}

void cmd_toggle_win(struct wdata *data, const struct arg *arg)
{
	struct arg parg;
	(void)arg;

	parg.i = data->wsel == data->win[0];
	cmd_set_win(data, &parg);
}

void cmd_mark_deselect_all(struct wdata *data, const struct arg *arg)
{
	struct finfo *fp;
	(void)arg;

	for (fp = data->wsel->files; fp; fp = fp->next)
		if (!streq(fp->name, ".."))
			fp->marked = 0;
}

void cmd_mark_select_all(struct wdata *data, const struct arg *arg)
{
	struct finfo *fp;
	(void)arg;

	for (fp = data->wsel->files; fp; fp = fp->next)
		if (!streq(fp->name, ".."))
			fp->marked = 1;
}

void cmd_mark_toggle(struct wdata *data, const struct arg *arg)
{
	(void)arg;
	if (data->wsel->sel.p && !streq(data->wsel->sel.p->name, ".."))
		data->wsel->sel.p->marked ^= 1;
}

void cmd_mark_invert(struct wdata *data, const struct arg *arg)
{
	struct finfo *fp;
	(void)arg;

	for (fp = data->wsel->files; fp; fp = fp->next)
		if (!streq(fp->name, ".."))
			fp->marked ^= 1;
}
