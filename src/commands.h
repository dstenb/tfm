#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <stdlib.h>
#include <unistd.h>

#include "dwindow.h"
#include "message.h"
#include "wdata.h"

/* common commands for various states */

typedef struct {
	int i;
	char *s;
} arg_t;

int cmd_action(wdata_t *data, const arg_t *arg);

/* go down one entry in dselected dwindow */
int cmd_go_down(wdata_t *data, const arg_t *arg);

/* go to the bottom in the selected dwindow */
int cmd_go_end(wdata_t *data, const arg_t *arg);

/* go to the top in the selected dwindow */
int cmd_go_home(wdata_t *data, const arg_t *arg);

/* go up one entry in the selected dwindow */
int cmd_go_up(wdata_t *data, const arg_t *arg);

/* quit program */
int cmd_quit(wdata_t *data, const arg_t *arg);

/* set whether or not dotfiles should be shown for selected dwindow */
int cmd_set_dotfiles(wdata_t *data, const arg_t *arg);

/* set path for selected dwindow */
int cmd_set_path(wdata_t *data, const arg_t *arg);

/* set selected file (absolute pos) for selected dwindow */
int cmd_set_selected(wdata_t *data, const arg_t *arg);

/* set selected file (relative pos) for selected dwindow */
int cmd_set_selected_r(wdata_t *data, const arg_t *arg);

/* set sort method for selected window and sort */
int cmd_set_sort(wdata_t *data, const arg_t *arg);

/* set view and fix bounds */
int cmd_set_view(wdata_t *data, const arg_t *arg);

/* set selected window */
int cmd_set_win(wdata_t *data, const arg_t *arg);

int cmd_shell(wdata_t *data, const arg_t *arg);

/* toggle if dotfiles should be shown for selected dwindow */
int cmd_toggle_dotfiles(wdata_t *data, const arg_t *arg);

/* set next sort method and sort */
int cmd_toggle_sort(wdata_t *data, const arg_t *arg);

/* set next view and fix bounds */
int cmd_toggle_view(wdata_t *data, const arg_t *arg);

/* toggle selected window */
int cmd_toggle_win(wdata_t *data, const arg_t *arg);

#endif
