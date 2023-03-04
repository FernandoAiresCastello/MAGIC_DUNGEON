#pragma once
#include "global.h"
#include "t_player.h"
#include "t_room.h"
#include "t_infobox.h"

class t_game
{
public:

	t_game();
	~t_game();

	void run();

private:

	TGL tgl;
	t_player player;
	t_room room;
	t_infobox infobox;
};
