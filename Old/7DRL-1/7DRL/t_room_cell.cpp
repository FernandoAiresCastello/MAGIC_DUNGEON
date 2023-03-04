#include "t_room_cell.h"

void t_room_cell::draw(TGL& tgl, int x, int y)
{
	if (type == t_celltype::empty) {
		tgl.color_single(0x000080);
		tgl.draw_tiled("solid_1", x, y);
	}
}
