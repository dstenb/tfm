#include "list_state.h"

static int cmd_chcmd(struct wdata *, const struct arg *);
static void handle_mouse(struct wdata *, const MEVENT *);
static void handle_key(struct wdata *, int);

struct {
	int key;
	int (*func) (struct wdata *, const struct arg *);
	struct arg arg;
} list_cmds[] = {
	{ 13,  cmd_action,            { 0, 0 } },
	{ 258, cmd_go_down,           { 0, 0 } },
	{ 'j', cmd_go_down,           { 0, 0 } },
	{ 'G', cmd_go_end,            { 0, 0 } },
	{ 'g', cmd_go_home,           { 0, 0 } },
	{ 259, cmd_go_up,             { 0, 0 } },
	{ 'k', cmd_go_up,             { 0, 0 } },
	{ 'u', cmd_set_path,          { 0, ".."} },
	{ 'H', cmd_toggle_dotfiles,   { 0, 0 } },
	{ 'S', cmd_toggle_sort,       { 0, 0 } },
	{ 'V', cmd_toggle_view,       { 0, 0 } },
	{ ' ', cmd_toggle_win,        { 0, 0 } },
	{ 't', cmd_toggle_win,        { 0, 0 } },
	{ ':', cmd_chcmd,             { 0, 0 } },
	{ 'd', cmd_mark_deselect_all, { 0, 0 } },
	{ 's', cmd_mark_select_all,   { 0, 0 } },
	{ 'v', cmd_mark_toggle,       { 0, 0 } },
	{ 'i', cmd_mark_invert,       { 0, 0 } }
};

struct state *list_state()
{
	return state_create(handle_key, handle_mouse, NULL, 1);
}

void handle_key(struct wdata *data, int c)
{
	int i;

	for (i = 0; i < ARRSIZE(list_cmds); i++) {
		if (c == list_cmds[i].key) {
			list_cmds[i].func(data, &list_cmds[i].arg);
			break;
		}
	}
}

void handle_mouse(struct wdata *data, const MEVENT * event)
{
	struct arg arg;
	int y, x;
	getmaxyx(stdscr, y, x);

	if (!data->wsel->path)
		return;

	if (data->view == V_SINGLE) {
		if (1 <= event->y && event->y <= (y - 2)) {
			arg.i = data->wsel->start.i + event->y - 1;
			cmd_set_selected(data, &arg);

			if (event->bstate & BUTTON3_CLICKED)
				cmd_action(data, NULL);
		}
	} else if (data->view == V_VERTICAL) {
		if (1 <= event->y && event->y <= (y - 2)) {
			if ((wdata_sel_win_index(data) == 0 &&
			     (event->x > (x / 2))) ||
			    (wdata_sel_win_index(data) == 1 &&
			     (event->x < (x / 2))))
				cmd_toggle_win(data, NULL);

			arg.i = data->wsel->start.i + event->y - 1;
			cmd_set_selected(data, &arg);

			if (event->bstate & BUTTON3_CLICKED)
				cmd_action(data, NULL);
		}
	} else {
		if (1 <= event->y && event->y < (y - 2) / 2) {
			if (wdata_sel_win_index(data) == 1)
				cmd_toggle_win(data, NULL);
			arg.i = data->wsel->start.i + event->y - 1;
			cmd_set_selected(data, &arg);

			if (event->bstate & BUTTON3_CLICKED)
				cmd_action(data, NULL);
		} else if (event->y > (y - 2) / 2 && event->y <= (y - 2)) {
			if (wdata_sel_win_index(data) == 0)
				cmd_toggle_win(data, NULL);
			arg.i = data->wsel->start.i + (event->y - (y / 2));
			cmd_set_selected(data, &arg);

			if (event->bstate & BUTTON3_CLICKED)
				cmd_action(data, NULL);
		}
	}
}

int cmd_chcmd(struct wdata *data, const struct arg *arg)
{
	(void)data;
	(void)arg;

	states_push(cmd_state());

	return 1;
}
