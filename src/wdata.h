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

struct wdata {
	struct dwindow *win[2];
	struct dwindow *wsel;
	view_t view;
	pthread_mutex_t mutex;
};

void wdata_handle_resize(struct wdata *);

int wdata_sel_win_index(struct wdata *);

void wdata_set_view(struct wdata *, view_t);

void wdata_lock_mutex(struct wdata *);

void wdata_unlock_mutex(struct wdata *);

#endif
