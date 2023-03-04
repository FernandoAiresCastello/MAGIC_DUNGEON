#include "t_tile_factory.h"

void t_tile_factory::make_tiles(TGL& tgl)
{
	tgl.tile_file("sword_n", "data/sword_n.bmp");
	tgl.tile_add("sword_n", "sword_n");
	tgl.tile_file("sword_s", "data/sword_s.bmp");
	tgl.tile_add("sword_s", "sword_s");
	tgl.tile_file("sword_e", "data/sword_e.bmp");
	tgl.tile_add("sword_e", "sword_e");
	tgl.tile_file("sword_w", "data/sword_w.bmp");
	tgl.tile_add("sword_w", "sword_w");
}
