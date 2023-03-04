#include "t_pos.h"

void t_pos::set(int x, int y)
{
	this->x = x;
	this->y = y;
}
void t_pos::move_dist(int dx, int dy)
{
	x += dx;
	y += dy;
}
