#include "t_game.h"
#include "t_castle.h"
#include "t_tile_loader.h"

t_game::t_game()
{
	tgl.window(256, 192, 0x000000, 4);

	t_tile_loader tloader;
	tloader.load_all();

	castle = new t_castle();
	cur_room = castle->create_empty_room();
}
t_game::~t_game()
{
	delete castle;
	castle = nullptr;
}
void t_game::run()
{
	while (tgl.running()) {
		game_loop();
		tgl.system();
		if (tgl.kb_esc()) tgl.exit();
	}
}
void t_game::game_loop()
{
	if (cur_room) {
		rend_room.draw(cur_room);
	} else {
		tgl.print_tiled("Current room is null", 1, 1);
	}
}
