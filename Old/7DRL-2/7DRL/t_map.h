#pragma once
#include "global.h"
#include "t_mapcell.h"
#include "t_pos.h"

struct t_map
{
	static const int width = 32;
	static const int height = 18;

	void draw(t_pos& pos);
	t_mapcell& get_cell(t_pos pos);
	void set_cell(int x, int y, t_mapcell cell);

private:
	t_mapcell cells[height][width];
};
