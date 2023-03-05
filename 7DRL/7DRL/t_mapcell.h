#pragma once
#include "global.h"

enum class t_map_terrain
{
	unknown, grass, forest
};

struct t_mapcell
{
	t_map_terrain type = t_map_terrain::grass;

	bool visited = false;
	string title = "Undefined";

	void draw(int x, int y);
};
