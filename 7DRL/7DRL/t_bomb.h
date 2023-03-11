#pragma once
#include "global.h"

struct t_player;
struct t_floor;

struct t_bomb {

	t_bomb(t_player* player, t_floor* cur_floor);

	int get_x();
	int get_y();
	bool is_active();
	void activate(int bx, int by);
	void tick();
	void detonate();

private:
	int x = -1;
	int y = -1;
	bool active = false;
	int timer = 0;
	const int timer_max = 200;
	t_player* player = nullptr;
	t_floor* cur_floor = nullptr;
};
