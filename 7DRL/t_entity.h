#pragma once
#include "t_pos.h"

class t_entity
{
public:
	t_entity() {}
	virtual ~t_entity() {}

	int x() { return pos.x; }
	int y() { return pos.y; }

	void set_pos(int x, int y);

protected:
	t_pos pos;
};
