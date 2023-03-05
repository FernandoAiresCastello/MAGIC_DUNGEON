#pragma once
#include "global.h"
#include "t_entity.h"

struct t_player : public t_entity
{
	int stars = 0;

	t_player();

	void visit();
};
