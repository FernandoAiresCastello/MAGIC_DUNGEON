#include "t_room.h"

void t_room::init_tiles(TGL& tgl)
{
	tgl.tile_file("floor_1", "data/floor_1.bmp");
	tgl.tile_add("floor_1", "floor_1");
	tgl.tile_file("brick_1", "data/brick_1.bmp");
	tgl.tile_add("brick_1", "brick_1");
}
void t_room::draw(TGL& tgl)
{
	int x = 0;
	int y = 0;
	tgl.color_normal(0x202020, 0x303030, 0, 0);
	for (int py = 0; py < height; py++) {
		for (int px = 0; px < width; px++) {
			tgl.draw_tiled("floor_1", x + px, y + py);
		}
	}
}
