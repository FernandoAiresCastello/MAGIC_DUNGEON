#pragma once
#include "global.h"

class t_room;
class t_location;

class t_room_renderer
{
public:
	void draw(t_room* room);

private:
	void draw_location(t_location* loc, int x, int y);
};
