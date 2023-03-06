#include "t_game.h"
#include "mapgen.h"

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

	mapgen_grasslands(current_map); // todo: randomly select biome generation

	player.map = &current_map;
	player.center_map();
	player.visit();

	scr.draw_background();

	while (tgl.running()) {
		if (tgl.kb_esc()) tgl.exit();
		scr.print_title("Player");
		string cmd = scr.input("Enter command", redraw_screen);
		exec_cmd(cmd);
		tgl.system();
	}
}
void t_game::exec_cmd(string raw_cmd)
{
	raw_cmd = tgl.ucase(tgl.trim(raw_cmd));

	vector<string> cmd = tgl.split(raw_cmd, ' ');
}
