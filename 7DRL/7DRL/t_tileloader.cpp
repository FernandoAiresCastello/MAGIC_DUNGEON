#include "t_tileloader.h"

void t_tileloader::load_all()
{
	tgl.tile_file("solid", "data/solid.bmp");
	tgl.tile_add("solid", "solid");
}
