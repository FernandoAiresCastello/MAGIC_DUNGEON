#include "t_player.h"
#include "t_game.h"
#include "t_screen.h"
#include "t_view.h"
#include "t_floor.h"
#include "t_object.h"
#include "t_enemy.h"

t_player::t_player(t_game* game, t_floor* cur_floor, t_screen* screen) :
	bomb(this, cur_floor)
{
	this->game = game;
	this->cur_floor = cur_floor;
	this->screen = screen;
}

int t_player::get_floor() { return floor_nr; }
int t_player::get_x() { return x; }
int t_player::get_y() { return y; }
int t_player::get_prevx() { return prevx; }
int t_player::get_prevy() { return prevy; }
int t_player::get_life() { return life; }
int t_player::get_coins() { return coins; }
int t_player::get_bombs() { return bombs; }
int t_player::get_exp() { return exp; }
int t_player::get_max_exp() { return max_exp; }
int t_player::get_exp_level() { return exp_level; }

void t_player::set_pos(int newx, int newy)
{
	prevx = x;
	prevy = y;
	x = newx;
	y = newy;
}
void t_player::move(int dx, int dy)
{
	t_direction old_dir = dir;

	if (dx > 0) dir = t_direction::east;
	else if (dx < 0) dir = t_direction::west;
	if (dy > 0) dir = t_direction::south;
	else if (dy < 0) dir = t_direction::north;

	int newx = x + dx;
	int newy = y + dy;
	t_enemy* enemy_detected = game->enemy_at(newx, newy);

	if (is_oob(newx, newy)) {
		tgl.play_notes("l30o3ccc");
		screen->print_pause("    Out of bounds");
	} else if (enemy_detected != nullptr) {
		bool chance = tgl.rnd(0, 100) > 30;
		if (chance) {
			attack_enemy(enemy_detected);
		} else {
			hurt_by_enemy(enemy_detected);
		}
	} else if (can_move_to(newx, newy)) {
		set_pos(newx, newy);
		screen->view.scroll(dx, dy);
		trigger_collisions();
		game->enemy_turn();
	}
}
bool t_player::can_move_to(int newx, int newy)
{
	t_location& loc = cur_floor->get(newx, newy);
	t_object& o = loc.obj;
	t_object& e = loc.entity;

	return o == t_object::ground || o == t_object::stairs || o == t_object::coin;
}
void t_player::step_back()
{
	x = prevx;
	y = prevy;
	screen->view.undo_last_scroll();
}
bool t_player::is_oob(int newx, int newy)
{
	return newx < 0 || newy < 0 || newx >= cur_floor->width || newy >= cur_floor->height;
}
t_location& t_player::getloc()
{
	return cur_floor->get(x, y);
}
bool t_player::is_on_stairs()
{
	return getloc().obj == t_object::stairs;
}
bool t_player::found_shop(t_shoptype type)
{
	if (type == t_shoptype::bomb) {
		return x == game->bomb_shop.x && y == game->bomb_shop.y;
	} else if (type == t_shoptype::life) {
		return x == game->life_shop.x && y == game->life_shop.y;
	} else if (type == t_shoptype::map) {
		return x == game->map_shop.x && y == game->map_shop.y;
	}
	return false;
}
bool t_player::found_coin()
{
	return getloc().obj == t_object::coin;
}
void t_player::grab_coin(int count)
{
	coins += count;
	tgl.sound("coin");
	getloc().obj = t_object::ground;
}
void t_player::drop_bomb()
{
	if (!bomb.is_active() && bombs > 0) {
		bombs--;
		bomb.activate(x, y);
	}
}
void t_player::destroy_walls_around()
{
	cur_floor->set_obj(t_object::ground, x - 1, y - 1);
	cur_floor->set_obj(t_object::ground, x + 0, y - 1);
	cur_floor->set_obj(t_object::ground, x + 1, y - 1);
	cur_floor->set_obj(t_object::ground, x - 1, y);
	cur_floor->set_obj(t_object::ground, x, y);
	cur_floor->set_obj(t_object::ground, x + 1, y);
	cur_floor->set_obj(t_object::ground, x - 1, y + 1);
	cur_floor->set_obj(t_object::ground, x + 0, y + 1);
	cur_floor->set_obj(t_object::ground, x + 1, y + 1);
}
void t_player::set_initial_floor(int number)
{
	floor_nr = number;
}
void t_player::next_floor()
{
	floor_nr++;
}
void t_player::trigger_collisions()
{
	if (is_on_stairs()) {
		game->confirm_goto_next_floor();
	} else if (found_coin()) {
		grab_coin();
	} else if (found_shop(t_shoptype::life)) {
		game->life_shop.sell(this, screen);
	} else if (found_shop(t_shoptype::bomb)) {
		game->bomb_shop.sell(this, screen);
	} else if (found_shop(t_shoptype::map)) {
		game->map_shop.sell(this, screen);
	}
}
void t_player::receive_damage(int damage)
{
	life -= damage;
	if (life <= 0) {
		life = 0;
		game->game_over();
	}
}
void t_player::hurt_by_bomb()
{
	receive_damage(bomb.get_damage());
}
void t_player::hurt_by_enemy(t_enemy* enemy)
{
	tgl.sound("enemy_attack");
	receive_damage(enemy->attack);
}
void t_player::attack_enemy(t_enemy* enemy)
{
	if (enemy->life > 0) {
		tgl.sound("slash");
		enemy->life -= (power + (exp_level - 1));
		if (enemy->life <= 0) {
			tgl.sound("enemy_killed");
			gain_exp(enemy->exp);
		}
	}
}
bool t_player::spend_coins(int count)
{
	if (coins - count < 0) {
		return false;
	} else {
		coins -= count;
		return true;
	}
}
void t_player::restore_life(int points)
{
	life += points;
	if (life > max_life) {
		life = max_life;
	}
}
void t_player::restore_bombs(int count)
{
	bombs += count;
	if (bombs > max_bombs) {
		bombs = max_bombs;
	}
}
void t_player::obtain_map()
{
	cur_floor->visit_all();
}
void t_player::gain_exp(int points)
{
	exp += points;
	if (exp > max_exp) {
		tgl.sound("level_up");
		screen->print_pause("      Level up!");
		exp_level++;
		exp = 0;
	}
}
void t_player::confirm_suicide()
{
	if (screen->confirm("  Kill self? (Y/N)")) {
		life = 0;
		game->game_over();
	}
}
