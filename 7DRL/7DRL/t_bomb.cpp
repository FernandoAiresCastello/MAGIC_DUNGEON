#include "t_bomb.h"
#include "t_player.h"
#include "t_floor.h"
#include "t_object.h"

t_bomb::t_bomb(t_player* player, t_floor* cur_floor)
{
	this->player = player;
	this->cur_floor = cur_floor;
}
int t_bomb::get_x()
{
	return x;
}
int t_bomb::get_y()
{
	return y;
}
bool t_bomb::is_active()
{
	return active;
}
void t_bomb::activate(int bx, int by)
{
	if (!active) {
		x = bx;
		y = by;
		timer = 0;
		active = true;
		tgl.play_notes("l30o4cde");
	}
}
void t_bomb::tick()
{
	if (active) {
		timer++;
		if (timer >= timer_max) {
			detonate();
		}
	}
}
void t_bomb::detonate()
{
	timer = 0;
	active = false;
	tgl.sound("explosion");

	cur_floor->detonate_wall(x - 1, y - 1);
	cur_floor->detonate_wall(x + 0, y - 1);
	cur_floor->detonate_wall(x + 1, y - 1);
	cur_floor->detonate_wall(x - 1, y);
	cur_floor->detonate_wall(x, y);
	cur_floor->detonate_wall(x + 1, y);
	cur_floor->detonate_wall(x - 1, y + 1);
	cur_floor->detonate_wall(x + 0, y + 1);
	cur_floor->detonate_wall(x + 1, y + 1);

	int px = player->get_x();
	int py = player->get_y();

	bool player_detected =
		(x - 1 == px && y - 1 == py) ||
		(x + 0 == px && y - 1 == py) ||
		(x + 1 == px && y - 1 == py) ||
		(x - 1 == px && y == py) ||
		(x == px && y == py) ||
		(x + 1 == px && y == py) ||
		(x - 1 == px && y + 1 == py) ||
		(x + 0 == px && y + 1 == py) ||
		(x + 1 == px && y + 1 == py);

	if (player_detected) {
		player->hurt_by_bomb();
	}
}
int t_bomb::get_damage()
{
	return tgl.rnd(40, 60);
}
