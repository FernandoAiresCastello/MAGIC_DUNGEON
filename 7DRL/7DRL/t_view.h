#pragma once
#include "global.h"

struct t_view {

	int scroll_x = 0;
	int scroll_y = 0;

	void setscrl(int x, int y)
	{
		prev_scroll_x = scroll_x;
		prev_scroll_y = scroll_y;

		scroll_x = x;
		scroll_y = y;
	}
	void scroll(int dx, int dy)
	{
		prev_scroll_x = scroll_x;
		prev_scroll_y = scroll_y;

		scroll_x += dx;
		scroll_y += dy;
	}
	void undo_last_scroll()
	{
		scroll_x = prev_scroll_x;
		scroll_y = prev_scroll_y;
	}

private:
	int prev_scroll_x = 0;
	int prev_scroll_y = 0;
};
