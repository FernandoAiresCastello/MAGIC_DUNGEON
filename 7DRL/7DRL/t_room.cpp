#include "t_room.h"
#include "t_castle.h"
#include "t_location.h"

t_room::t_room(t_castle* castle) : loc()
{
	this->castle = castle;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			loc[y][x] = new t_location(this);
		}
	}
}
t_room::~t_room()
{
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			delete loc[y][x];
			loc[y][x] = nullptr;
		}
	}
}
t_location* t_room::get(int x, int y)
{
	if (x >= 0 && y >= 0 && x < width && y < height)
		return loc[y][x];
	else
		return nullptr;
}
