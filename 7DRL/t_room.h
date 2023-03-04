#pragma once
#include "global.h"
#include "t_room_cell.h"

class t_room
{
public:
	static const int width = 32;
	static const int height = 18;

	static void init_tiles(TGL& tgl);

	void draw(TGL& tgl);

private:
	t_room_cell cells[height][width];
};
