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
	void (*mousecmd) (wdata_t *data, const MEVENT *event);
	void (*activate) (void);
	int normal_bindings;
} state;

/* remove all states */
void states_clear(void);

/* handle key event for current state */
void states_handlekey(wdata_t *data, int c);

void states_handlemouse(wdata_t *data, const MEVENT *event);

/* remove current satte */
void states_pop(void);

/* add and set state as current */
void states_push(state *s);

#endif
