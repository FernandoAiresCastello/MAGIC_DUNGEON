#include "t_location.h"
#include "t_room.h"

t_location::t_location(t_room* room)
{
	this->room = room;
}
t_location::~t_location()
{
}
