#include "cmd_state.h"

#define CMD_BUFSIZE 8092

struct cmd {
	char *name;
	void (*func) (struct wdata *, const struct arg *);
};

struct ac_node {
	char *name;
	struct ac_node *next;
};

static void activate(void);
static void execute(struct wdata *);
static void handle_key(struct wdata *, int);
static void reset(void);

static void autocomplete_clear(void);
static void autocomplete_free(struct ac_node *);
static struct ac_node *autocomplete_get(const char *);
static void autocomplete_handle(void);
static void autocomplete_next(void);
static void autocomplete_retrieve(void);

static struct cmd cmds[] = {
	{"sh", cmd_shell},
	{"quit", cmd_quit},
	{"q", cmd_quit},
	{"mkdir", cmd_mkdir},
	{"cd", cmd_set_path}
};

struct {
	char buf[CMD_BUFSIZE + 1];
	int bufpos;
} cmd_data;

struct {
	struct ac_node *node;
	struct ac_node *curr;
} ac_data;

void activate()
{
	reset();
	set_message(M_INFO, ":%s", cmd_data.buf);
}

void execute(struct wdata *data)
{
	char tmp[CMD_BUFSIZE + 1];
	char *name;
	int i;
	struct arg arg;

	strcpy(tmp, cmd_data.buf);

	name = strtok(tmp, " ");
	arg.s = strtok(NULL, " ");

	if (!name) {
		/* no command entered, leave cmd state */
		set_message(M_INFO, "");
	} else {
		for (i = 0; i < ARRSIZE(cmds); i++) {
			if (streq(name, cmds[i].name)) {
				cmds[i].func(data, &arg);
				return;
			}
		}

		/* no command found, print error message */
		set_message(M_ERROR, "no such command: '%s' ", name);
	}
}

void reset()
{
	autocomplete_clear();

	cmd_data.buf[(cmd_data.bufpos = 0)] = '\0';
}

/* clear and reset autocomplete data */
void autocomplete_clear()
{
	autocomplete_free(ac_data.node);
	ac_data.node = NULL;
	ac_data.curr = NULL;
}

/* recursively free all nodes */
void autocomplete_free(struct ac_node *node)
{
	struct ac_node *tmp;

	while (node) {
		tmp = node;
		node = node->next;
		free(tmp);
	}
}

/* get list of command names that begins with str */
struct ac_node *autocomplete_get(const char *str)
{
	struct ac_node *node = NULL;
	struct ac_node *tmp;
	int i;

	for (i = 0; i < ARRSIZE(cmds); i++) {
		if (strncmp(cmds[i].name, str, strlen(str)) == 0) {
			if (!(tmp = malloc(sizeof(struct ac_node))))
				oom();
			tmp->next = node;
			tmp->name = cmds[i].name;
			node = tmp;
		}
	}

	return node;
}

/* handle autocomplete */
void autocomplete_handle()
{
	if (strwcnt(cmd_data.buf) <= 1) {
		if (ac_data.node) {	/* change to next command */
			autocomplete_next();
		} else {	/* retrieve autocomplete list */
			autocomplete_retrieve();
		}
	} else {
		/* handle argument completion */
	}
}

void autocomplete_next()
{
	if (!(ac_data.curr = ac_data.curr->next))
		ac_data.curr = ac_data.node;

	strrfw(cmd_data.buf, CMD_BUFSIZE, ac_data.curr->name);
	cmd_data.bufpos = strlen(cmd_data.buf);
	set_message(M_INFO, ":%s", cmd_data.buf);
}

void autocomplete_retrieve()
{
	char *cmdname;

	if (!(cmdname = strfw(cmd_data.buf))) {
		cmdname = strdup("");
	}

	ac_data.node = autocomplete_get(cmdname);
	ac_data.curr = ac_data.node;
	free(cmdname);

	if (ac_data.curr) {
		strrfw(cmd_data.buf, CMD_BUFSIZE, ac_data.curr->name);
		cmd_data.bufpos = strlen(cmd_data.buf);
		set_message(M_INFO, ":%s", cmd_data.buf);
	} else {
		set_message(M_ERROR, "'%s': no such command", cmd_data.buf);
	}
}

struct state *cmd_state()
{
	return state_create(handle_key, NULL, activate, 0);
}

void handle_key(struct wdata *data, int c)
{
	if (c == 27) {
		reset();
		states_pop();
	} else if (c == 9) {
		autocomplete_handle();
	} else if (c == 13) {
		execute(data);
		reset();
		states_pop();
	} else if (c == 263) {
		if (cmd_data.bufpos > 0) {
			autocomplete_clear();
			cmd_data.buf[--cmd_data.bufpos] = '\0';
			set_message(M_INFO, ":%s", cmd_data.buf);
		} else {
			set_message(M_INFO, "");
			states_pop();
		}
	} else if (c >= 32) {
		if (cmd_data.bufpos < CMD_BUFSIZE) {
			autocomplete_clear();
			cmd_data.buf[cmd_data.bufpos++] = c;
			cmd_data.buf[cmd_data.bufpos] = '\0';
			set_message(M_INFO, ":%s", cmd_data.buf);
		}
	}
}
