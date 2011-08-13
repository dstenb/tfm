#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "dwindow.h"

typedef enum {
	V_SINGLE,
	V_VERTICAL,
/*	V_HORIZONTAL,
*/
	N_VIEWS
} view_t;

typedef struct {
	dwindow *win[2];
	dwindow *wsel;
	view_t view;
} wdata_t;

void wdata_handle_resize(wdata_t *data, int y, int x);

void wdata_set_view(wdata_t *data, view_t view);

#endif
