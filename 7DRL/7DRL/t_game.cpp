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

	load_config();

	tgl.window_gbc(0x000000, 5);
	tgl.title(GAME_TITLE " " GAME_VERSION);
	tgl.mouse(false);
	screen->cols = tgl.cols();
	screen->rows = tgl.rows();

	t_tileset::init();
	load_sounds();
	screen->color_scheme.load();

	if (!skip_intro) {
		show_game_intro();
	}
	goto_next_floor();
}
void t_game::load_config()
{
	if (!tgl.file_exists("config.ini")) return;

	auto lines = tgl.file_lines("config.ini");
	for (auto& raw_line : lines) {
		string line = tgl.trim(raw_line);
		if (line.empty()) continue;
		auto parts = tgl.split(line, '=');
		if (parts.size() != 2) continue;
		string cfg = tgl.lcase(tgl.trim(parts[0]));
		string value = tgl.lcase(tgl.trim(parts[1]));

		if (cfg == "debug" && value == "true") {
			debug_mode = true;
		} else if (cfg == "skip_intro" && value == "true") {
			skip_intro = true;
		} else if (cfg == "initial_floor") {
			player->set_initial_floor(tgl.to_int(value) - 1);
		}
	}
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
	e.x = random_x();
	e.y = random_y();
	e.life = player->get_floor();
	e.attack = player->get_floor();

	int type = -1;
	
	if (player->get_floor() == 1)
		type = 0;
	else if (player->get_floor() <= 5)
		type = tgl.rnd(0, 1);
	else if (player->get_floor() < 10)
		type = tgl.rnd(0, 2);
	else
		type = tgl.rnd(0, 3);

	if (type == 0) {
		e.tile = "slime";
		e.life += tgl.rnd(0, 1);
		e.attack += tgl.rnd(0, 1);
		e.exp = tgl.rnd(1, 3);

	} else if (type == 1) {
		e.tile = "spider";
		e.life += tgl.rnd(1, 3);
		e.attack += tgl.rnd(1, 5);
		e.exp = tgl.rnd(5, 10);

	} else if (type == 2) {
		e.tile = "skelet";
		e.life += tgl.rnd(10, 20);
		e.attack += tgl.rnd(10, 20);
		e.exp = tgl.rnd(15, 20);

	} else if (type == 3) {
		e.tile = "ghost";
		e.life += tgl.rnd(20, 50);
		e.attack += tgl.rnd(20, 50);
		e.exp = tgl.rnd(25, 50);
	}

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
	int color_preset = tgl.rnd(0, screen->color_scheme.user_presets.size() - 1);

	cur_floor->color(
		screen->color_scheme.user_presets[color_preset].first,
		screen->color_scheme.user_presets[color_preset].second);
}
bool t_game::get_trap_gen_chance(int floor_nr)
{
	if (floor_nr < 5) return tgl.rnd_chance(1);
	else if (floor_nr < 10) return tgl.rnd_chance(2);
	else if (floor_nr < 20) return tgl.rnd_chance(4);
	else if (floor_nr < 30) return tgl.rnd_chance(7);
	else if (floor_nr < 50) return tgl.rnd_chance(10);
	else return tgl.rnd_chance(15);
}
void t_game::generate_floor()
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
			bool trap_chance = get_trap_gen_chance(player->get_floor());
			if (trap_chance) {
				cur_floor->set_obj(t_object::trap, x, y);
			} else {
				cur_floor->set_obj(t_object::ground, x, y);
			}
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
						tgl.draw_tiled(e.tile, scrx, scry);
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
		if (player->get_life() > 0) {
			tgl.draw_tiled("player", scrx, scry);
		} else {
			tgl.draw_tiled("tombstone", scrx, scry);
		}

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

	} else if (loc.obj == t_object::trap) {
		tgl.draw_tiled("trap", scrx, scry);

	} else if (loc.obj == t_object::rubble) {
		tgl.draw_tiled("rubble", scrx, scry);
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
	tgl.print_tiled(tgl.fmt("F%i E%iN%i   %3i%%",
		player->get_floor(), player->get_x(), player->get_y(), cur_floor->percent_visited), 1, y);
	// bottom 1
	y = screen->rows - 2;
	tgl.print_tiled(tgl.fmt("~%03i  %02i %8i",
		player->get_life(), player->get_bombs(), player->get_coins()), 1, y);
	tgl.draw_tiled("bomb", 6, y);
	tgl.draw_tiled("collect", 18, y);

	// bottom 2
	y = screen->rows - 1;
	tgl.print_tiled(tgl.fmt("LV:%i XP:%i/%i",
		player->get_exp_level(), player->get_exp(), player->get_max_exp()), 1, y);

	if (debug_mode) {
		tgl.print_tiled("DBG", screen->cols - 4, y);
	}
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
	tgl.sound_load("intro", "sound/intro.wav");
	tgl.sound_load("oops", "sound/oops.wav");
	tgl.sound_load("trap", "sound/trap.wav");
	tgl.sound_load("explosion", "sound/bomb.wav");
	tgl.sound_load("coin", "sound/coin.wav");
	tgl.sound_load("slash", "sound/slash.wav");
	tgl.sound_load("enemy_attack", "sound/enemy_attack.wav");
	tgl.sound_load("enemy_killed", "sound/enemy_killed.wav");
	tgl.sound_load("shop", "sound/shop.wav");
	tgl.sound_load("level_up", "sound/level_up.wav");
	tgl.sound_load("exit", "sound/exit.wav");
	tgl.sound_load("game_over", "sound/game_over.wav");
	tgl.sound_load("ending", "sound/ending.wav");
}
void t_game::show_floor_intro()
{
	const rgb forecolor = 0xffffff;
	const rgb backcolor = 0x101010;

	tgl.backcolor(backcolor);
	tgl.sound_stop();

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

	tgl.sound("intro");
}
void t_game::goto_next_floor()
{
	tgl.sound_stop();
	tgl.sound_await("exit");

	player->next_floor();
	if (player->get_floor() > max_floors) {
		game_complete();
		return;
	}

	int player_x = random_x();
	int player_y = random_y();
	player->set_pos(player_x, player_y);

	generate_floor();
	cur_floor->set_obj(t_object::ground, player_x, player_y);
	player->destroy_walls_around();

	view->setscrl(player_x - screen->cols / 2 + 1, player_y - screen->rows / 2 + 1);

	show_floor_intro();
}
void t_game::confirm_goto_next_floor()
{
	if (screen->confirm("   Descend? (Y/N)   ")) {
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

	redraw_screen();
	tgl.system();

	process_input();
	tick_bomb_timers();
}
void t_game::redraw_screen()
{
	tgl.clear();
	draw_current_floor();
	draw_enemies();
	draw_info();
}
void t_game::process_input()
{
	int key = tgl.kb_lastkey();

	// player actions
	if (key == SDLK_RIGHT || key == SDLK_d)
		player->move(1, 0);
	else if (key == SDLK_LEFT || key == SDLK_a)
		player->move(-1, 0);
	else if (key == SDLK_DOWN || key == SDLK_s)
		player->move(0, 1);
	else if (key == SDLK_UP || key == SDLK_w)
		player->move(0, -1);
	else if (key == SDLK_PERIOD || key == SDLK_RETURN)
		player->move(0, 0);
	else if (key == SDLK_SPACE)
		player->drop_bomb();
	else if (key == SDLK_q && tgl.kb_ctrl())
		player->confirm_suicide();

	// special / debug
	else if (debug_mode) {
		if (key == SDLK_ESCAPE)
			tgl.exit();
		else if (key == SDLK_F2)
			randomize_color_scheme();
		else if (key == SDLK_F3)
			player->grab_coin(100);
		else if (key == SDLK_F4)
			player->gain_exp(100);
		else if (key == SDLK_F5)
			confirm_goto_next_floor();
		else if (key == SDLK_F6)
			change_colors();
		else if (key == SDLK_F7)
			player->obtain_map();
	}
}
void t_game::change_colors()
{
	const int sel_fg = 0;
	const int sel_bg = 1;
	int sel = sel_fg;

	bool finished = false;
	while (tgl.running() && !finished) {
		if (sel == sel_fg) {
			screen->print_bottom(
				tgl.fmt(" >F: %06X   (QWE)", cur_floor->forecolor),
				tgl.fmt("  B: %06X   (ASD)", cur_floor->backcolor));
		} else if (sel == sel_bg) {
			screen->print_bottom(
				tgl.fmt("  F: %06X   (QWE)", cur_floor->forecolor),
				tgl.fmt(" >B: %06X   (ASD)", cur_floor->backcolor));
		}
		tgl.system();
		int key = tgl.kb_lastkey();
		if (key == SDLK_F6 || key == SDLK_ESCAPE || key == SDLK_RETURN) {
			finished = true;
		} else if (key == SDLK_DOWN) {
			sel = sel_bg;
		} else if (key == SDLK_UP) {
			sel = sel_fg;
		} else if (key == SDLK_s && tgl.kb_ctrl()) {
			screen->color_scheme.user_presets.push_back({ cur_floor->forecolor, cur_floor->backcolor });
			screen->print_pause(" Color scheme saved ");
			screen->color_scheme.save();

		} else if (key == SDLK_l && tgl.kb_ctrl()) {
			screen->print_pause(" Color scheme loaded ");
			screen->color_scheme.load();

		} else if (key == SDLK_q) { // R+
			if (sel == sel_fg) cur_floor->forecolor = screen->inc_color_r(cur_floor->forecolor);
			else if (sel == sel_bg) cur_floor->backcolor = screen->inc_color_r(cur_floor->backcolor);
		} else if (key == SDLK_w) { // G+
			if (sel == sel_fg) cur_floor->forecolor = screen->inc_color_g(cur_floor->forecolor);
			else if (sel == sel_bg) cur_floor->backcolor = screen->inc_color_g(cur_floor->backcolor);
		} else if (key == SDLK_e) { // B+
			if (sel == sel_fg) cur_floor->forecolor = screen->inc_color_b(cur_floor->forecolor);
			else if (sel == sel_bg) cur_floor->backcolor = screen->inc_color_b(cur_floor->backcolor);
		} else if (key == SDLK_a) { // R-
			if (sel == sel_fg) cur_floor->forecolor = screen->dec_color_r(cur_floor->forecolor);
			else if (sel == sel_bg) cur_floor->backcolor = screen->dec_color_r(cur_floor->backcolor);
		} else if (key == SDLK_s) { // G-
			if (sel == sel_fg) cur_floor->forecolor = screen->dec_color_g(cur_floor->forecolor);
			else if (sel == sel_bg) cur_floor->backcolor = screen->dec_color_g(cur_floor->backcolor);
		} else if (key == SDLK_d) { // B-
			if (sel == sel_fg) cur_floor->forecolor = screen->dec_color_b(cur_floor->forecolor);
			else if (sel == sel_bg) cur_floor->backcolor = screen->dec_color_b(cur_floor->backcolor);
		}
		redraw_screen();
	}
}
void t_game::game_over()
{
	redraw_screen();
	save_hiscores();
	tgl.sound_stop();
	tgl.pause(200);
	tgl.sound("game_over");
	screen->print_pause(" *** GAME  OVER ***", 1400);
	tgl.exit();
}
void t_game::save_hiscores()
{
	bool game_completed = false;
	int floor_nr = player->get_floor();
	if (floor_nr >= max_floors) {
		floor_nr = max_floors;
		game_completed = true;
	}
	string score = tgl.fmt("FLOOR %i / LEVEL %i / COINS %i", floor_nr, player->get_exp_level(), player->get_coins());
	if (game_completed) {
		score += " / GAME COMPLETED!";
	}
	string line = "[" + tgl.datetime() + "] " + score;
	tgl.file_appendln("hi_scores.txt", line);
}
void t_game::game_complete()
{
	save_hiscores();
	tgl.sound_stop();

	const rgb white = 0xe0e0e0;
	rgb forecolor = white;
	rgb backcolor = 0x101010;
	tgl.backcolor(backcolor);

	int timer = 200;
	while (tgl.running() && timer > 0) {
		tgl.clear();
		tgl.system();
		timer--;
	}

	tgl.sound("ending");

	string info = "                            "
		"~ Congratulations! ~"
		"                    "
		"Whoa!!! You have finally reached the warp zone beyond the last floor of the Magic Dungeon. "
		"You will now be safely teleported home, with all " + tgl.to_string(player->get_coins()) + " coins "
		"that you've collected in your adventure. Yay! Such rich! Many moneys! Very luxurious! WOW!"
		"                    "
		"~ Goodbye! ~";
	
	int info_offset = 0;
	int counter_color_chg = 0;
	while (tgl.running()) {
		counter_color_chg++;
		if (counter_color_chg > 5) {
			counter_color_chg = 0;
			int color_preset = tgl.rnd(0, screen->color_scheme.user_presets.size() - 1);
			forecolor = screen->color_scheme.user_presets[color_preset].first;
			backcolor = screen->color_scheme.user_presets[color_preset].second;

			info_offset++;
			if (info_offset >= info.length()) {
				info_offset = 0;
			}
		}
		tgl.backcolor(backcolor);
		tgl.clear();
		tgl.font_transparent(false);
		tgl.font_color(forecolor, backcolor);
		tgl.print_tiled("*** YOU WIN! ***", 2, 7);
		tgl.print_tiled("   Thank you", 2, 9);
		tgl.print_tiled("  for playing!", 2, 10);
		tgl.font_color(white, backcolor);
		tgl.print_tiled(info.substr(info_offset, 18), 1, 16);
		tgl.system();
	}
}
void t_game::show_game_intro()
{
	tgl.play_notes("l60o7abababbbb");

	const rgb white = 0xe0e0e0;
	rgb forecolor = white;
	rgb backcolor = 0x101010;
	tgl.backcolor(backcolor);
	tgl.font_transparent(false);
	tgl.font_color(white, backcolor);

	int timer = 300;
	while (tgl.running() && timer > 0) {
		tgl.clear();
	//	tgl.print_tiled("                    ", 0, 0);
		tgl.print_tiled("    Developed by    ", 0, 6);
		tgl.print_tiled("Fernando A. Castello", 0, 8);
		tgl.print_tiled("      (C) 2023      ", 0, 10);
		tgl.system();
		timer--;
		if (tgl.kb_lastkey() == SDLK_RETURN) break;
	}
	timer = 300;
	while (tgl.running() && timer > 0) {
		tgl.clear();
	//	tgl.print_tiled("                    ", 0, 0);
		tgl.print_tiled("   Made in 7 days   ", 0, 6);
		tgl.print_tiled("   for the annual   ", 0, 8);
		tgl.print_tiled("   7DRL challenge   ", 0, 10);
		tgl.system();
		timer--;
		if (tgl.kb_lastkey() == SDLK_RETURN) break;
	}
	timer = 300;
	while (tgl.running() && timer > 0) {
		tgl.clear();
	//	tgl.print_tiled("                    ", 0, 0);
		tgl.print_tiled("     Powered by     ", 0, 7);
		tgl.print_tiled(" TileGameLib  (TGL) ", 0, 9);
		tgl.system();
		timer--;
		if (tgl.kb_lastkey() == SDLK_RETURN) break;
	}

	tgl.sound("intro");

	string info = "                            "
		"~~~~~ Story: ~~~~~"
		"                    "
		"Legend has it that huge stashes of gold are hidden deep within the dark, long forgotten chambers of the Magic Dungeon. "
		"Are you brave enough to venture forth and collect as much treasure as you can? "
		"Prepare yourself, as you enter these mystical ruins, armed with only a sword, shield, and some explosives. "
		"Many fabled creatures and ingenious traps await inside each of the magically generated 50 underground floors. "
		"In each floor you must search for the stairs that lead to the next level below. "
		"Once you enter the dungeon, there are only two ways out: reaching the warp zone in the 50th floor, or dying. "
		"Some say that the spirits of ancient merchants still haunt these labyrinths, offering life energy, explosives and maps, "
		"in exchange for some of your precious treasure. Finding them will surely aid you in your quest... "
		"                    "
		"Good luck!";

	int info_offset = 0;
	int counter_color_chg = 0;
	while (tgl.running()) {
		counter_color_chg++;
		if (counter_color_chg > 5) {
			do {
				counter_color_chg = 0;
				int color_preset = tgl.rnd(0, screen->color_scheme.user_presets.size() - 1);
				forecolor = screen->color_scheme.user_presets[color_preset].first;
			}
			while (forecolor == 0x000000 || forecolor == 0x101010 || forecolor == 0x202020);
			
			info_offset++;
			if (info_offset >= info.length()) {
				info_offset = 0;
			}
		}
		tgl.backcolor(backcolor);
		tgl.clear();
		tgl.font_transparent(false);
		tgl.font_color(forecolor, backcolor);
		tgl.color_binary(forecolor, backcolor);

	//	tgl.print_tiled("                    ", 0, 0);
		tgl.print_tiled("   Welcome to the   ", 0, 1);
		tgl.print_tiled(" ~ MAGIC DUNGEON! ~ ", 0, 5);

		tgl.font_color(0x606060, backcolor);
		tgl.print_tiled(GAME_VERSION, 13, 6);

		int y = 9;

		for (int i = 4; i <= 15; i++) {
			tgl.draw_tiled("wall", i, y + 2);
		}
		tgl.draw_tiled("player", 5, y + 1);
		tgl.draw_tiled("slime", 7, y + 1);
		
		tgl.draw_tiled("tower", 12, y);
		tgl.draw_tiled("tower", 14, y);

		tgl.draw_tiled("solid", 12, y + 1);
		tgl.draw_tiled("stairs", 13, y + 1);
		tgl.draw_tiled("solid", 14, y + 1);

		tgl.font_color(white, backcolor);
		tgl.print_tiled("ENTER: Start", 2, 13);
		tgl.print_tiled("   F1: Controls", 2, 14);

		tgl.font_color(white, backcolor);
		tgl.print_tiled(info.substr(info_offset, 18), 1, 16);

		tgl.system();

		int key = tgl.kb_lastkey();
		if (key == SDLK_RETURN && !tgl.kb_alt()) return;
		else if (key == SDLK_F1) show_controls();
		else if (key == SDLK_ESCAPE) tgl.exit();
	}
}
void t_game::show_controls()
{
	const rgb white = 0xe0e0e0;
	const rgb yellow = 0xe0e010;
	rgb forecolor = white;
	rgb backcolor = 0x101010;
	tgl.backcolor(backcolor);
	tgl.font_transparent(false);

	bool done = false;
	while (tgl.running() && !done) {
		
		tgl.clear();
		tgl.font_color(yellow, backcolor);
		tgl.print_tiled("Controls:", 1, 1);

		tgl.font_color(white, backcolor);
		tgl.print_tiled("Arrow keys = move", 1, 4);
		tgl.print_tiled("W, A, S, D = move", 1, 6);
		tgl.print_tiled("Space = drop bomb", 1, 8);
		tgl.print_tiled("Enter, . = wait", 1, 10);
		tgl.print_tiled("Ctrl+Q = give up", 1, 12);
		tgl.print_tiled("Alt+Enter = ", 1, 14);
		tgl.print_tiled(" toggle fullscreen", 1, 15);

		tgl.system();

		int key = tgl.kb_lastkey();
		if (key == SDLK_RETURN && !tgl.kb_alt()) done = true;
		else if (key == SDLK_F1 || key == SDLK_ESCAPE) done = true;
	}
}
