#include "t_game.h"

t_screen scr;
t_map current_map;
t_player player;

void redraw_screen()
{
	scr.draw_map(current_map);
	scr.draw_player(player);
	scr.print_player_stats(player);
}
void t_game::run()
{
	scr.init();
	tgl.title("7DRL 2023");

	player.map = &current_map;
	player.center_map();
	player.visit();

	scr.draw_background();

	while (tgl.running()) {
		scr.print_title("Player");
		string cmd = scr.input("Enter command", redraw_screen);
		if (tgl.kb_esc()) tgl.exit();
		tgl.system();
	}
}
