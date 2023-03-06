#include "t_tile_loader.h"

void t_tile_loader::load_all()
{
	load_single("loc_unknown");
	load_single("loc_empty");
	load_single("loc_wall_1");
}
void t_tile_loader::load_single(string id)
{
	tgl.tile_file(id, "tiles/" + id + ".bmp");
	tgl.tile_add(id, id);
}
