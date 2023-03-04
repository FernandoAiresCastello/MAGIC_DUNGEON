#include "t_map.h"

void t_map::draw(t_pos& pos)
{
	for (int py = 0; py < height; py++) {
		for (int px = 0; px < width; px++) {
			cells[py][px].draw(px + pos.x, py + pos.y);
		}
	}
}
