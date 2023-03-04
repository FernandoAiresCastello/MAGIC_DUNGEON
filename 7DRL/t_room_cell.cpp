#include "t_room_cell.h"

void t_room_cell::draw(TGL& tgl, int x, int y)
{
	tgl.color_normal(0x202020, 0x303030, 0, 0);
	tgl.draw_tiled("floor_1", x, y);
}
