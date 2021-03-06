#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <stdlib.h>
#include <unistd.h>

#include "dwindow.h"
#include "message.h"
#include "program.h"
#include "wdata.h"

/* common commands for various states */

struct arg {
	int i;
	char *s;
};

/* open file or directory */
void cmd_action(struct wdata *, const struct arg *);

/* go down one entry in dselected dwindow */
void cmd_go_down(struct wdata *, const struct arg *);

/* go to the bottom in the selected dwindow */
void cmd_go_end(struct wdata *, const struct arg *);

/* go to the top in the selected dwindow */
void cmd_go_home(struct wdata *, const struct arg *);

/* go up one entry in the selected dwindow */
void cmd_go_up(struct wdata *, const struct arg *);

/* create a directory */
void cmd_mkdir(struct wdata *, const struct arg *);

/* quit program */
void cmd_quit(struct wdata *, const struct arg *);

/* set whether or not dotfiles should be shown for selected dwindow */
void cmd_set_dotfiles(struct wdata *, const struct arg *);

/* set path for selected dwindow */
void cmd_set_path(struct wdata *, const struct arg *);

/* set selected file (absolute pos) for selected dwindow */
void cmd_set_selected(struct wdata *, const struct arg *);

/* set selected file (relative pos) for selected dwindow */
void cmd_set_selected_r(struct wdata *, const struct arg *);

/* set sort method for selected window and sort */
void cmd_set_sort(struct wdata *, const struct arg *);

/* set view and fix bounds */
void cmd_set_view(struct wdata *, const struct arg *);

/* set selected window */
void cmd_set_win(struct wdata *, const struct arg *);

/* spawn a shell in the current directory */
void cmd_shell(struct wdata *, const struct arg *);

/* toggle if dotfiles should be shown for selected dwindow */
void cmd_toggle_dotfiles(struct wdata *, const struct arg *);

/* set next sort method and sort */
void cmd_toggle_sort(struct wdata *, const struct arg *);

/* set next view and fix bounds */
void cmd_toggle_view(struct wdata *, const struct arg *);

/* toggle selected window */
void cmd_toggle_win(struct wdata *, const struct arg *);

void cmd_mark_deselect_all(struct wdata *, const struct arg *);

void cmd_mark_select_all(struct wdata *, const struct arg *);

void cmd_mark_toggle(struct wdata *, const struct arg *);

void cmd_mark_invert(struct wdata *, const struct arg *);

#endif
