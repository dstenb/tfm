#ifndef _FINFO_H_
#define _FINFO_H_

#include <errno.h>
#include <sys/stat.h>

#include "utils.h"

#define F_ISDIR(x) (x->flags & DIRECTORY)
#define F_ISLNK(x) (x->flags & SYMLINK)

enum {
	DIRECTORY = 1 << 0,
	SYMLINK = 1 << 1
};

typedef struct finfo finfo;

struct finfo {
	char *path;
	char *name;
	unsigned char flags;
	struct {
		int u, g, o;
	} perms;
	uid_t uid;
	gid_t gid;
	off_t size;
	time_t mtime;
	finfo *prev;
	finfo *next;
};

/* sort comparison prototype */
typedef int finfocmp(const finfo *, const finfo * b);

/* create a finfo struct */
finfo *finfo_create(const char *dir, const char *name);

/* free a finfo struct */
void finfo_free(finfo * fp);

/* stat and fill data. returns 0 if successful, errno otherwise */
int finfo_stat(finfo * fp);

/* comparison functions */
int finfocmp_name(const finfo *, const finfo *);
int finfocmp_size(const finfo *, const finfo *);
int finfocmp_mtime(const finfo *, const finfo *);

/* returns a pointer to a statically allocated string for perm, 
 * returns NULL if bad perm */
const char *strperm(int perm);

#endif
