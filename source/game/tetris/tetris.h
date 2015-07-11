#ifndef TETRIS_H
#define TETRIS_H

#include "../2d_renderer/Text.h"
#include "SDL/SDL_ttf.h"
#include "settings.h"
#include "tetrisplayer.h"
#include "../resources/assetmanager.h"

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

private:
	SDL_Renderer* screen;

	//These settings could be used for other games
	bool running;
	bool gameover;
	float time;
	unsigned int frameCounter;

	TTF_Font* font;
	SDL_Texture* background;
	SDL_Texture* gridImage;
	SDL_Texture* blockImages[NUM_PIECES];
	SDL_Texture* frameImage;

	TetrisPlayer players[2];
	Settings settings;
	AssetManager assets;

	// Debug menu stuff
	enum MenuItem
	{
		Tetris_Reset = 0,
		Tetris_PlayEndless,
		Tetris_PlayVersus,
		Tetris_QuitGame,
		Tetris_ReloadTextures,
		Tetris_ExitMenu,
		NUM_MENUITEMS
	};

	bool menuopen;
	int focusedMenuItem;
	Text menuTexts[NUM_MENUITEMS];
	SDL_Texture* menuImage;

	void OpenMenu();
	void CloseMenu();
	void TriggerMenu();
	void RedrawMenu();
	void MenuHandleInput(SDL_GameControllerButton button);
};

#endif