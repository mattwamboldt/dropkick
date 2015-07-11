#include "gameloop.h"
#include "tetris/tetris.h"
#include <random>
#include <time.h>

int main(int argc, char* args[])
{
	srand((unsigned int)time(NULL));

	GameLoop loop;

	if (loop.Init())
	{
		TetrisGame game = TetrisGame(loop.Renderer());
		loop.Run(&game);
	}

	return 0;
}
