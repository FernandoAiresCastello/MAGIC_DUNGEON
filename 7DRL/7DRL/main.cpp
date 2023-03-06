#include "global.h"
#include "t_game.h"

int main(int argc, char* argv[])
{
	t_game* game = new t_game();
	game->run();
	delete game;

	return tgl.exit();
}
