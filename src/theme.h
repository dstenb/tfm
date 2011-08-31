#ifndef _THEME_H_
#define _THEME_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "utils.h"
#include "ui.h"

/* declare COLOR_DEFAULT if not set */
#ifndef COLOR_DEFAULT
#define COLOR_DEFAULT -1
#endif

/* handles theme data */

typedef enum {
	C_TOPBAR = 1,
	C_FILEBAR,
	C_INFO,
	C_WARNING,
	C_ERROR,
	C_FILE,
	C_DIRECTORY,
	C_SYMLINK,
	C_SELECTED,
} color_t;

#define N_COLORS 9

/* initialize the theme */
void theme_init(void);

/* read and set theme data from a file, 
 * returns 0 if successful, else errno */
int theme_read_from_file(const char *path);

#endif
