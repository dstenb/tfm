#include "wdata.h"

void wdata_handle_resize(struct wdata *data)
{
	int y, x;

	getmaxyx(stdscr, y, x);
	(void)x;

	if (data->view == V_VERTICAL) {
		dwindow_set_winsize(data->win[0], (y - 3));
		dwindow_set_winsize(data->win[1], (y - 3));
	} else if (data->view == V_HORIZONTAL) {
		dwindow_set_winsize(data->win[0], (y - 4) / 2);
		dwindow_set_winsize(data->win[1], (y - 4) / 2);
	} else {
		dwindow_set_winsize(data->win[0], (y - 3));
		dwindow_set_winsize(data->win[1], (y - 3));
	}
}

int wdata_sel_win_index(struct wdata *data)
{
	if (data->wsel == data->win[0])
		return 0;
	else if (data->wsel == data->win[1])
		return 1;
	return -1;
}

void wdata_set_view(struct wdata *data, view_t view)
{
	if (view < N_VIEWS) {
		data->view = view;
		wdata_handle_resize(data);
	}
}

void wdata_lock_mutex(struct wdata *data)
{
	pthread_mutex_lock(&data->mutex);
}

void wdata_unlock_mutex(struct wdata *data)
{
	pthread_mutex_unlock(&data->mutex);
}
