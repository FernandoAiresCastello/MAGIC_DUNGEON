#include "t_room.h"

void t_room::init_tiles(TGL& tgl)
{
	tgl.tile_file("floor_1", "data/floor_1.bmp");
	tgl.tile_add("floor_1", "floor_1");
	tgl.tile_file("brick_1", "data/brick_1.bmp");
	tgl.tile_add("brick_1", "brick_1");
	tgl.tile_file("solid_1", "data/solid_1.bmp");
	tgl.tile_add("solid_1", "solid_1");
}
void t_room::draw(TGL& tgl)
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			cells[y][x].draw(tgl, x, y);
		}
	}
}
