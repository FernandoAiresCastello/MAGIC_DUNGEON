#pragma once
#include "global.h"

struct t_enemy {
	int x = 0;
	int y = 0;
	string tile = "spider";
	int life = tgl.rnd(3, 5);
	int attack = tgl.rnd(1, 2);
	int exp = 1;
};
