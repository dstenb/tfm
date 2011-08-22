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

/* return pointer to config struct, TODO: make a better solution than this */
const config_t *config();

/* close config and free all allocated data */
void config_close();

/* initializes config and sets default values */
void config_init();

/* read data in to config from file, returns 0 if ok, else errno */
int config_read(const char *path);

/* create a string containing the absolute path to the configuration directory,
 * returns NULL if not successful */
char *get_conf_dir();

#endif
