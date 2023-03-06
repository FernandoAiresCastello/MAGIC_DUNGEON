#pragma once
#include "global.h"
#include "t_room_renderer.h"

class t_castle;
class t_room;

class t_game
{
public:
	t_game();
	~t_game();

	void run();

private:
	t_castle* castle = nullptr;
	t_room* cur_room = nullptr;
	t_room_renderer rend_room;

	void game_loop();
};
