#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "dwindow.h"

typedef struct {
	dwindow *win[2];
	dwindow *wsel;
	char *buf;
	size_t bufsize;
} wdata_t;

#endif
