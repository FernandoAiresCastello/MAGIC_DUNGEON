#pragma once
#include "global.h"
#include "t_rect.h"

class t_room
{
public:
	const int width = 32;
	const int height = 18;

	static void init_tiles(TGL& tgl);

	void draw(TGL& tgl);

private:

};
