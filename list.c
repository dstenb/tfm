#include "list.h"

static void activate();

static void cmd_action(wdata_t *data);
static void cmd_chwin(wdata_t *data);
static void cmd_go_down(wdata_t *data);
static void cmd_go_up(wdata_t *data);
static void cmd_previous_dir(wdata_t *data);
static void cmd_toggle_sort(wdata_t *data);

void
activate()
{
	set_message(M_INFO, "");
}

void
cmd_action(wdata_t *data)
{
	if (F_ISDIR(data->wsel->sel.p)) {
		if (!streq(data->wsel->sel.p->name, "..")) {
			dwindow_read(data->wsel, data->wsel->sel.p->path);
		} else {
			cmd_previous_dir(data);
		}
	} else {

	}
}

void
cmd_chwin(wdata_t *data)
{
	data->wsel = data->win[data->wsel == data->win[0]];
}

void
cmd_go_down(wdata_t *data)
{
	dwindow_set_selected(data->wsel, data->wsel->sel.i + 1);
}

void
cmd_go_up(wdata_t *data)
{
	if (data->wsel->sel.i > 0) {
		dwindow_set_selected(data->wsel, data->wsel->sel.i - 1);
	}
}

void
cmd_previous_dir(wdata_t *data)
{
	char newpath[PATH_MAX];
	char name[PATH_MAX];
	char *p;

	*name = '\0';

	if (!streq(data->wsel->path, "/")) {
		snprintf(newpath, sizeof(newpath), "%s", data->wsel->path);
		prevdir(newpath);

		if ((p = strrchr(data->wsel->path, '/')))
			snprintf(name, sizeof(name), "%s", (p + 1));

		dwindow_read(data->wsel, newpath);
		dwindow_set_selected_by_name(data->wsel, name);
	}
}

void
cmd_toggle_sort(wdata_t *data)
{
	dwindow *dwin = data->wsel;

	dwindow_set_sort(dwin, (dwin->sort + 1) % N_SORTMETHODS);
	set_message(M_INFO, "sorted by: %s", strsort(dwin->sort)); 
	dwindow_set_selected(dwin, 0);
}

state *
list_state()
{
	state *s;
	
	if (!(s = malloc(sizeof(state))))
		oom();
	s->keycmd = list_handle_key;
	s->activate = activate;
	s->normal_bindings = 1;
	return s;
}

void
list_handle_key(wdata_t *data, int c)
{
	if (c == 't') {
		cmd_chwin(data);
	} else if (c == ':') {
		states_push(cmd_state());
/*	} else if (c == '/') {
		states_push(search_state());
 */	} else if (c == 'u') {
		cmd_previous_dir(data);
 	} else if (c == 's') {
		cmd_toggle_sort(data);
	} else if (has_selected_file(data->wsel)) {
		if (c == 13) {
			cmd_action(data);
		} else if (c == 'j' || c == 258) {
			cmd_go_down(data);
		} else if (c == 'k' || c == 259) {
			cmd_go_up(data);
		} else if (c == 'g') {
			dwindow_set_selected(data->wsel, 0);
		} else if (c == 'G') {
			dwindow_set_selected(data->wsel, data->wsel->size);
		}
	}
}
