#include "t_screen.h"
#include "t_tileloader.h"

void t_screen::init()
{
	back_color = 0x201080;
	title.back_color = 0x3020c0;
	title.fore_color = 0xc0c0c0;
	text.color = 0xe0e0e0;

	tgl.window(256, 192, back_color, 4);
	tgl.view_new("main", 0, 0, tgl.width(), tgl.height(), back_color, true);

	map_pos.set(0, 0);
	title.pos.set(1, 18);

	t_tileloader tileloader;
	tileloader.load_all();
}
void t_screen::draw_background()
{
	tgl.view("main");
}
void t_screen::draw_map(t_map& map)
{
	map.draw(map_pos);
}
void t_screen::draw_player(t_player& player)
{
	player.draw(map_pos.x, map_pos.y);
}
void t_screen::print_title(string str)
{
	tgl.font_color(title.fore_color, title.back_color);
	for (int x = 0; x < tgl.cols(); x++) {
		tgl.print_tiled(" ", x, title.pos.y);
	}
	tgl.font_color(title.fore_color);
	tgl.print_tiled(str, title.pos.x, title.pos.y);
}
void t_screen::println(string str)
{
	tgl.font_color(text.color);
	tgl.print_tiled(str, title.pos.x, title.pos.y + 1 + print_line);
	print_line++;
	if (print_line >= max_lines) {
		print_line = 0;
	}
}
