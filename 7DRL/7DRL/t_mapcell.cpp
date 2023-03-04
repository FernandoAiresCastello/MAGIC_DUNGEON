#include "t_mapcell.h"

void t_mapcell::draw(int x, int y)
{
	tgl.color_normal(0x000000, 0x000000, 0x000000, 0x000000);
	tgl.draw_tiled("solid", x, y);
}
