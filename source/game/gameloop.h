//Having the gameloop separate means we can treat the game as a library
//that's loaded into other main functons. This'll help later with android
//and possiblly ios. Plus modularity is supposed to be a good idea right?
#ifndef GAMELOOP
#define GAMELOOP

#include "SDL/SDL.h"
#include "tetris/tetris.h"
#include "input\inputmanager.h"



class GameLoop
{
public:
	GameLoop(){}
	~GameLoop();

	bool Init();
	void Run(TetrisGame* game);
	SDL_Renderer* GameLoop::Renderer();

private:
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	InputManager input;

	bool initRan = false;
};

#endif