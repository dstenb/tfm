#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "config.h"
#include "hashmap.h"
#include "ui.h"

void program_close(void);

void program_init(void);

int program_read_from_file(const char *);

void program_spawn(char *);

#endif
