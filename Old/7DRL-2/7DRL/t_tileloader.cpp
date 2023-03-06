#include "t_tileloader.h"

void t_tileloader::load_all()
{
	tgl.transparency_key(0xffffff);

	load("player", "player_up_1");
	load("player", "player_up_2");

	load_single("unvisited_1");
	load_single("unvisited_2");
	load_single("empty_1");
	load_single("lonestar_1");
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
