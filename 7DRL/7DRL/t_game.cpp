#include "t_game.h"

void t_game::run()
{
	scr.init();
	tgl.title("7DRL 2023");

	while (tgl.running()) {
		scr.draw_background();
		scr.draw_title("Debug");
		scr.draw_map(map);
		
		if (tgl.kb_esc()) tgl.exit();

		tgl.system();
	}
}
