#include "state.h"

typedef struct snode snode;

struct snode {
	state *state;
	snode *prev, *next;
};

static snode *snode_create(state *s, snode *p, snode *n);
static void snode_free(snode *sl);

static snode *c_node = NULL;

snode *
snode_create(state *s, snode *p, snode *n)
{
	snode *sl;

	if (!(sl = malloc(sizeof(snode))))
		oom();
	sl->state = s;
	sl->prev = p;
	sl->next = n;
	return sl;
}

void
snode_free(snode *sl)
{
	if (sl) {
		free(sl->state);
		free(sl);
	}
}

void
states_clear()
{
	while (c_node)
		states_pop();
}

void
states_handlekey(wdata_t *data, int c)
{
	int handled = 0;

	if (!c_node)
		return;

	if (c_node->state->normal_bindings) {
		handled = 1;

		if (c == 113) {
			exit(EXIT_SUCCESS);
		} else {
			handled = 0;
		}

	}
	
	if (!handled && c_node->state->keycmd)
		c_node->state->keycmd(data, c);
}

void
states_pop()
{
	snode *tmp;

	if (!c_node)
		return;

	tmp = c_node;
	c_node = c_node->prev;
	snode_free(tmp);
}

void
states_push(state *s)
{
	assert(s);
	c_node = snode_create(s, c_node, NULL);
}

void
states_setbuf(wdata_t *data)
{
	if (!c_node)
		return;

	if (c_node->state->set_buf)
		c_node->state->set_buf(data);
}
