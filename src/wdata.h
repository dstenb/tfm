#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <pthread.h>
#include "dwindow.h"

typedef enum {
	V_SINGLE,
	V_VERTICAL,
	V_HORIZONTAL,
	N_VIEWS
} view_t;

typedef struct {
	dwindow *win[2];
	dwindow *wsel;
	view_t view;
	pthread_mutex_t mutex;
} wdata_t;

void wdata_handle_resize(wdata_t * data);

int wdata_sel_win_index(wdata_t *);

void wdata_set_view(wdata_t * data, view_t view);

void wdata_lock_mutex(wdata_t * data);

void wdata_unlock_mutex(wdata_t * data);

#endif
