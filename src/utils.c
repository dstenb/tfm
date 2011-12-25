#include "utils.h"

void
die(const char *fmt, ...)
{
	va_list val;

#ifdef DIE_UI_FIX
	ui_close();
#endif

	va_start(val, fmt);
	vfprintf(stderr, fmt, val);
	va_end(val);
	exit(EXIT_FAILURE);
}

void
oom()
{
	die("out of memory\n");
}

void
prevdir(char *path)
{
	char *p;

	if ((p = strrchr(path, '/'))) {
		if (p == path)
			*(p + 1) = '\0';
		else
			*p = '\0';
	}
}

int
spawn(char **argv, int foreground)
{
	pid_t pid;

	if ((pid = fork()) == 0) {
		execvp(*argv, argv);
		exit(1);
	} else if (pid > 0) {
		if (foreground)
			wait(NULL);
		return 0;
	} else {
		return errno;
	}
}

char *
strsize(size_t size)
{
	char buf[32];
	float f;
	char *s;

	if (size < 1024) {
		s = "B";
		f = (float)size;
	} else if (size < (1024 * 1024)) {
		s = "kB";
		f = size / 1024.0;
	} else if (size < (1024 * 1024 * 1024)) {
		s = "MB";
		f = size / (1024.0 * 1024.0);
	} else {
		s = "GB";
		f = size / (1024.0 * 1024.0 * 1024.0);
	}

	snprintf(buf, sizeof(buf), "%.1f %s", f, s);
	return strdup(buf);
}

int
strwcnt(const char *str)
{
	int i = 0;
	int word = 0;

	for ( ; *str; str++) {
		if (*str == ' ') {
			word = 0;
		} else if (!word) {
			word = 1;
			i++;
		}
	}

	return i;
}

char *
strfw(const char *str)
{
	char buf[strlen(str) + 1];
	char *p;
	char *q;

	strcpy(buf, str);
	p = strtok_r(buf, " \t", &q);

	return p ? strdup(p) : NULL;
}

void
strrfw(char *str, size_t size, const char *word)
{
	char buf[size];
	char *q;

	strcpy(buf, str);
	strtok_r(buf, " \t", &q);

	snprintf(str, size, "%s%s", word, q);
}
