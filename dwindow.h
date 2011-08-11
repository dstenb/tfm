#ifndef _DWINDOW_H_
#define _DWINDOW_H_

#include <dirent.h>

#include "ui.h"
#include "finfo.h"
#include "utils.h"

typedef size_t pos_t;

typedef enum {
	BY_NAME,
	BY_SIZE,
	BY_MTIME,
	N_SORTMETHODS
} sort_t;

typedef struct {
	char *path;
	finfo *dirinfo;
	finfo *files;
	struct {
		finfo *p;
		pos_t i;
	} sel;
	struct {
		finfo *p;
		pos_t i;
	} start;
	size_t size;
	sort_t sort;
	int winsize;
	finfocmp *cmp;
} dwindow;

/* create a dwindow struct */
dwindow *dwindow_create();

/* free a dwindow struct, and all it's data */
void dwindow_free(dwindow *dwin);

/* read a path. returns 0 if successful, or errno if not */
int dwindow_read(dwindow *dwin, const char *path);

/* set selected finfo struct by pos */
void dwindow_set_selected(dwindow *dwin, pos_t pos);

/* set selected finfo struct by name. returns 1 if successful, else 0 */
int dwindow_set_selected_by_name(dwindow *dwin, const char *name);

/* set sort method (and then sort) */
void dwindow_set_sort(dwindow *dwin, int sort);

/* sorts the files */
void dwindow_sort(dwindow *dwin);

#endif
