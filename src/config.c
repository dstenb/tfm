#include "config.h"

static config_t configuration;

const config_t *config()
{
	return &configuration;
}

void config_close()
{
	free(configuration.default_program);
	free(configuration.timefmt);
	free(configuration.theme);
}

void config_init()
{
	if (!(configuration.default_program = strdup("vim")))
		oom();
	if (!(configuration.timefmt = strdup("%y/%m/%d")))
		oom();
	if (!(configuration.theme = strdup("theme")))
		oom();

	configuration.show_dot = 0;
	configuration.sort = BY_NAME;
	configuration.view = V_VERTICAL;
}

int config_read(const char *path)
{
	FILE *fp;
	char buf[4096];
	char *n, *s, *v;
	int i = 0;

	if (!(fp = fopen(path, "r")))
		return errno;

	while (fgets(buf, sizeof(buf), fp)) {
		i++;

		n = strtok(buf, " \t\n");
		s = strtok(NULL, " \t\n");
		v = strtok(NULL, " \t\n");

		if (!n || *n == '#' || *n == '\0')
			continue;
		else if (!s || *s != '=' || !v)
			die("%s:%i: missing value\n", path, i);

		if (streq(n, "show_dotfiles")) {
			if (streq(v, "true")) {
				configuration.show_dot = 1;
			} else if (streq(v, "false")) {
				configuration.show_dot = 0;
			} else {
				die("%s:%i, unknown value '%s' for '%s'"
				    " (exp. 'true', 'false')\n", path, i, v, n);
			}
		} else if (streq(n, "sort_by")) {
			if (streq(v, "name")) {
				configuration.sort = BY_NAME;
			} else if (streq(v, "size")) {
				configuration.sort = BY_SIZE;
			} else if (streq(v, "mtime")) {
				configuration.sort = BY_MTIME;
			} else {
				die("%s:%i: unknown value '%s' for '%s'"
				    " (exp. 'name', 'size', "
				    "'mtime')\n", path, i, v, n);
			}
		} else if (streq(n, "default_program")) {
			free(configuration.default_program);
			if (!(configuration.default_program = strdup(v)))
				oom();
		} else if (streq(n, "time_fmt")) {
			free(configuration.timefmt);
			if (!(configuration.timefmt = strdup(v)))
				oom();
		} else if (streq(n, "view")) {
			if (streq(v, "single")) {
				configuration.view = V_SINGLE;
			} else if (streq(v, "horizontal")) {
				configuration.view = V_HORIZONTAL;
			} else if (streq(v, "vertical")) {
				configuration.view = V_VERTICAL;
			} else {
				die("%s:%i: unknown value '%s' for '%s'"
				    " (exp. 'single', "
				    "'horizontal', 'vertical'\n",
				    path, i, v, n);
			}
		} else if (streq(n, "theme")) {
			free(configuration.theme);
			if (!(configuration.theme = strdup(v)))
				oom();
		} else {
			die("%s:%i: unknown name '%s'\n", path, i, n);
		}
	}

	fclose(fp);
	return 0;
}

char *get_conf_dir()
{
	char buf[PATH_MAX];
	char *home;
	char *path = NULL;

	if ((home = getenv("HOME"))) {
		snprintf(buf, sizeof(buf), "%s/.tfm", home);
		if (!(path = strdup(buf)))
			oom();
	}

	return path;
}
