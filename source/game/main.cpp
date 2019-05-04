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

#if defined(_MSC_VER) && (_MSC_VER >= 1900)
FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}
#endif