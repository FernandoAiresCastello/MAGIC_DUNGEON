#include "t_map.h"

void t_map::draw(t_pos& pos)
{
	for (int py = 0; py < height; py++) {
		for (int px = 0; px < width; px++) {
			cells[py][px].draw(px + pos.x, py + pos.y);
		}
	}
}
t_mapcell& t_map::get_cell(t_pos pos)
{
	return cells[pos.y][pos.x];
}
void t_map::set_cell(int x, int y, t_mapcell cell)
{
	cells[y][x].set_equal(cell);
}
