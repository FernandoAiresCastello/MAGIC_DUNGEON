#pragma once
#include "global.h"

class t_tile_loader
{
public:
	void load_all();

private:
	void load_single(string id);
};
