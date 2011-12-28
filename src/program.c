#include "program.h"

static char *get_cmd(const char *);
static char *get_ext(const char *);
static void parse_line(const char *, int, char *);

static struct hm *program_hm = NULL;

void program_close()
{
	hm_foreach(program_hm, free);
	hm_free(program_hm);
}

void program_init()
{
	program_hm = hm_init(13);
}

int program_read_from_file(const char *path)
{
	FILE *fp;
	char buf[1024];
	int line = 1;

	if (!(fp = fopen(path, "r")))
		return errno;

	while (fgets(buf, sizeof(buf), fp)) {
		parse_line(path, line, buf);

		line++;
	}

	return 0;
}

void program_spawn(char *path)
{
	char *argv[3] = { get_cmd(path), path, NULL };

	ui_close();
	spawn(argv, 1);
	ui_init();
}

char *get_cmd(const char *path)
{
	char *cmd = hm_get(program_hm, get_ext(path));

	return cmd ? cmd : config()->default_program;
}

char *get_ext(const char *path)
{
	char *p;

	return (p = strrchr(path, '.')) ? ++p : "";
}

void parse_line(const char *path, int nr, char *line)
{
	char *ext;
	char *cmd;

	ext = strtok(line, " =\t\r\n");
	cmd = strtok(NULL, " =\t\r\n");

	if (!ext || *ext == '#')
		return;

	if (!cmd || *cmd == '#')
		die("%s:%i: missing value\n", path, nr);

	printf("%s:%i: (%s) (%s)\n", path, nr, ext, cmd);
	hm_put(program_hm, ext, strdup(cmd));
}
