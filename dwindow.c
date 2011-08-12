#include "dwindow.h"

static void dwindow_clear(dwindow *dwin);
static void dwindow_read_files(dwindow *dwin, DIR *dir);

static int finfo_cmp(finfo *a, finfo *b, finfocmp *cmp);
static finfo *merge(finfo *a, finfo *b, finfocmp *cmp);
static finfo *mergesort(finfo *head, finfocmp *cmp);

void
dwindow_clear(dwindow *dwin)
{
	finfo *t;

	while (dwin->files) {
		t = dwin->files;
		dwin->files = dwin->files->next;
		finfo_free(t);
	}

	dwin->size = 0;
	dwin->sel.p = NULL;
	dwin->sel.i = 0;
	dwin->start.p = NULL;
	dwin->sel.i = 0;

	finfo_free(dwin->dirinfo);
	free(dwin->path);
}

dwindow *
dwindow_create()
{
	dwindow *dwin;

	if (!(dwin = calloc(1, sizeof(dwindow))))
		oom();
	return dwin;
}

void
dwindow_free(dwindow *dwin)
{
	if (dwin) {
		dwindow_clear(dwin);
		free(dwin);
	}
}

int
dwindow_read(dwindow *dwin, const char *path)
{
	DIR *dir;
	char rpath[PATH_MAX];
	finfo *fp;

	realpath(path, rpath);

	if (!(dir = opendir(rpath))) {
		/* try to read previous dir */
		prevdir(rpath);
		if (!streq(rpath, "/"))
			return dwindow_read(dwin, rpath);

		return errno;
	}

	dwindow_clear(dwin);

	if (!(dwin->path = strdup(rpath)))
		oom();
	
	dwindow_read_files(dwin, dir);

	if (!streq(dwin->path, "/")) {
		fp = finfo_create(dwin->path, "..");

		if (finfo_stat(fp) == 0) {
			fp->next = dwin->files;
			if (dwin->files)
				dwin->files->prev = fp;
			dwin->files = fp;
			dwin->size++;
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

	return 0;
}

void
dwindow_read_files(dwindow *dwin, DIR *dir)
{
	struct dirent *de;
	finfo *fp;

	while ((de = readdir(dir))) {
		if (!streq(de->d_name, ".") && !streq(de->d_name, "..")) {
			fp = finfo_create(dwin->path, de->d_name);

			if (finfo_stat(fp) == 0) {
				fp->next = dwin->files;
				if (dwin->files)
					dwin->files->prev = fp;
				dwin->files = fp;
				dwin->size++;
			} else {
				/* TODO: handle errors */
				finfo_free(fp);
			}
		}
	}
}

void
dwindow_set_selected(dwindow *dwin, pos_t pos)
{
	pos_t i;
	finfo *t;

	if (!dwin->files)
		return;

	for (i = 0, t = dwin->files; i < pos && t->next; i++, t = t->next) { }

	dwin->sel.p = t;
	dwin->sel.i = i;

	while((dwin->sel.i - dwin->start.i) >=  dwin->winsize && dwin->start.p->next) {
		dwin->start.p = dwin->start.p->next;
		dwin->start.i++;
	}

	if (dwin->sel.i < dwin->start.i) {
		dwin->start.p = dwin->sel.p;
		dwin->start.i = dwin->sel.i;
	}

}

int
dwindow_set_selected_by_name(dwindow *dwin, const char *name)
{
	finfo *fp;
	pos_t i;

	for (i = 0, fp = dwin->files; fp; i++, fp = fp->next) {
		if (streq(fp->name, name)) {
			dwindow_set_selected(dwin, i);
			return 1;
		}
	}

	return 0;
}

void
dwindow_sort(dwindow *dwin)
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
}

int
finfo_cmp(finfo *a, finfo *b, finfocmp *cmp)
{
	if (F_ISDIR(a) == F_ISDIR(b))
		return cmp(a, b);
	else
		return F_ISDIR(a) ? -1 : 1;
}

finfo *
merge(finfo *a, finfo *b, finfocmp *cmp)
{
	finfo *res;

	if (!a) return b;
	if (!b) return a;

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

finfo *
mergesort(finfo *head, finfocmp *cmp)
{
	finfo *a;
	finfo *b;

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
