#ifndef _STATE_H_
#define _STATE_H_

#include <assert.h>
#ifdef HAVE_NCURSESW_NCURSES_H
#include <ncursesw/ncurses.h>
#else
#include <ncurses.h>
#endif

#include "wdata.h"
#include "utils.h"

typedef struct {
	void (*keycmd) (wdata_t *data, int c);
	void (*set_buf) (wdata_t *data);
	int normal_bindings;
} state;

/* remove all states */
void states_clear();

/* handle key event for current state */
void states_handlekey(wdata_t *data, int c);

void states_setbuf(wdata_t *data);

/* remove current satte */
void states_pop();

/* add and set state as current */
void states_push(state *s);

#endif
