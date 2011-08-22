#include "config.h"

config_t *
config_default()
{
	config_t *config;

	if (!(config = calloc(1, sizeof(config_t))))
		oom();

	if (!(config->default_program = strdup("vim")))
		oom();
	if (!(config->timefmt = strdup("%y/%m/%d")))
		oom();
	if (!(config->theme = strdup("theme")))
		oom();

	config->show_dot = 0;
	config->sort = BY_NAME;
	config->view = V_VERTICAL;

	return config;
}

void
config_free(config_t *config)
{
	if (!config)
		return;

	free(config->default_program);
	free(config->timefmt);
	free(config->theme);
	free(config);
}

int
config_read(config_t *config, const char *path)
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
				config->show_dot = 1;
			} else if (streq(v, "false")) {
				config->show_dot = 0;
			} else {
				die("%s:%i, unknown value '%s' for '%s'"
						" (exp. 'true', 'false')\n",
						path, i, v, n);
			}
		} else if (streq(n, "sort_by")) {
			if (streq(v, "name")) {
				config->sort = BY_NAME;
			} else if (streq(v, "size")) {
				config->sort = BY_SIZE;
			} else if (streq(v, "mtime")) {
				config->sort = BY_MTIME;
			} else {
				die("%s:%i: unknown value '%s' for '%s'"
						" (exp. 'name', 'size', "
						"'mtime')\n",
						path, i, v, n);
			}
		} else if (streq(n, "default_program")) {
			free(config->default_program);
			if (!(config->default_program = strdup(v)))
				oom();
		} else if (streq(n, "time_fmt")) {
			free(config->timefmt);
			if (!(config->timefmt = strdup(v)))
				oom();
		} else if (streq(n, "view")) {
			if (streq(v, "single")) {
				config->view = V_SINGLE;
			} else if (streq(v, "horizontal")) {
				config->view = V_HORIZONTAL;
			} else if (streq(v, "vertical")) {
				config->view = V_VERTICAL;
			} else {
				die("%s:%i: unknown value '%s' for '%s'"
						" (exp. 'single', "
						"'horizontal', 'vertical'\n",
						path, i, v, n);
			}
		} else if (streq(n, "theme")) {
			free(config->theme);
			if (!(config->theme = strdup(v)))
				oom();
		} else {
			die("%s:%i: unknown name '%s'\n", path, i, n);
		}
	}

	fclose(fp);
	return 0;
}

char *
get_conf_dir()
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
