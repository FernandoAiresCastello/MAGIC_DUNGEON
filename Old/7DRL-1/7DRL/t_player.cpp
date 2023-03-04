#include "t_player.h"

t_player::t_player()
{
}
t_player::~t_player()
{
}
void t_player::init_tiles(TGL& tgl)
{
	tgl.tile_file("wiz_1", "data/wiz_1.bmp");
	tgl.tile_file("wiz_2", "data/wiz_2.bmp");
	tgl.tile_add("wiz", "wiz_1", 5);
	tgl.tile_add("wiz", "wiz_2");
}
void t_player::draw(TGL& tgl)
{
	tgl.color_sprite(0xffff80, 0x000000, 0x8000ff);
	tgl.draw_tiled("wiz", pos.x, pos.y);
}
void t_player::on_keypress(int key)
{
	if (can_move) {
		moved = false;
		if (key == SDLK_RIGHT) {
			move_dist(1, 0);
		} else if (key == SDLK_LEFT) {
			move_dist(-1, 0);
		} else if (key == SDLK_DOWN) {
			move_dist(0, 1);
		} else if (key == SDLK_UP) {
			move_dist(0, -1);
		}
	}
}
void t_player::move_dist(int dx, int dy)
{
	if (dx == 0 && dy == 0) {
		moved = false;
	}
	else if (dy == 0) {
		if (dx > 0) facing_dir = t_dir::east;
		else if (dx < 0) facing_dir = t_dir::west;
		moved = true;
	}
	else if (dx == 0) {
		if (dy > 0) facing_dir = t_dir::south;
		else if (dy < 0) facing_dir = t_dir::north;
		moved = true;
	}
	else {
		moved = false;
	}

	if (moved) {
		pos.move_dist(dx, dy);
		prev_pos = pos;
	}
}
