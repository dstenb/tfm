#include "cmd.h"

#define CMD_BUFSIZE 8092

typedef struct cmd cmd;
typedef struct ac_node ac_node;

struct cmd {
	char *name;
	int (*func)(wdata_t *data, const arg_t *arg);
};

struct ac_node {
	char *name;
	ac_node *next;
};

static void activate();
static void execute(wdata_t *data);
static void reset();

static void autocomplete_clear();
static void autocomplete_free(ac_node *anode);
static ac_node *autocomplete_get(const char *str);
static void autocomplete_handle();
static int autocomplete_length(ac_node *anode);
static void autocomplete_next();
static void autocomplete_retrieve();
static char *get_first_word(const char *txt);
static int no_of_words(const char *txt);
static void replace_first_word(char *txt, size_t size, const char *nw);

static cmd cmds[] = {
	{ "cd", cmd_set_path },
	{ "q", cmd_quit },
	{ "quit", cmd_quit },
	{ "sh", cmd_shell }
};

struct {
	char buf[CMD_BUFSIZE + 1];
	int bufpos;
} cmd_data;

struct {
	ac_node *node;
	ac_node *curr;
} ac_data;

void
activate()
{
	reset();
	set_message(M_INFO, ":%s", cmd_data.buf);
}

void
execute(wdata_t *data)
{
	char tmp[CMD_BUFSIZE + 1];
	char *name;
	int i;
	arg_t arg;

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

void
reset()
{
	autocomplete_clear();

	cmd_data.buf[(cmd_data.bufpos = 0)] = '\0';
}

/* clear and reset autocomplete data */
void
autocomplete_clear()
{
	autocomplete_free(ac_data.node);
	ac_data.node = NULL;
	ac_data.curr = NULL;
}

/* recursively free all nodes */
void
autocomplete_free(ac_node *anode)
{
	ac_node *tmp;

	while (anode) {
		tmp = anode;
		anode = anode->next;
		free(tmp);
	}
}

/* get list of command names that begins with str */
ac_node *
autocomplete_get(const char *str)
{
	ac_node *anode = NULL;
	ac_node *tmp;
	int i;

	for (i = 0; i < ARRSIZE(cmds); i++) {
		if (strstr(cmds[i].name, str) == cmds[i].name) {
			if (!(tmp = malloc(sizeof(ac_node))))
				oom();
			tmp->next = anode;
			tmp->name = cmds[i].name;
			anode = tmp;
		}
	}

	return anode;
}

/* handle autocomplete */
void
autocomplete_handle()
{
	if (no_of_words(cmd_data.buf) <= 1) {
		if (ac_data.node) { /* change to next command */
			autocomplete_next();
		} else { /* retrieve autocomplete list */
			autocomplete_retrieve();
		}
	} else {
		/* handle argument completition */
	}
}

int
autocomplete_length(ac_node *anode)
{
	int i;

	for (i = 0; anode; i++, anode = anode->next) {
		NULL;
	}

	return i;
}

void
autocomplete_next()
{
	if (!(ac_data.curr = ac_data.curr->next))
		ac_data.curr = ac_data.node;

	replace_first_word(cmd_data.buf, CMD_BUFSIZE, ac_data.curr->name);
	cmd_data.bufpos = strlen(cmd_data.buf);
	set_message(M_INFO, ":%s", cmd_data.buf);
}

void
autocomplete_retrieve()
{
	char *cmdname;

	if (!(cmdname = get_first_word(cmd_data.buf))) {
		cmdname = strdup("");
	}

	ac_data.node = autocomplete_get(cmdname);
	ac_data.curr = ac_data.node;
	free(cmdname);

	if (ac_data.curr) {
		replace_first_word(cmd_data.buf, CMD_BUFSIZE, ac_data.curr->name);
		cmd_data.bufpos = strlen(cmd_data.buf);
		set_message(M_INFO, ":%s", cmd_data.buf);
	} else {
		set_message(M_ERROR, "'%s': no such command", cmd_data.buf);
	}
}

/* get first word from text */
char *
get_first_word(const char *txt)
{
	char buf[strlen(txt) + 1];
	char *p;
	char *q;

	strcpy(buf, txt);
	p = strtok_r(buf, " \t", &q);

	return p ? strdup(p) : NULL;
}

int
no_of_words(const char *txt)
{
	int i = 0;
	int at_word = 0;

	for ( ; *txt; txt++) {
		if (*txt == ' ') {
			at_word = 0;
		} else if (!at_word) {
			at_word = 1;
			i++;
		}
	}

	return i;
}

void
replace_first_word(char *txt, size_t size, const char *nw)
{
	char buf[size];
	char *q;

	strcpy(buf, txt);
	strtok_r(buf, " \t", &q);

	snprintf(txt, size, "%s%s", nw, q);
}

state *
cmd_state()
{
	state *s;

	if (!(s = malloc(sizeof(state))))
		die("out of memory\n");
	s->keycmd = cmd_handle_key;
	s->activate = activate;
	s->normal_bindings = 0;
	return s;
}

void
cmd_handle_key(wdata_t *data, int c)
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
