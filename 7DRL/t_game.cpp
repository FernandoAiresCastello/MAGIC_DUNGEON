#include "t_game.h"

t_game::t_game()
{
	tgl.title("7DRL 2023");
	tgl.window(256, 192, 0x000000, 4);
	tgl.view_new("main", 0, 0, tgl.width(), tgl.height(), 0x000000, true);

	t_room::init_tiles(tgl);
	t_player::init_tiles(tgl);

	player.set_pos(10, 10);
}
t_game::~t_game()
{
}
void t_game::run()
{
	while (tgl.running()) {

		tgl.view("main");

		room.draw(tgl);
		player.draw(tgl);
		infobox.draw(tgl);

		int key = tgl.kb_lastkey();
		player.on_keypress(key);

		if (tgl.kb_esc()) tgl.exit();
		tgl.system();
	}
}
