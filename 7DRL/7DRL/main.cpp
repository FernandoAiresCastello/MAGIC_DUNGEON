#include "global.h"
#include "t_screen.h"
#include "t_color_scheme.h"
#include "t_floor.h"
#include "t_player.h"
#include "t_enemy.h"
#include "t_game.h"

int main(int argc, char* argv[])
{
	t_game* game = new t_game();
	game->run();
	delete game;
	return 0;
}
