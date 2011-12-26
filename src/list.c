#include "list.h"

static int cmd_chcmd(wdata_t *, const arg_t *);
static void handle_mouse(wdata_t *, const MEVENT *);
static void handle_key(wdata_t *, int);

struct {
	int key;
	int (*func)(wdata_t *, const arg_t *);
	arg_t arg;
} list_cmds[] = {
	{ 13,  cmd_action,          { 0, 0 } },
	{ 258, cmd_go_down,         { 0, 0 } },
	{ 'j', cmd_go_down,         { 0, 0 } },
	{ 'G', cmd_go_end,          { 0, 0 } },
	{ 'g', cmd_go_home,         { 0, 0 } },
	{ 259, cmd_go_up,           { 0, 0 } },
	{ 'k', cmd_go_up,           { 0, 0 } },
	{ 'u', cmd_set_path,        { 0, ".." } },
	{ 'H', cmd_toggle_dotfiles, { 0, 0 } },
	{ 'S', cmd_toggle_sort,     { 0, 0 } },
	{ 'V', cmd_toggle_view,     { 0, 0 } },
	{ ' ', cmd_toggle_win,      { 0, 0 } },
	{ 't', cmd_toggle_win,      { 0, 0 } },
	{ ':', cmd_chcmd,           { 0, 0 } }
};

state *
list_state()
{
	state *s;

	if (!(s = malloc(sizeof(state))))
		oom();
	s->keycmd = handle_key;
	s->mousecmd = handle_mouse;
	s->activate = NULL;
	s->normal_bindings = 1;
	return s;
}

void
handle_key(wdata_t *data, int c)
{
	int i;

	for (i = 0; i < ARRSIZE(list_cmds); i++) {
		if (c == list_cmds[i].key) {
			list_cmds[i].func(data, &list_cmds[i].arg);
			break;
		}
	}
}

void
handle_mouse(wdata_t *data, const MEVENT *event)
{
	arg_t arg;
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

int
cmd_chcmd(wdata_t *data, const arg_t *arg)
{
	(void)data;
	(void)arg;

	states_push(cmd_state());

	return 1;
}
