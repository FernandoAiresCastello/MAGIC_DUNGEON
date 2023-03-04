#include "t_infobox.h"

void t_infobox::print_test(TGL& tgl)
{
	title(tgl, "Title");
	println(tgl, 0, "Line 0");
	println(tgl, 1, "Line 1");
	println(tgl, 2, "Line 2");
	println(tgl, 3, "Line 3");
}
void t_infobox::print_debug(TGL& tgl, t_player& player)
{
	title(tgl, "Debug");
	println(tgl, 0, tgl.fmt("Player: %i,%i", player.x(), player.y()));
}
void t_infobox::title(TGL& tgl, string str)
{
	tgl.color_normal(0xd0d0d0, 0x000000, 0, 0);
	for (int x = 0; x < tgl.cols(); x++) {
		tgl.print_tiled(" ", x, y);
	}
	tgl.print_tiled(str, 1, y);
}
void t_infobox::println(TGL& tgl, int line, string str)
{
	println(tgl, line, 0xe0e0e0, str);
}
void t_infobox::println(TGL& tgl, int line, rgb color, string str)
{
	tgl.color_single(color);
	tgl.print_tiled(str, 1, y + 1 + line);
}
