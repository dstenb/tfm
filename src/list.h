#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdio.h>

#include "state.h"
#include "cmd.h"
/*#include "search.h"*/
#include "dwindow.h"

#include "commands.h"

state *list_state(void);

void list_handle_key(wdata_t *data, int c);

void list_handle_mouse(wdata_t *data, const MEVENT *event);

#endif
