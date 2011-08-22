#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "commands.h"
#include "message.h"
#include "state.h"

state *cmd_state();

void cmd_handle_key(wdata_t *data, int c);

#endif
