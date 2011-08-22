#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdlib.h>
#include <stdio.h>

#include "dwindow.h"
#include "wdata.h"
#include "utils.h"

typedef struct {
	char *default_program;
	char *theme;
	char *timefmt;
	int show_dot;
	int sort;
	int view;
} config_t;

/* creates a config struct with default values */
config_t *config_default();

/* free a config struct and all its data */
void config_free(config_t *config);

/* read data in to config struct from file, returns 0 if ok, else errno */
int config_read(config_t *config, const char *path);

#endif
