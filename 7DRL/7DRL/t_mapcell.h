#pragma once
#include "global.h"

struct t_mapcell
{
	bool visited = false;
	string title = "Undefined";

	void draw(int x, int y);
};
