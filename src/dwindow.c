#include "dwindow.h"

static void dwindow_clear(struct dwindow *, struct finfo **);
static void dwindow_fix_finfo_data(struct finfo *, struct finfo *);
static void dwindow_prepend(struct dwindow *, struct finfo *);
static void dwindow_read_files(struct dwindow *, DIR *);

static int finfo_cmp(struct finfo *, struct finfo *, finfocmp *);
static struct finfo *merge(struct finfo *, struct finfo *, finfocmp *);
static struct finfo *mergesort(struct finfo *, finfocmp *);

static int valid_name(const char *, int);

void dwindow_clear(struct dwindow *dwin, struct finfo **files)
{
	if (files) {
		*files = dwin->files;
	} else {
		finfo_free_all(dwin->files);
	}

	dwin->files = NULL;
	dwin->size = 0;
	dwin->sel.p = NULL;
	dwin->sel.i = 0;
	dwin->start.p = NULL;
	dwin->sel.i = 0;

	finfo_free(dwin->dirinfo);
	free(dwin->path);
}

struct dwindow *dwindow_create()
{
	struct dwindow *dwin;

	if (!(dwin = calloc(1, sizeof(struct dwindow))))
		oom();
	return dwin;
}

void dwindow_fix_bounds(struct dwindow *dwin)
{
	if (!(has_selected_file(dwin)))
		return;

	while ((int)(dwin->sel.i - dwin->start.i) >= dwin->winsize &&
	       dwin->start.p->next) {
		dwin->start.p = dwin->start.p->next;
		dwin->start.i++;
	}

	if (dwin->sel.i < dwin->start.i) {
		dwin->start.p = dwin->sel.p;
		dwin->start.i = dwin->sel.i;
	}
}

void dwindow_free(struct dwindow *dwin)
{
	if (dwin) {
		dwindow_clear(dwin, NULL);
		free(dwin);
	}
}

void dwindow_prepend(struct dwindow *dwin, struct finfo *fp)
{
	fp->next = dwin->files;

	if (dwin->files)
		dwin->files->prev = fp;
	dwin->files = fp;
	dwin->size++;
}

int dwindow_read(struct dwindow *dwin, const char *path)
{
	DIR *dir;
	char rpath[PATH_MAX];
	struct finfo *fp;
	struct finfo *old_files = NULL;

	realpath(path, rpath);

	if (!(dir = opendir(rpath))) {
		/* try to read previous dir */

		if (!streq(rpath, "/")) {
			prevdir(rpath);
			return dwindow_read(dwin, rpath);
		}

		return errno;
	}

	if (dwin->path && streq(dwin->path, rpath))
		dwindow_clear(dwin, &old_files);
	else
		dwindow_clear(dwin, NULL);

	if (!(dwin->path = strdup(rpath)))
		oom();

	dwindow_read_files(dwin, dir);

	if (!streq(dwin->path, "/")) {
		fp = finfo_create(dwin->path, "..");

		if (finfo_stat(fp) == 0) {
			dwindow_prepend(dwin, fp);
		} else {
			finfo_free(fp);
		}
	}

	/* set dir info struct */
	dwin->dirinfo = finfo_create(dwin->path, ".");
	finfo_stat(dwin->dirinfo);

	closedir(dir);

	dwindow_sort(dwin);
	dwin->sel.p = dwin->files;
	dwin->start.p = dwin->files;
	dwin->sel.i = dwin->start.i = 0;

	/* reload finfo data if reloading a directory */
	if (old_files) {
		dwindow_fix_finfo_data(old_files, dwin->files);
		finfo_free_all(old_files);
	}

	return 0;
}

void dwindow_fix_finfo_data(struct finfo *old, struct finfo *new)
{
	struct finfo *tmp;

	for ( ; old; old = old->next) {
		for (tmp = new; tmp; tmp = tmp->next) {
			int cmp = strcmp(old->name, tmp->name);

			if (cmp == 0) {
				tmp->marked = old->marked;
				break;
			}
		}
	}
}

void dwindow_read_files(struct dwindow *dwin, DIR * dir)
{
	struct dirent *de;
	struct finfo *fp;

	while ((de = readdir(dir))) {
		if (valid_name(de->d_name, dwin->show_dot)) {
			fp = finfo_create(dwin->path, de->d_name);

			if (finfo_stat(fp) == 0) {
				dwindow_prepend(dwin, fp);
			} else {
				finfo_free(fp);
			}
		}
	}
}

void dwindow_reload(struct dwindow *dwin)
{
	char *name = NULL;

	if (!(dwin->path))
		return;

	if (has_selected_file(dwin)) {
		if (!(name = strdup(dwin->sel.p->name)))
			oom();
	}

	dwindow_read(dwin, dwin->path);

	if (name) {
		dwindow_set_selected_by_name(dwin, name);
		free(name);
	}
}

void dwindow_set_selected(struct dwindow *dwin, pos_t pos)
{
	pos_t i;
	struct finfo *t;

	if (!dwin->files)
		return;

	for (i = 0, t = dwin->files; i < pos && t->next; i++, t = t->next) {
	}

	dwin->sel.p = t;
	dwin->sel.i = i;

	dwindow_fix_bounds(dwin);
}

int dwindow_set_selected_by_name(struct dwindow *dwin, const char *name)
{
	struct finfo *fp;
	pos_t i;

	for (i = 0, fp = dwin->files; fp; i++, fp = fp->next) {
		if (streq(fp->name, name)) {
			dwindow_set_selected(dwin, i);
			return 1;
		}
	}

	return 0;
}

void dwindow_set_sort(struct dwindow *dwin, int sort)
{
	if (sort >= 0 && sort < N_SORTMETHODS) {
		dwin->sort = sort;

		if (sort == BY_SIZE)
			dwin->cmp = finfocmp_size;
		else if (sort == BY_MTIME)
			dwin->cmp = finfocmp_mtime;
		else
			dwin->cmp = finfocmp_name;

		dwindow_sort(dwin);
	}
}

void dwindow_set_winsize(struct dwindow *dwin, int winsize)
{
	dwin->winsize = winsize;
	dwindow_fix_bounds(dwin);
}

void dwindow_show_dotfiles(struct dwindow *dwin, int show)
{
	dwin->show_dot = show;
	dwindow_reload(dwin);
}

void dwindow_sort(struct dwindow *dwin)
{
	if (!dwin || !dwin->files || !dwin->files->next || !dwin->cmp)
		return;

	/* skip sorting '..' finfo struct if present */
	if (streq(dwin->files->name, "..")) {
		dwin->files->next = mergesort(dwin->files->next, dwin->cmp);
		dwin->files->next->prev = dwin->files;
	} else {
		dwin->files = mergesort(dwin->files, dwin->cmp);
	}

	dwindow_fix_bounds(dwin);
}

int finfo_cmp(struct finfo *a, struct finfo *b, finfocmp * cmp)
{
	if (F_ISDIR(a) == F_ISDIR(b))
		return cmp(a, b);
	else
		return F_ISDIR(a) ? -1 : 1;
}

struct finfo *merge(struct finfo *a, struct finfo *b, finfocmp * cmp)
{
	struct finfo *res;

	if (!a)
		return b;
	if (!b)
		return a;

	if (finfo_cmp(a, b, cmp) < 0) {
		res = a;
		res->next = merge(a->next, b, cmp);
	} else {
		res = b;
		res->next = merge(a, b->next, cmp);
	}

	res->next->prev = res;
	return res;
}

struct finfo *mergesort(struct finfo *head, finfocmp * cmp)
{
	struct finfo *a;
	struct finfo *b;

	if (!head || !head->next)
		return head;

	a = head;
	b = head->next;

	while (b && b->next) {
		head = head->next;
		b = head->next->next;
	}

	b = head->next;
	head->next = NULL;

	return merge(mergesort(a, cmp), mergesort(b, cmp), cmp);
}

const char *strsort(int sort)
{
	switch (sort) {
	case BY_NAME:
		return "name";
	case BY_SIZE:
		return "size";
	case BY_MTIME:
		return "modify time";
	default:
		return NULL;
	}
}

int valid_name(const char *name, int show_dot)
{
	if (show_dot)
		return !streq(name, ".") && !streq(name, "..");
	else
		return *name != '.';

}
