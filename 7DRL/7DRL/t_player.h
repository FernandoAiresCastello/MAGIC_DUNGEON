#pragma once
#include "global.h"
#include "t_direction.h"
#include "t_location.h"
#include "t_bomb.h"
#include "t_shop.h"

struct t_game;
struct t_floor;
struct t_screen;
struct t_enemy;

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
	int get_exp();
	int get_max_exp();
	int get_exp_level();
	void set_pos(int newx, int newy);
	void move(int dx, int dy);
	bool can_move_to(int newx, int newy);
	void step_back();
	bool is_oob(int newx, int newy);
	t_location& getloc();
	bool is_on_stairs();
	bool found_coin();
	bool found_shop(t_shoptype type);
	void grab_coin(int count = 1);
	void drop_bomb();
	void destroy_walls_around();
	void next_floor();
	void trigger_collisions();
	void hurt_by_bomb();
	void hurt_by_enemy(t_enemy* enemy);
	void attack_enemy(t_enemy* enemy);
	bool spend_coins(int count);
	void restore_life(int points);
	void restore_bombs(int count);
	void obtain_map();
	void gain_exp(int points);

private:
	t_game* game = nullptr;
	t_floor* cur_floor = nullptr;
	t_screen* screen = nullptr;
	t_direction dir = t_direction::none;
	int x = 0;
	int y = 0;
	int prevx = x;
	int prevy = y;
	int floor_nr = 0;
	const int max_life = 999;
	int life = 100;
	const int max_bombs = 99;
	int bombs = 10;
	int coins = 0;
	int power = 1;
	int exp = 0;
	const int max_exp = 100;
	int exp_level = 1;

	void receive_damage(int damage);
};
