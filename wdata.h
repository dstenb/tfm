#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "dwindow.h"

typedef enum {
	M_INFO,
	M_WARNING,
	M_ERROR
} message_t;

typedef struct {
	dwindow *win[2];
	dwindow *wsel;
	char *buf;
	size_t bufsize;
	message_t buftype;
} wdata_t;

#endif
