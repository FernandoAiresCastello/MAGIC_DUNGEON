#include "t_tileloader.h"

void t_tileloader::load_all()
{
	tgl.transparency_key(0xffffff);

	load("player", "player_1");
	load("player", "player_2");

	load_single("unknown");
	load_single("grass_1");
	load_single("forest_1");
	load_single("heart_1");
	load_single("magic_1");
	load_single("star_1");
}
void t_tileloader::load(string id, string filename)
{
	tgl.tile_file(filename, "tiles/" + filename + ".bmp");
	tgl.tile_add(id, filename);
}
void t_tileloader::load_single(string id)
{
	tgl.tile_file(id, "tiles/" + id + ".bmp");
	tgl.tile_add(id, id);
}
