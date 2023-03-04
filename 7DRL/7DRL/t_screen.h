#pragma once
#include "global.h"
#include "t_map.h"
#include "t_pos.h"

struct t_screen
{
	void init();
	void draw_background();
	void draw_map(t_map& map);
	void draw_title(string str);

	rgb back_color;
	t_pos map_pos;

	struct {
		t_pos pos;
		rgb back_color;
		rgb fore_color;
	} title;
};
