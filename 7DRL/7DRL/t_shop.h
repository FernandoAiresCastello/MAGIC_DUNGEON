#pragma once
#include "global.h"

struct t_screen;
struct t_player;
struct t_floor;

enum class t_shoptype {
	undefined, life, bomb, map
};

struct t_shop {
	t_shoptype type = t_shoptype::undefined;
	int x = -1;
	int y = -1;

	void spawn(t_shoptype type, t_floor* cur_floor, int x, int y);
	void sell(t_player* player, t_screen* screen);
};
