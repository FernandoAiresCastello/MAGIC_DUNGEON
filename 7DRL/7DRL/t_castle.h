#pragma once
#include "global.h"

class t_room;

class t_castle
{
public:
	t_castle();
	~t_castle();

	t_room* create_empty_room();

private:
	vector<t_room*> rooms;
};
