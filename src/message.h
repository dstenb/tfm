#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "theme.h"
#include "ui.h"

typedef enum {
	M_INFO,
	M_WARNING,
	M_ERROR
} msg_t;

void print_message(int);

void set_message(msg_t, const char *, ...);

#endif
