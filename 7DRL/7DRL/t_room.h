#pragma once
#include "global.h"

class t_castle;
class t_location;

class t_room
{
public:
	static const int width = 32;
	static const int height = 24;

	t_room(t_castle* castle);
	~t_room();

	t_location* get(int x, int y);

private:
	t_castle* castle = nullptr;
	t_location* loc[height][width];
};
