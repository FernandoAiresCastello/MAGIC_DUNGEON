#include "t_mapcell.h"

void t_mapcell::draw(int x, int y)
{
	if (!visited) {
		tgl.draw_tiled("empty_1", x, y);
		return;
	}
	if (type == t_mapcell_type::empty) {
		tgl.draw_tiled("empty_1", x, y);
		return;
	}
	if (type == t_mapcell_type::lonestar) {
		tgl.draw_tiled("lonestar_1", x, y);
		return;
	}
}
void t_mapcell::set_equal(t_mapcell& other)
{
	this->visited = other.visited;
	this->text = other.text;
}
