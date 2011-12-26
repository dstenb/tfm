#ifndef _DWINDOW_H_
#define _DWINDOW_H_

#include <dirent.h>

#include "ui.h"
#include "finfo.h"
#include "utils.h"

#define has_selected_file(x) (x->sel.p)

typedef size_t pos_t;

typedef enum {
	BY_NAME,
	BY_SIZE,
	BY_MTIME,
	N_SORTMETHODS
} sort_t;

struct dwindow {
	char *path;
	struct finfo *dirinfo;
	struct finfo *files;
	struct {
		struct finfo *p;
		pos_t i;
	} sel;
	struct {
		struct finfo *p;
		pos_t i;
	} start;
	size_t size;
	sort_t sort;
	int winsize;
	finfocmp *cmp;
	int show_dot;
};

/* create a dwindow struct */
struct dwindow *dwindow_create(void);

/* fix bounds so the start pointer is set correctly */
void dwindow_fix_bounds(struct dwindow *);

/* free a dwindow struct, and all it's data */
void dwindow_free(struct dwindow *);

/* read a path. returns 0 if successful, or errno if not */
int dwindow_read(struct dwindow *, const char *);

void dwindow_reload(struct dwindow *);

/* set selected finfo struct by pos */
void dwindow_set_selected(struct dwindow *, pos_t);

/* set selected finfo struct by name. returns 1 if successful, else 0 */
int dwindow_set_selected_by_name(struct dwindow *, const char *);

/* set sort method (and then sort) */
void dwindow_set_sort(struct dwindow *, int);

/* set window size and fix bounds */
void dwindow_set_winsize(struct dwindow *, int);

void dwindow_show_dotfiles(struct dwindow *, int);

/* sorts the files */
void dwindow_sort(struct dwindow *);

const char *strsort(int sort);

#endif
