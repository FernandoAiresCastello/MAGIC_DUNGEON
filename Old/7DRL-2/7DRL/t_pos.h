#pragma once
#include "global.h"

struct t_pos
{
	int x = 0;
	int y = 0;

	t_pos()
	{
		set(0, 0);
	}

	t_pos(int x, int y)
	{
		set(x, y);
	}

	void set(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	void move_dist(int dx, int dy)
	{
		x += dx;
		y += dy;
	}

	t_pos offset(int dx, int dy)
	{
		return t_pos(x + dx, y + dy);
	}
};
