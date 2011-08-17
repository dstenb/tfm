#include "list.h"

static int cmd_chcmd(wdata_t *data, const arg_t *arg);

struct {
	int key;
	int (*func)(wdata_t *data, const arg_t *arg);
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
	s->keycmd = list_handle_key;
	s->activate = NULL;
	s->normal_bindings = 1;
	return s;
}

void
list_handle_key(wdata_t *data, int c)
{
	int i;

	for (i = 0; i < ARRSIZE(list_cmds); i++) {
		if (c == list_cmds[i].key) {
			list_cmds[i].func(data, &list_cmds[i].arg);
			break;
		}
	}
}

int
cmd_chcmd(wdata_t *data, const arg_t *arg)
{
	states_push(cmd_state());

	return 1;
}
