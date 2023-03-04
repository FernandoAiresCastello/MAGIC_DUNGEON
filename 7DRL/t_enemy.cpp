#include "t_enemy.h"

t_enemy::t_enemy()
{
}
t_enemy::~t_enemy()
{
}
void t_enemy::init_tiles(TGL& tgl)
{
	tgl.tile_file("slime_1", "data/slime_1.bmp");
	tgl.tile_add("slime_1", "slime_1");
}
void t_enemy::draw(TGL& tgl)
{
	tgl.color_sprite(0x008000, 0x00a000, 0x00ff00);
	tgl.draw_tiled("slime_1", pos.x, pos.y);
}
