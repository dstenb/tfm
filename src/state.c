#include "state.h"

struct snode {
	struct state *state;
	struct snode *prev, *next;
};

static struct snode *snode_create(struct state *, struct snode *,
				  struct snode *);
static void snode_free(struct snode *);

static struct snode *c_node = NULL;

struct snode *snode_create(struct state *s, struct snode *p, struct snode *n)
{
	struct snode *sl;

	if (!(sl = malloc(sizeof(struct snode))))
		oom();
	sl->state = s;
	sl->prev = p;
	sl->next = n;
	return sl;
}

void snode_free(struct snode *sl)
{
	if (sl) {
		free(sl->state);
		free(sl);
	}
}

void states_clear()
{
	while (c_node)
		states_pop();
}

struct state *state_create(void (*keycmd) (struct wdata *, int),
			   void (*mousecmd) (struct wdata *, const MEVENT *),
			   void (*activate) (void), int normal_bindings)
{
	struct state *s;

	if (!(s = malloc(sizeof(struct state))))
		oom();
	s->keycmd = keycmd;
	s->mousecmd = mousecmd;
	s->activate = activate;
	s->normal_bindings = normal_bindings;

	return s;
}

void states_handlekey(struct wdata *data, int c)
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

void states_handlemouse(struct wdata *data, const MEVENT * event)
{
	if (!c_node)
		return;

	if (c_node && c_node->state->mousecmd)
		c_node->state->mousecmd(data, event);
}

void states_pop()
{
	struct snode *tmp;

	if (!c_node)
		return;

	tmp = c_node;
	c_node = c_node->prev;
	snode_free(tmp);

	if (c_node && c_node->state->activate)
		c_node->state->activate();
}

void states_push(struct state *s)
{
	assert(s);
	c_node = snode_create(s, c_node, NULL);

	if (s->activate)
		s->activate();
}
