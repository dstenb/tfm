#include "cmd.h"

#define CMD_BUFSIZE PATH_MAX + 100

typedef struct cmd cmd;

struct cmd {
	char *name;
} cmds[] = {
	{ NULL }
};

struct {
	char buf[CMD_BUFSIZE + 1];
	char cmd[CMD_BUFSIZE];
	char **args;
	int i;
} cmd_data;

state
*cmd_state()
{
	state *s;

	*cmd_data.buf = '\0';
	cmd_data.i = 0;
	
	if (!(s = malloc(sizeof(state))))
		die("out of memory\n");
	s->keycmd = cmd_handle_key;
	s->set_buf = cmd_set_buf;
	s->normal_bindings = 0;
	return s;
}

void
cmd_handle_key(wdata_t *data, int c)
{
	/* 9 == TAB */

	if (c == 27) {
		states_pop();
	}
	else if (c == 263) {
		if (cmd_data.i > 0) {
			cmd_data.buf[--cmd_data.i] = '\0';
		}
		else {
			states_pop();
		}
	}
	else if (c >= 32) {
		if (cmd_data.i < CMD_BUFSIZE) {
			cmd_data.buf[cmd_data.i++] = c;
			cmd_data.buf[cmd_data.i] = '\0';
		}
	}
}
void
cmd_set_buf(wdata_t *data)
{
	snprintf(data->buf, data->bufsize, ":%s", cmd_data.buf); 
}
