#pragma once
#include "global.h"
#include "t_dir.h"
#include "t_pos.h"

class t_player
{
public:

	static void init_tiles(TGL& tgl);

	void set_pos(int x, int y);
	void draw(TGL& tgl);
	void on_keypress(int key);

private:

	t_pos pos;
	t_pos prev_pos;
	bool can_move = true;
	bool moved = false;
	t_dir facing_dir = t_dir::none;

	void move_dist(int dx, int dy);
};
