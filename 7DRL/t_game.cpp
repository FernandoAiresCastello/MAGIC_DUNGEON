#include "t_game.h"

t_game::t_game()
{
	tgl.title("7DRL 2023");
	tgl.window(256, 192, 0x000000, 4);

	t_room::init_tiles(tgl);
	t_player::init_tiles(tgl);
	t_enemy::init_tiles(tgl);

	player.set_pos(room.width / 2, room.height / 2);
}
t_game::~t_game()
{
}
void t_game::run()
{
	while (tgl.running()) {

		tgl.clear();

		room.draw(tgl);
		player.draw(tgl);
		infobox.print_debug(tgl, player);

		int key = tgl.kb_lastkey();
		player.on_keypress(key);

		if (tgl.kb_esc()) tgl.exit();
		tgl.system();
	}
}
