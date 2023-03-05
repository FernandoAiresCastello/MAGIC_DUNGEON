#pragma once
#include "global.h"
#include "t_pos.h"
#include "t_map.h"

struct t_entity
{
	t_pos pos;
	string tile;
	t_map* map = nullptr;

	t_entity();

	void draw(int map_offset_x, int map_offset_y);
	void center_map();
};
