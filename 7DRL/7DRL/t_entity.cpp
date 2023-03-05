#include "t_entity.h"

t_entity::t_entity()
{
	pos.set(0, 0);
}
void t_entity::draw(int map_offset_x, int map_offset_y)
{
	if (tile == "") return;

	tgl.draw_tiled(tile, map_offset_x + pos.x, map_offset_y + pos.y);
}
void t_entity::center_map()
{
	pos.set(t_map::width / 2, t_map::height / 2);
}
