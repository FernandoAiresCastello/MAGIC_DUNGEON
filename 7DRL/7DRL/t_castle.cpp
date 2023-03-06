#include "t_castle.h"
#include "t_room.h"

t_castle::t_castle()
{
}
t_castle::~t_castle()
{
	for (auto& room : rooms) {
		delete room;
		room = nullptr;
	}
	rooms.clear();
}
t_room* t_castle::create_empty_room()
{
	t_room* room = new t_room(this);
	rooms.push_back(room);
	return room;
}
