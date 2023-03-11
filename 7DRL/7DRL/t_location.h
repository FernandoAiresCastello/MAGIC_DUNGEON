#pragma once
#include "global.h"
#include "t_object.h"

struct t_location {
	bool visited = false;
	t_object obj = t_object::ground;
	t_object entity = t_object::none;
	int graphical_variant = 0;
};
