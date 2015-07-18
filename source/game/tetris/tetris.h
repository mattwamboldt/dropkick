#ifndef TETRIS_H
#define TETRIS_H

#include "../2d_renderer/Text.h"
#include "SDL/SDL_ttf.h"
#include "settings.h"
#include "tetrisplayer.h"
#include "../resources/assetmanager.h"
#include "..\ui\pausemenu.h"

using namespace Tetris;

class TetrisGame
{
public:
	TetrisGame(SDL_Renderer* screen);
	void Reset();
	void Update(float deltaTime);
	void Render();

	void HandlePress(SDL_GameControllerButton button, int playerIndex);
	void HandleRelease(SDL_GameControllerButton button, int playerIndex);

	void SetMode(Mode mode);
	void OpenMenu();
	void CloseMenu();

	AssetManager assets;

private:
	SDL_Renderer* screen;

	//These settings could be used for other games
	bool running;
	bool gameover;
	float time;
	unsigned int frameCounter;

	SDL_Texture* background;
	SDL_Texture* gridImage;
	SDL_Texture* blockImages[NUM_PIECES];
	SDL_Texture* frameImage;

	TetrisPlayer players[2];
	Randomizer randomizer[2];
	std::vector<Tetrion*> playfields;

	Settings settings;

	Mode currentMode;
	TTF_Font* font;

	PauseMenu menu;
};

#endif