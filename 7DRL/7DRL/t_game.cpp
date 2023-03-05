#include "t_game.h"

void t_game::run()
{
	scr.init();
	tgl.title("7DRL 2023");

	player.map = &map;
	player.center_map();
	player.visit();

	while (tgl.running()) {
		scr.draw_background();
		scr.draw_map(map);
		scr.draw_player(player);

		scr.print_title("Debug");
		scr.println("Line 1");
		scr.println("Line 2");
		scr.println("Line 3");
		scr.println("Line 4");

		if (tgl.kb_esc()) tgl.exit();

		tgl.system();
	}
}
