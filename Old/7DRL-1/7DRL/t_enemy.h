#pragma once
#include "global.h"
#include "t_entity.h"

class t_enemy : public t_entity
{
public:
	t_enemy();
	~t_enemy();

	static void init_tiles(TGL& tgl);

	void draw(TGL& tgl);
};
