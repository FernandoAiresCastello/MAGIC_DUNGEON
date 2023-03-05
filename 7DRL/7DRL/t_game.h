#pragma once
#include "global.h"
#include "t_screen.h"
#include "t_map.h"
#include "t_player.h"

struct t_game
{
	void run();

	t_screen scr;
	t_map map;
	t_player player;
};
