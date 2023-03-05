#include "t_player.h"

t_player::t_player()
{
	tile = "player";
}
void t_player::visit()
{
	if (!map) return;

	// center
	map->get_cell(pos.offset(-1, 0)).visited = true;
	map->get_cell(pos.offset(0, 0)).visited = true;
	map->get_cell(pos.offset(1, 0)).visited = true;
	// top
	map->get_cell(pos.offset(-1, -1)).visited = true;
	map->get_cell(pos.offset(0, -1)).visited = true;
	map->get_cell(pos.offset(1, -1)).visited = true;
	// bottom
	map->get_cell(pos.offset(-1, 1)).visited = true;
	map->get_cell(pos.offset(0, 1)).visited = true;
	map->get_cell(pos.offset(1, 1)).visited = true;
}
