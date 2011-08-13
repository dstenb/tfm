#include "theme.h"

static int parse_color(const char *str, int *color);
static int parse_line(char *line, int *id, int *fg, int *bg);

struct {
	int id, fg, bg;
} c_data[] = {
	{ C_TOPBAR,    COLOR_WHITE,  COLOR_DEFAULT },
	{ C_FILEBAR,   COLOR_BLACK,  COLOR_WHITE},
	{ C_INFO,      COLOR_WHITE,  COLOR_DEFAULT},
	{ C_WARNING,   COLOR_YELLOW, COLOR_DEFAULT},
	{ C_ERROR,     COLOR_RED,    COLOR_DEFAULT},
	{ C_FILE,      COLOR_WHITE,  COLOR_DEFAULT},
	{ C_DIRECTORY, COLOR_BLUE,   COLOR_DEFAULT},
	{ C_SYMLINK,   COLOR_GREEN,  COLOR_DEFAULT},
	{ C_SELECTED,  COLOR_WHITE,  COLOR_BLUE}
};

struct {
	char *name;
	int v;
} c_identifiers[] = {
	{ "topbar", C_TOPBAR },
	{ "filebar", C_FILEBAR },
	{ "info", C_INFO },
	{ "warning", C_WARNING },
	{ "error", C_ERROR },
	{ "file", C_FILE },
	{ "directory", C_DIRECTORY },
	{ "symlink", C_SYMLINK },
	{ "selected", C_SELECTED }
};

struct {
	char *name;
	int v;
} c_dictionary[] = {
	{ "default", COLOR_DEFAULT },
	{ "black", COLOR_BLACK },
	{ "red", COLOR_RED },
	{ "green", COLOR_GREEN },
	{ "yellow", COLOR_YELLOW },
	{ "blue", COLOR_BLUE },
	{ "magenta", COLOR_MAGENTA },
	{ "cyan", COLOR_CYAN },
	{ "white", COLOR_WHITE }
};

int
parse_color(const char *str, int *color)
{
	int i;
	
	for (i = 0; i < ARRSIZE(c_dictionary); i++) {
		if (strcasecmp(c_dictionary[i].name, str) == 0) {
			*color = c_dictionary[i].v;
			return 1;
		}
	}

	return 0;
}

int
parse_line(char *line, int *id, int *fg, int *bg)
{
	char *name;
	char *values[2];
	int i;

	name = strtok(line, " =\t\r\n");
	values[0] = strtok(NULL, " =\t\r\n");
	values[1] = strtok(NULL, " =\t\r\n");

	if (!name || *name == '#')
		return 0;

	for (i = 0; i < ARRSIZE(c_identifiers); i++) {
		if (strcasecmp(c_identifiers[i].name, name) == 0) {
			*id = c_identifiers[i].v;

			if (!values[0] || !parse_color(values[0], fg))
				return 0;
			if (!values[1] || !parse_color(values[1], bg))
				return 0;
			break;
		}
	}

	return 1;
}

void
theme_init()
{
	int i;

	for (i = 0; i < ARRSIZE(c_data); i++) {
		init_pair(c_data[i].id, c_data[i].fg, c_data[i].bg);
	}
}

int
theme_read_from_file(const char *path)
{
	FILE *fp;
	char buf[1024];
	int id = 0;
	int fg = COLOR_WHITE;
	int bg = COLOR_BLACK;

	if (!(fp = fopen(path, "r")))
		return errno;

	while(fgets(buf, sizeof(buf), fp)) {
		if (parse_line(buf, &id, &fg, &bg)) {
			if ((id - 1) >= 0 && (id - 1) < ARRSIZE(c_data)) {
				c_data[id - 1].fg = fg;
				c_data[id - 1].bg = bg;
			}
		}
	}

	fclose(fp);

	return 0;
}