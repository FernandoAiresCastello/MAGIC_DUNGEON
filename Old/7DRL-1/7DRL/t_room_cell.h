#pragma once
#include "global.h"

enum class t_celltype
{
	empty, ground
};

class t_room_cell
{
public:
	t_celltype type = t_celltype::empty;

	void draw(TGL& tgl, int x, int y);
};
