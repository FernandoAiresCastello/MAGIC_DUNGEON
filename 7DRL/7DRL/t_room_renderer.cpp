#include "t_room_renderer.h"
#include "t_room.h"
#include "t_location.h"

void t_room_renderer::draw(t_room* room)
{
	for (int y = 0; y < room->height; y++) {
		for (int x = 0; x < room->width; x++) {
			draw_location(room->get(x, y), x, y);
		}
	}
}
void t_room_renderer::draw_location(t_location* loc, int x, int y)
{
	if (!loc->visited) {
		tgl.draw_tiled("loc_unknown", x, y);
		return;
	}

	string tile = "";

	if (loc->type == t_loctype::empty) {
		tile = "loc_empty";
	} else if (loc->type == t_loctype::solid) {
		tile = "loc_wall_1";
	} else {
		tile = "loc_unknown";
	}

	tgl.draw_tiled(tile, x, y);
}
