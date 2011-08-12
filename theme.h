#ifndef _THEME_H_
#define _THEME_H_

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "utils.h"
#include "ui.h"

#ifndef COLOR_DEFAULT
#define COLOR_DEFAULT -1
#endif

typedef enum {
	C_TOPBAR = 1,
	C_FILEBAR,
	C_INFO,
	C_WARNING,
	C_ERROR,
	C_FILE,
	C_DIRECTORY,
	C_SYMLINK,
	C_SELECTED
} theme_color_t;

/* initialize the default theme */
void theme_init_default();

/* read and set theme data from file, 
 * returns 0 if successful, else errno */
int theme_read_from_file(const char *path);

#endif
