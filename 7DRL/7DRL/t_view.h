#pragma once
#include "global.h"

struct t_view {
	int scroll_x = 0;
	int scroll_y = 0;

	void setscrl(int x, int y)
	{
		scroll_x = x;
		scroll_y = y;
	}
	void scroll(int dx, int dy)
	{
		scroll_x += dx;
		scroll_y += dy;
	}
};
