#pragma once
#include "global.h"
#include "t_location.h"
#include "t_exit.h"

struct t_floor {
	static const int width = 100;
	static const int height = 100;
	rgb forecolor = 0xffffff;
	rgb backcolor = 0x000000;
	t_exit floor_exit;
	int percent_visited = 0;

	t_floor();

	t_location& get(int x, int y);
	void set_obj(t_object o, int x, int y);
	void set_entity(t_object e, int x, int y);
	void detonate_wall_or_trap(int x, int y);
	void visit(int x, int y);
	void visit_all();
	void unvisit(int x, int y);
	bool visited(int x, int y);
	void color(int fg, int bg);

private:
	static const int total_locations = width * height;
	t_location locations[height][width];
	t_location oob = t_location();
	int visited_count = 0;

	void calc_percent_visited();
};
