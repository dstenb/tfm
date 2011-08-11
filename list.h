#ifndef _LIST_H_
#define _LIST_H_

#include <stdlib.h>
#include <stdio.h>

#include "state.h"
#include "cmd.h"
#include "dwindow.h"

state *list_state();

void list_handle_key(wdata_t *data, int c);

void list_set_buf(wdata_t *data);

#endif
