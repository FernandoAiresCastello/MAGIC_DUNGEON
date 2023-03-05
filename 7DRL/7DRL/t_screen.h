#pragma once
#include "global.h"
#include "t_map.h"
#include "t_pos.h"
#include "t_player.h"

struct t_screen
{
	void init();
	void draw_background();
	void draw_map(t_map& map);
	void draw_player(t_player& player);
	void print_title(string str);
	void println(string str);

private:
	rgb back_color;
	t_pos map_pos;

	struct {
		t_pos pos;
		rgb back_color;
		rgb fore_color;
	} title;

	struct {
		t_pos pos;
		rgb color;
	} text;

	int print_line = 0;
	int max_lines = 4;
};
