#pragma once
#include "t_game.h"
#include "t_enemy.h"

struct t_screen;
struct t_view;
struct t_floor;
struct t_player;

struct t_game {

	t_game();
	~t_game();

	void run();
	void enemy_turn();
	void confirm_goto_next_floor();

private:
	t_screen* screen = nullptr;
	t_view* view = nullptr;
	t_floor* cur_floor = nullptr;
	t_player* player = nullptr;
	vector<t_enemy> enemies;

	void init();
	int random_x();
	int random_y();
	t_enemy generate_enemy();
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
	void clear_top_text();
	void clear_bottom_text();
	void draw_info();
	void sync_player();
	void visit_surroundings();
	void init_sounds();
	void draw_floor_intro();
	void goto_next_floor();
	void tick_bomb_timers();
	void move_enemies();
	void game_loop();
};
