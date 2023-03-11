#include "t_game.h"
#include "t_screen.h"
#include "t_view.h"
#include "t_floor.h"
#include "t_player.h"
#include "t_tileset.h"

t_game::t_game()
{
	init();
}
t_game::~t_game()
{
	delete screen;		screen = nullptr;
	delete cur_floor;	cur_floor = nullptr;
	delete player;		player = nullptr;
}
void t_game::init()
{
	screen = new t_screen();
	view = &screen->view;
	cur_floor = new t_floor();
	player = new t_player(this, cur_floor, screen);

	tgl.window_gbc(0x000000, 5);
	tgl.mouse(false);
	screen->cols = tgl.cols();
	screen->rows = tgl.rows();

	t_tileset::init();
	load_sounds();

	goto_next_floor();
}
void t_game::run()
{
	while (tgl.running()) {
		game_loop();
	}
}
void t_game::enemy_turn()
{
	move_enemies();

	bool chance = tgl.rnd(0, 100) > 80;
	if (chance) {
		t_enemy* enemy = get_random_enemy_around_player();
		if (enemy) {
			player->hurt_by_enemy(enemy);
		}
	}
}
t_enemy* t_game::enemy_at(int x, int y)
{
	for (auto& enemy : enemies) {
		if (enemy.x == x && enemy.y == y && enemy.life > 0) {
			return &enemy;
		}
	}
	return nullptr;
}
t_enemy* t_game::get_random_enemy_around_player()
{
	int x = player->get_x();
	int y = player->get_y();

	t_enemy* enemies[4] = {
		enemy_at(x, y - 1),
		enemy_at(x, y + 1),
		enemy_at(x - 1, y),
		enemy_at(x + 1, y)
	};

	for (int i = 0; i < 4; i++) {
		if (enemies[i] != nullptr) {
			return enemies[i];
		}
	}
	return nullptr;
}
int t_game::random_x()
{
	return tgl.rnd(1, cur_floor->width - 2);
}
int t_game::random_y()
{
	return tgl.rnd(1, cur_floor->height - 2);
}
void t_game::generate_shops()
{
	life_shop.spawn(t_shoptype::life, cur_floor, random_x(), random_y());
	bomb_shop.spawn(t_shoptype::bomb, cur_floor, random_x(), random_y());
	map_shop.spawn(t_shoptype::map, cur_floor, random_x(), random_y());
}
t_enemy t_game::generate_enemy()
{
	t_enemy e;

	e.tile = "spider";
	e.x = random_x();
	e.y = random_y();
	e.life = player->get_floor() + tgl.rnd(0, 5);
	e.attack = player->get_floor() + tgl.rnd(0, 3);

	return e;
}
void t_game::generate_enemies()
{
	int max_enemies = 100 + ((tgl.rnd(2, 5)) * player->get_floor());
	if (max_enemies > 1000) {
		max_enemies = 1000;
	}
	enemies.clear();

	for (int i = 0; i < max_enemies; i++) {
		bool regen = false;
		do {
			t_enemy e = generate_enemy();
			if (cur_floor->get(e.x, e.y).obj == t_object::ground) {
				regen = false;
				enemies.push_back(e);
			} else {
				regen = true;
			}
		}
		while (regen);
	}
}
void t_game::generate_exit(int x, int y)
{
	cur_floor->floor_exit.x = x;
	cur_floor->floor_exit.y = y;
	cur_floor->set_obj(t_object::stairs, x, y);
}
void t_game::generate_room(int x, int y, int w, int h)
{
	// walls
	for (int px = x; px <= x + w; px++) {
		cur_floor->set_obj(t_object::wall, px, y);
		cur_floor->set_obj(t_object::wall, px, y + h);
	}
	for (int py = y; py <= y + h; py++) {
		cur_floor->set_obj(t_object::wall, x, py);
		cur_floor->set_obj(t_object::wall, x + w, py);
	}
	// coins
	for (int py = y + 1; py <= y + h - 1; py++) {
		for (int px = x + 1; px <= x + w - 1; px++) {
			bool chance = tgl.rnd(0, 100) > 50;
			if (chance) {
				cur_floor->set_obj(t_object::coin, px, py);
			}
		}
	}

	// room entrance & room exit
	int exit_x = 0;
	int exit_y = 0;
	int exit_placement = tgl.rnd(0, 3);
	if (exit_placement == 0) { // on top corner
		exit_y = y;
		exit_x = tgl.rnd(x + 1, x + w - 1);
	} else if (exit_placement == 1) { // on bottom corner
		exit_y = y + h;
		exit_x = tgl.rnd(x + 1, x + w - 1);
	} else if (exit_placement == 2) { // on left corner
		exit_x = x;
		exit_y = tgl.rnd(y + 1, y + h - 1);
	} else if (exit_placement == 3) { // on right corner
		exit_x = x + w;
		exit_y = tgl.rnd(y + 1, y + h - 1);
	}
	cur_floor->set_obj(t_object::ground, exit_x, exit_y);
}
void t_game::generate_wall(int x, int y, int length, int orient)
{
	int px = x;
	int py = y;
	for (int i = 0; i <= length; i++) {
		if (cur_floor->get(px, py).entity == t_object::none) {
			cur_floor->set_obj(t_object::wall, px, py);
		}
		if (orient == 0) {
			px++;
		} else {
			py++;
		}
	}
}

void t_game::randomize_color_scheme()
{
	int color_preset = tgl.rnd(0, screen->color_scheme.presets.size() - 1);

	cur_floor->color(
		screen->color_scheme.presets[color_preset].first, 
		screen->color_scheme.presets[color_preset].second);
}
void t_game::init_current_floor()
{
	const int min_room_w = 3;
	const int min_room_h = 3;
	const int max_room_w = 20;
	const int max_room_h = 20;
	const int max_wall_len = 40;
	const int room_count = 50;
	const int random_wall_count = 40;
	const int exit_count = 5;

	// color scheme
	randomize_color_scheme();

	// base
	for (int y = 0; y < cur_floor->height; y++) {
		for (int x = 0; x < cur_floor->width; x++) {
			cur_floor->unvisit(x, y);
			cur_floor->set_obj(t_object::ground, x, y);
		}
	}
	// corners
	for (int x = 0; x < cur_floor->width; x++) {
		cur_floor->set_obj(t_object::wall, x, 0);
		cur_floor->set_obj(t_object::wall, x, cur_floor->height - 1);
	}
	for (int y = 0; y < cur_floor->height; y++) {
		cur_floor->set_obj(t_object::wall, 0, y);
		cur_floor->set_obj(t_object::wall, cur_floor->width - 1, y);
	}
	// random walls
	for (int i = 0; i < random_wall_count; i++) {
		int x = tgl.rnd(0, cur_floor->width);
		int y = tgl.rnd(0, cur_floor->height);
		int length = tgl.rnd(2, max_wall_len);
		int orient = tgl.rnd(0, 1);
		generate_wall(x, y, length, orient);
	}
	// rooms
	for (int i = 0; i < room_count; i++) {
		int x = tgl.rnd(0, cur_floor->width);
		int y = tgl.rnd(0, cur_floor->height);
		int w = tgl.rnd(min_room_w, max_room_w);
		int h = tgl.rnd(min_room_h, max_room_h);
		generate_room(x, y, w, h);
	}
	// exits
	for (int i = 0; i < exit_count; i++) {
		generate_exit(random_x(), random_y());
	}
	// shops
	generate_shops();
	// enemies
	generate_enemies();
}
bool t_game::is_enemy_within_view(t_enemy& e)
{
	return
		e.x >= view->scroll_x && e.y >= view->scroll_y &&
		e.x < view->scroll_x + screen->cols && e.y < view->scroll_y + screen->rows;
}
void t_game::draw_enemies()
{
	tgl.tile_transparent(false);
	tgl.color_binary(cur_floor->forecolor, cur_floor->backcolor);

	int scrx = 0;
	int scry = 0;

	for (int mapy = view->scroll_y; mapy < view->scroll_y + screen->rows; mapy++) {
		for (int mapx = view->scroll_x; mapx < view->scroll_x + screen->cols; mapx++) {
			if (cur_floor->visited(mapx, mapy)) {
				for (auto& e : enemies) {
					if (e.life > 0 && e.x == mapx && e.y == mapy) {
						tgl.draw_tiled("spider", scrx, scry);
					}
				}
			}
			scrx++;
		}
		scry++;
		scrx = 0;
	}
}
void t_game::draw_location(int mapx, int mapy, int scrx, int scry)
{
	if (mapx < 0 || mapy < 0 || mapx >= cur_floor->width || mapy >= cur_floor->height) {
		tgl.draw_tiled("empty", scrx, scry);
		return;
	}
	t_location& loc = cur_floor->get(mapx, mapy);
	if (!loc.visited) {
		tgl.draw_tiled("unvisited", scrx, scry);
		return;
	}

	if (loc.entity == t_object::player) {
		tgl.draw_tiled("player", scrx, scry);

	} else if (player->bomb.is_active() && player->bomb.get_x() == mapx && player->bomb.get_y() == mapy) {
		tgl.draw_tiled("bomb", scrx, scry);
	} else if (
		(bomb_shop.x == mapx && bomb_shop.y == mapy) ||
		(life_shop.x == mapx && life_shop.y == mapy) ||
		(map_shop.x == mapx && map_shop.y == mapy)) {
		tgl.draw_tiled("shop", scrx, scry);

	} else if (loc.obj == t_object::coin) {
		tgl.draw_tiled("collect", scrx, scry);

	} else if (loc.obj == t_object::ground) {
		if (loc.graphical_variant == 0) {
			tgl.draw_tiled("ground", scrx, scry);
		} else if (loc.graphical_variant == 1) {
			tgl.draw_tiled("ground_2", scrx, scry);
		} else if (loc.graphical_variant == 2) {
			tgl.draw_tiled("ground_3", scrx, scry);
		}

	} else if (loc.obj == t_object::wall) {
		if (loc.graphical_variant == 0) {
			tgl.draw_tiled("solid", scrx, scry);
		} else if (loc.graphical_variant == 1 || loc.graphical_variant == 2) {
			tgl.draw_tiled("wall", scrx, scry);
		}

	} else if (loc.obj == t_object::stairs) {
		tgl.draw_tiled("stairs", scrx, scry);
	}
}
void t_game::draw_current_floor()
{
	tgl.tile_transparent(true);
	tgl.backcolor(cur_floor->backcolor);
	tgl.color_binary(cur_floor->forecolor);

	int scrx = 0;
	int scry = 0;

	for (int mapy = view->scroll_y; mapy < view->scroll_y + screen->rows; mapy++) {
		for (int mapx = view->scroll_x; mapx < view->scroll_x + screen->cols; mapx++) {
			draw_location(mapx, mapy, scrx, scry);
			scrx++;
		}
		scry++;
		scrx = 0;
	}

	draw_enemies();
}
void t_game::draw_info()
{
	tgl.font_transparent(false);
	tgl.tile_transparent(false);
	tgl.font_color(cur_floor->forecolor, cur_floor->backcolor);
	tgl.color_binary(cur_floor->forecolor, cur_floor->backcolor);

	screen->clear_top_text();
	screen->clear_bottom_text();

	// top
	int y = 0;
	tgl.print_tiled(tgl.fmt("F%i E%iN%i     %3i%%",
		player->get_floor(), player->get_x(), player->get_y(), cur_floor->percent_visited), 1, y);
	// bottom 1
	y = screen->rows - 2;
	tgl.print_tiled(tgl.fmt("~%03i  %02i %8i",
		player->get_life(), player->get_bombs(), player->get_coins()), 1, y);
	tgl.draw_tiled("bomb", 6, y);
	tgl.draw_tiled("collect", 18, y);

	// bottom 2
	y = screen->rows - 1;
	tgl.print_tiled(tgl.fmt("lv:%i xp:%i/%i",
		player->get_exp_level(), player->get_exp(), player->get_max_exp()), 1, y);
}
void t_game::sync_player()
{
	cur_floor->set_entity(t_object::none, player->get_prevx(), player->get_prevy());
	cur_floor->set_entity(t_object::player, player->get_x(), player->get_y());
}
void t_game::visit_surroundings()
{
	int x = player->get_x();
	int y = player->get_y();

	cur_floor->visit(x - 1, y - 1);
	cur_floor->visit(x + 0, y - 1);
	cur_floor->visit(x + 1, y - 1);
	cur_floor->visit(x - 1, y);
	cur_floor->visit(x, y);
	cur_floor->visit(x + 1, y);
	cur_floor->visit(x - 1, y + 1);
	cur_floor->visit(x + 0, y + 1);
	cur_floor->visit(x + 1, y + 1);
}
void t_game::load_sounds()
{
	tgl.sound_load("explosion", "sound/bomb.wav");
	tgl.sound_load("coin", "sound/coin.wav");
	tgl.sound_load("slash", "sound/slash.wav");
	tgl.sound_load("enemy_attack", "sound/enemy_attack.wav");
	tgl.sound_load("enemy_killed", "sound/enemy_killed.wav");
	tgl.sound_load("shop", "sound/shop.wav");
	tgl.sound_load("level_up", "sound/level_up.wav");
	tgl.sound_load("exit", "sound/exit.wav");
}
void t_game::draw_floor_intro()
{
	const rgb forecolor = 0xffffff;
	const rgb backcolor = 0x101010;

	tgl.backcolor(backcolor);

	int timer = 100;
	while (tgl.running() && timer > 0) {
		tgl.clear();
		tgl.system();
		timer--;
	}

	tgl.play_notes("l40o5cdedefefgggg");

	timer = 200;
	while (tgl.running() && timer > 0) {
		tgl.clear();
		tgl.font_transparent(false);
		tgl.font_color(forecolor, backcolor);
		tgl.print_tiled(tgl.fmt("FLOOR %4i", player->get_floor()), 5, 8);
		tgl.system();
		timer--;
	}
}
void t_game::goto_next_floor()
{
	player->next_floor();

	int player_x = random_x();
	int player_y = random_y();
	player->set_pos(player_x, player_y);

	init_current_floor();
	cur_floor->set_obj(t_object::ground, player_x, player_y);
	player->destroy_walls_around();

	view->setscrl(player_x - screen->cols / 2 + 1, player_y - screen->rows / 2 + 1);

	draw_floor_intro();
}
void t_game::confirm_goto_next_floor()
{
	if (screen->confirm("   Descend? (Y/N)   ")) {
		tgl.sound("exit");
		goto_next_floor();
	}
}
void t_game::tick_bomb_timers()
{
	player->bomb.tick();
}
void t_game::move_enemies()
{
	for (auto& enemy : enemies) {
		if (enemy.life <= 0) continue;
		bool chance = tgl.rnd(0, 100) > 50;
		if (!chance) continue;
		
		int dx = tgl.rnd(-1, 1);
		int dy = tgl.rnd(-1, 1);
		int newx = enemy.x + dx;
		int newy = enemy.y + dy;

		t_object& o = cur_floor->get(newx, newy).obj;

		if (player->get_x() == newx && player->get_y() == newy) {
			player->hurt_by_enemy(&enemy);
		} else if (o != t_object::wall && o != t_object::none) {
			if (enemy_at(newx, newy) == nullptr) {
				enemy.x = newx;
				enemy.y = newy;
			}
		}
	}
}
void t_game::game_loop()
{
	sync_player();
	visit_surroundings();

	tgl.clear();
		draw_current_floor();
		draw_info();
	tgl.system();

	tick_bomb_timers();
	process_input();
}
void t_game::process_input()
{
	int key = tgl.kb_lastkey();

	// player actions
	if (key == SDLK_RIGHT || key == SDLK_d || key == SDLK_KP_6)
		player->move(1, 0);
	else if (key == SDLK_LEFT || key == SDLK_a || key == SDLK_KP_4)
		player->move(-1, 0);
	else if (key == SDLK_DOWN || key == SDLK_s || key == SDLK_KP_2)
		player->move(0, 1);
	else if (key == SDLK_UP || key == SDLK_w || key == SDLK_KP_8)
		player->move(0, -1);
	else if (key == SDLK_PERIOD || key == SDLK_RETURN || key == SDLK_KP_ENTER)
		player->move(0, 0);
	else if (key == SDLK_SPACE || key == SDLK_KP_5)
		player->drop_bomb();
	// special
	else if (key == SDLK_ESCAPE)
		tgl.exit();
	else if (key == SDLK_F2)
		randomize_color_scheme();
	else if (key == SDLK_F3)
		player->grab_coin(100);
	else if (key == SDLK_F4)
		player->gain_exp(100);
	else if (key == SDLK_F5)
		confirm_goto_next_floor();
}
