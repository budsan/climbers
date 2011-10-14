#include "climbers.h"

int main(int argc, char *argv[])
{
	Game* game = new Climbers();
        if (game->Init()) game->Run();
	delete game;

	return 0;
}
