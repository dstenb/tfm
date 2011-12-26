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
	struct finfo *prev;
	struct finfo *next;
};

/* sort comparison prototype */
typedef int finfocmp(const struct finfo *, const struct finfo *);

/* create a finfo struct */
struct finfo *finfo_create(const char *, const char *);

/* free a finfo struct */
void finfo_free(struct finfo *fp);

/* stat and fill data. returns 0 if successful, errno otherwise */
int finfo_stat(struct finfo *fp);

/* comparison functions */
int finfocmp_name(const struct finfo *, const struct finfo *);
int finfocmp_size(const struct finfo *, const struct finfo *);
int finfocmp_mtime(const struct finfo *, const struct finfo *);

/* returns a pointer to a statically allocated string for perm,
 * returns NULL if bad perm */
const char *strperm(int perm);

#endif
