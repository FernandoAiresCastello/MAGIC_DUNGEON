#include "t_screen.h"
#include "t_tileloader.h"

void t_screen::init()
{
	back_color = 0x201080;
	title.back_color = 0x3010b0;
	title.fore_color = 0xffff60;
	stats.back_color = 0x4020c0;
	stats.fore_color = 0xf0f0f0;
	text.color = 0xe0e0e0;
	
	map_pos.set(0, 0);
	stats.pos.set(4, 18);
	title.pos.set(1, 20);
	text.pos.set(1, 21);

	t_tileloader tileloader;
	tileloader.load_all();

	tgl.window(256, 192, back_color, 4);
	tgl.view_new("main", 0, 0, tgl.width(), tgl.height(), back_color, true);
	
	tgl.input_cursor('^');
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
void t_screen::print_player_stats(t_player& player)
{
	string values = tgl.fmt(" %04i/%04i  %03i/%03i  %i",
		player.stats.life_pts, player.stats.life_pts_max, player.stats.magic_pts, player.stats.magic_pts_max,
		player.stars);

	tgl.font_color(stats.fore_color, stats.back_color);
	for (int x = 0; x < tgl.cols(); x++) {
		tgl.print_tiled(" ", x, stats.pos.y);
	}
	tgl.font_color(stats.fore_color);
	tgl.print_tiled(values, stats.pos.x, stats.pos.y);

	//tgl.draw_tiled("heart_1", stats.pos.x, stats.pos.y);
	//tgl.draw_tiled("magic_1", stats.pos.x + 11, stats.pos.y);
	//tgl.draw_tiled("star_1", stats.pos.x + 20, stats.pos.y);
}
void t_screen::print_player_stats_detail(t_player& player)
{
	string values = tgl.fmt("H:%i/%i M:%i/%i PA:%i PD:%i MA:%i MD:%i",
		player.stats.life_pts, player.stats.life_pts_max, player.stats.magic_pts, player.stats.magic_pts_max, 
		player.stats.phy_atk, player.stats.phy_def, player.stats.mag_atk, player.stats.mag_def);

	tgl.font_color(stats.fore_color, stats.back_color);
	for (int x = 0; x < tgl.cols(); x++) {
		tgl.print_tiled(" ", x, stats.pos.y);
	}
	tgl.font_color(stats.fore_color);
	tgl.print_tiled(values, stats.pos.x, stats.pos.y);
}
void t_screen::print_title(string str)
{
	tgl.font_color(title.fore_color, title.back_color);
	for (int x = 0; x < tgl.cols(); x++) {
		tgl.print_tiled(" ", x, title.pos.y);
		tgl.print_tiled(" ", x, title.pos.y - 1);
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
void t_screen::println(int ix, string str)
{
	tgl.font_color(text.color);
	tgl.print_tiled(str, title.pos.x, title.pos.y + 1 + ix);
}
string t_screen::input(string msg, void(*redraw_fn)())
{
	clear_lines();
	println(0, msg);
	println(1, ">");
	tgl.input_color(text.color, back_color);
	return tgl.input_tiled(29, text.pos.x + 1, text.pos.y + 1, redraw_fn);
}
void t_screen::clear_lines()
{
	tgl.font_color(text.color, back_color);
	int y = text.pos.y;
	for (int line = y; line < y + max_lines; line++) {
		for (int x = 0; x < tgl.cols(); x++) {
			tgl.print_tiled(" ", x, line);
		}
	}
	print_line = 0;
}
