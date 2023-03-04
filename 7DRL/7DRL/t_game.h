#pragma once
#include "global.h"
#include "t_screen.h"
#include "t_map.h"

struct t_game
{
	void run();

	t_screen scr;
	t_map map;
};
