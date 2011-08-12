#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "ui.h"
#include "theme.h"

typedef enum {
	M_INFO,
	M_WARNING,
	M_ERROR
} msg_t;

void print_message(int y);

void set_message(msg_t type, const char *fmt, ...);

#endif
