#pragma once
#include "t_game.h"
#include "t_enemy.h"
#include "t_shop.h"

struct t_screen;
struct t_view;
struct t_floor;
struct t_player;

struct t_game {
	vector<t_enemy> enemies;
	t_shop life_shop;
	t_shop bomb_shop;
	t_shop map_shop;

	t_game();
	~t_game();

	void run();
	void enemy_turn();
	t_enemy* enemy_at(int x, int y);
	t_enemy* get_random_enemy_around_player();
	void confirm_goto_next_floor();

private:
	t_screen* screen = nullptr;
	t_view* view = nullptr;
	t_floor* cur_floor = nullptr;
	t_player* player = nullptr;

	void init();
	int random_x();
	int random_y();
	t_enemy generate_enemy();
	void generate_shops();
	void generate_enemies();
	void generate_exit(int x, int y);
	void generate_room(int x, int y, int w, int h);
	void generate_wall(int x, int y, int length, int orient);
	void randomize_color_scheme();
	void init_current_floor();
	bool is_enemy_within_view(t_enemy& e);
	void draw_enemies();
	void draw_location(int mapx, int mapy, int scrx, int scry);
	void draw_current_floor();
	void draw_info();
	void sync_player();
	void visit_surroundings();
	void load_sounds();
	void draw_floor_intro();
	void goto_next_floor();
	void tick_bomb_timers();
	void move_enemies();
	void game_loop();
	void process_input();
};
