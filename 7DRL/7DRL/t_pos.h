#pragma once
#include "global.h"

struct t_pos
{
	int x = 0;
	int y = 0;

	void set(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
};
