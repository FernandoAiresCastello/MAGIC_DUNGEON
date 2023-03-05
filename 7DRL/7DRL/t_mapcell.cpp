#include "t_mapcell.h"

void t_mapcell::draw(int x, int y)
{
	if (!visited) {
		tgl.draw_tiled("unknown", x, y);
		return;
	}
	if (type == t_map_terrain::grass) {
		tgl.draw_tiled("grass_1", x, y);
		return;
	}
	if (type == t_map_terrain::forest) {
		tgl.draw_tiled("forest_1", x, y);
		return;
	}
}
