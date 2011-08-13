#include "finfo.h"

static const char *permstr[] = { 
	"---", "--x", "-w-", "-wx", "r--", "r-x", "rw-", "rwx"
};

finfo *
finfo_create(const char *dir, const char *name)
{
	size_t ds = strlen(dir);
	size_t ns = strlen(name);
	finfo *fp;
	
	if (!(fp = calloc(1, sizeof(finfo))))
		oom();
	
	if (!(fp->name = strdup(name)))
		oom();

	if (!(fp->path = calloc(ns+ds+2, sizeof(char))))
		oom();

	snprintf(fp->path, ns+ds+2, "%s/%s", dir, name);

	return fp;
}

void
finfo_free(finfo *fp)
{
	if (fp) {
		free(fp->path);
		free(fp->name);
		free(fp);
	}
}

int
finfo_stat(finfo *fp)
{
	struct stat st;

	if ((lstat(fp->path, &st)) != 0)
		return errno;

	fp->perms.u = (st.st_mode >> 6) & 7;
	fp->perms.g = (st.st_mode >> 3) & 7;
	fp->perms.o = (st.st_mode) & 7;
	fp->gid = st.st_gid;
	fp->uid = st.st_uid;
	fp->size = st.st_size;
	fp->mtime = st.st_mtime;
	fp->flags = 0;

	if (S_ISDIR(st.st_mode))
		fp->flags |= DIRECTORY;
	if (S_ISLNK(st.st_mode))
		fp->flags |= SYMLINK;

	return 0;
}

int
finfocmp_name(const finfo *a, const finfo *b)
{
	return strcmp(a->name, b->name);
}

int
finfocmp_size(const finfo *a, const finfo *b)
{
	if (a->size > b->size)
		return 1;
	else if (a->size < b->size)
		return -1;
	else
		return finfocmp_name(a, b);
}

int
finfocmp_mtime(const finfo *a, const finfo *b)
{
	if (a->mtime > b->mtime)
		return 1;
	else if (a->mtime < b->mtime)
		return -1;
	else
		return finfocmp_name(a, b);
}

const char *
strperm(int perm)
{
	return (perm >= 0 && perm <= 7) ? permstr[perm] : NULL;
}
