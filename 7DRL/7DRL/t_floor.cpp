#include "t_floor.h"

t_floor::t_floor()
{
	oob.obj = t_object::none;
	oob.entity = t_object::none;
}
t_location& t_floor::get(int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height) {
		return locations[y][x];
	} else {
		return oob;
	}
}
void t_floor::set_obj(t_object o, int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height) {
		locations[y][x].obj = o;
		locations[y][x].graphical_variant = tgl.rnd(0, 2);
	}
}
void t_floor::set_entity(t_object e, int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height) {
		locations[y][x].entity = e;
	}
}
void t_floor::detonate_wall_or_trap(int x, int y)
{
	t_object& obj = get(x, y).obj;
	if (obj == t_object::wall || obj == t_object::trap) {
		set_obj(t_object::rubble, x, y);
	}
}
void t_floor::visit(int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height) {
		if (!locations[y][x].visited) {
			locations[y][x].visited = true;
			visited_count++;
			calc_percent_visited();
		}
	}
}
void t_floor::visit_all()
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			visit(x, y);
		}
	}
}
void t_floor::unvisit(int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height) {
		if (locations[y][x].visited) {
			locations[y][x].visited = false;
			visited_count--;
			calc_percent_visited();
		}
	}
}
bool t_floor::visited(int x, int y)
{
	return locations[y][x].visited;
}
void t_floor::color(int fg, int bg)
{
	forecolor = fg;
	backcolor = bg;
}
void t_floor::calc_percent_visited()
{
	percent_visited = (visited_count * 100) / total_locations;
}
