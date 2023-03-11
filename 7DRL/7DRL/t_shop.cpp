#include "t_shop.h"
#include "t_screen.h"
#include "t_player.h"
#include "t_floor.h"

void t_shop::spawn(t_shoptype type, t_floor* cur_floor, int x, int y)
{
	this->type = type;
	this->x = x;
	this->y = y;

	cur_floor->set_obj(t_object::ground, x, y);
}

void t_shop::sell(t_player* player, t_screen* screen)
{
	bool not_enough_money = false;

	if (type == t_shoptype::life) {
		if (screen->confirm(" Restore life for", "  350 coins? (Y/N)")) {
			if (player->spend_coins(350)) {
				tgl.sound("shop");
				player->restore_life(100);
			} else {
				not_enough_money = true;
			}
		}
	} else if (type == t_shoptype::bomb) {
		if (screen->confirm(" Buy 1 bomb for", "  100 coins? (Y/N)")) {
			if (player->spend_coins(100)) {
				tgl.sound("shop");
				player->restore_bombs(1);
			} else {
				not_enough_money = true;
			}
		}
	} else if (type == t_shoptype::map) {
		if (screen->confirm(" Buy map for", "  500 coins? (Y/N)")) {
			if (player->spend_coins(500)) {
				tgl.sound("shop");
				player->obtain_map();
			} else {
				not_enough_money = true;
			}
		}
	}

	if (not_enough_money) {
		screen->print_pause(" Not enough coins!");
	}
	player->step_back();
}
