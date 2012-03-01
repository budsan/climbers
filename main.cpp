#include "climbers.h"

int main(int argc, char *argv[])
{
	Game* game = new Climbers();
        if (game->init()) game->run();
	delete game;

	return 0;
}
