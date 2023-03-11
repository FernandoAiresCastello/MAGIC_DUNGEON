#pragma once
#include "global.h"
#include "t_direction.h"
#include "t_location.h"
#include "t_bomb.h"

struct t_game;
struct t_floor;
struct t_screen;

struct t_player {
	t_bomb bomb;

	t_player(t_game* game, t_floor* cur_floor, t_screen* screen);

	int get_floor();
	int get_x();
	int get_y();
	int get_prevx();
	int get_prevy();
	int get_life();
	int get_coins();
	int get_bombs();
	void set_pos(int newx, int newy);
	void move(int dx, int dy);
	bool can_move_to(int newx, int newy);
	bool is_oob(int newx, int newy);
	t_location& getloc();
	bool is_on_stairs();
	bool found_coin();
	void grab_coin();
	void drop_bomb();
	void destroy_walls_around();
	void next_floor();
	void hurt_by_bomb();
	void trigger_collisions();

private:
	t_game* game = nullptr;
	t_floor* cur_floor = nullptr;
	t_screen* screen = nullptr;
	t_direction dir = t_direction::none;
	int x = 0;
	int y = 0;
	int prevx = x;
	int prevy = y;
	int life = 100;
	int coins = 0;
	int bombs = 10;
	int floor_nr = 0;
};
