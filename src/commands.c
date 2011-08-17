#include "commands.h"

int
cmd_action(wdata_t *data, const arg_t *arg)
{
	if (data->wsel->sel.p && F_ISDIR(data->wsel->sel.p)) {
		dwindow_read(data->wsel, data->wsel->sel.p->path);
	}

	return 1;
}

int
cmd_go_down(wdata_t *data, const arg_t *arg)
{
	dwindow_set_selected(data->wsel, data->wsel->sel.i + 1);
	return 1;
}

int
cmd_go_end(wdata_t *data, const arg_t *arg)
{
	dwindow_set_selected(data->wsel, data->wsel->size);
	return 1;
}

int
cmd_go_home(wdata_t *data, const arg_t *arg)
{
	dwindow_set_selected(data->wsel, 0);
	return 1;
}

int 
cmd_go_up(wdata_t *data, const arg_t *arg)
{
	if (data->wsel->sel.i > 0) {
		dwindow_set_selected(data->wsel, data->wsel->sel.i - 1);
	}

	return 1;
}

int
cmd_set_dotfiles(wdata_t *data, const arg_t *arg)
{
	if (arg->i < 0 || arg->i > 1)
		return 0;

	dwindow_show_dotfiles(data->wsel, arg->i);

	set_message(M_INFO, "showing dotfiles: %s",
			arg->i ? "enabled" : "disabled");
	return 1;
}

int
cmd_set_path(wdata_t *data, const arg_t *arg)
{
	if (data->wsel->path)
		chdir(data->wsel->path);
	dwindow_read(data->wsel, arg->s);
	return 1;
}

int
cmd_set_selected(wdata_t *data, const arg_t *arg)
{
	dwindow_set_selected(data->wsel, arg->i);
	return 1;
}

int
cmd_set_selected_r(wdata_t *data, const arg_t *arg)
{
	dwindow_set_selected(data->wsel,
			data->wsel->sel.i + arg->i);
	return 1;
}

int
cmd_set_sort(wdata_t *data, const arg_t *arg)
{
	if (arg->i < 0 || arg->i >= N_SORTMETHODS)
		return 0;

	dwindow_set_sort(data->wsel, arg->i);

	set_message(M_INFO, "sorted by: %s", 
			strsort(data->wsel->sort));

	return 1;
}

int
cmd_set_view(wdata_t *data, const arg_t *arg)
{
	if (arg->i < 0 || arg->i >= N_VIEWS)
		return 0;

	wdata_set_view(data, arg->i);

	return 1;
}

int
cmd_set_win(wdata_t *data, const arg_t *arg)
{
	if (arg->i < 0 || arg->i >= 2)
		return 0;

	data->wsel = data->win[arg->i];

	return 1;
}

int
cmd_toggle_dotfiles(wdata_t *data, const arg_t *arg)
{
	arg_t parg;

	parg.i = !data->wsel->show_dot;
	cmd_set_dotfiles(data, &parg);

	return 1;
}

int
cmd_toggle_sort(wdata_t *data, const arg_t *arg)
{
	arg_t parg;

	parg.i = (data->wsel->sort + 1) % N_SORTMETHODS;
	cmd_set_sort(data, &parg);

	return 1;
}

int
cmd_toggle_view(wdata_t *data, const arg_t *arg)
{
	arg_t parg;

	parg.i = (data->view + 1) % N_VIEWS;
	cmd_set_view(data, &parg);

	return 1;
}

int
cmd_toggle_win(wdata_t *data, const arg_t *arg)
{
	arg_t parg;
	
	parg.i = data->wsel == data->win[0];
	cmd_set_win(data, &parg);

	return 1;
}
