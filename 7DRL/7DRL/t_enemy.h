#pragma once
#include "global.h"

struct t_enemy {
	string tile;
	int x = 0;
	int y = 0;
	int life = 0;
	int attack = 0;
	const int exp = 1;
};
