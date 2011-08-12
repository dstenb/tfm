#include "message.h"

static int get_msg_attr(msg_t type);

static msg_t type = C_INFO;
static char buf[512];

int
get_msg_attr(msg_t type)
{
	int attr;

	if (type == M_WARNING)
		attr = COLOR_PAIR(C_WARNING) | A_BOLD;
	else if (type == M_ERROR)
		attr = COLOR_PAIR(C_ERROR) | A_BOLD;
	else
		attr = COLOR_PAIR(C_INFO);

	return attr;
}

void
print_message(int y)
{
	ui_printline(stdscr, y, get_msg_attr(type), buf);
}

void
set_message(msg_t t, const char *fmt, ...)
{
	va_list val;

	type = t;

	va_start(val, fmt);
	vsnprintf(buf, sizeof(buf), fmt, val);
	va_end(val);
}
