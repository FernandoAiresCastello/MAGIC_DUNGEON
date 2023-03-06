#pragma once
#include "global.h"

class t_room;

enum class t_loctype
{
	empty, solid
};

class t_location
{
public:
	t_loctype type = t_loctype::empty;
	bool visited = true;

	t_location(t_room* room);
	~t_location();

private:
	t_room* room = nullptr;
};
