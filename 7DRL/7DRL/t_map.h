#pragma once
#include "global.h"
#include "t_mapcell.h"
#include "t_pos.h"

struct t_map
{
	static const int width = 24;
	static const int height = 16;

	t_mapcell cells[height][width];

	void draw(t_pos& pos);
};
