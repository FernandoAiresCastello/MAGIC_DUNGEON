#pragma once
#include "global.h"
#include "t_entity.h"
#include "t_dir.h"

class t_player : public t_entity
{
public:
	t_player();
	~t_player();

	static void init_tiles(TGL& tgl);

	void draw(TGL& tgl);
	void on_keypress(int key);

private:
	t_pos prev_pos;
	bool can_move = true;
	bool moved = false;
	t_dir facing_dir = t_dir::none;

	void move_dist(int dx, int dy);
};
