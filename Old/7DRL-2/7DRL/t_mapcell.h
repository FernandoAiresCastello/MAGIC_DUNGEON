#pragma once
#include "global.h"

enum class t_mapcell_type
{
	empty, lonestar
};

struct t_mapcell
{
	t_mapcell_type type = t_mapcell_type::empty;
	string text = "Undefined";
	bool visited = false;

	void draw(int x, int y);
	void set_equal(t_mapcell& other);
};
