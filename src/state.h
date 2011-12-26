#ifndef _STATE_H_
#define _STATE_H_

#include <assert.h>
#ifdef HAVE_NCURSESW_NCURSES_H
#include <ncursesw/ncurses.h>
#else
#include <ncurses.h>
#endif

#include "utils.h"
#include "wdata.h"

struct state {
	void (*keycmd) (struct wdata *, int);
	void (*mousecmd) (struct wdata *, const MEVENT *);
	void (*activate) (void);
	int normal_bindings;
};

/* remove all states */
void states_clear(void);

struct state *state_create(void (*keycmd) (struct wdata *, int),
			   void (*mousecmd) (struct wdata *, const MEVENT *),
			   void (*activate) (void), int);

/* handle key event for current state */
void states_handlekey(struct wdata *, int);

void states_handlemouse(struct wdata *, const MEVENT *);

/* remove current satte */
void states_pop(void);

/* add and set state as current */
void states_push(struct state *);

#endif
