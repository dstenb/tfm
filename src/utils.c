#include "utils.h"

void
die(const char *fmt, ...)
{
	va_list val;

	ui_close();

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
