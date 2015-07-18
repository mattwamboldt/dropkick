#ifndef UI_PAUSEMENU_H
#define UI_PAUSEMENU_H
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#include "..\2d_renderer\text.h"

class TetrisGame;

namespace Tetris
{
	class PauseMenu
	{
	public:
		PauseMenu(TetrisGame* game, SDL_Renderer* screen);
		void HandleInput(SDL_GameControllerButton button);
		void Render();
		void OpenMenu();

		bool isOpen;
	private:
		enum MenuItem
		{
			Tetris_Reset = 0,
			Tetris_PlayEndless,
			Tetris_PlayVersus,
			Tetris_PlayCoop,
			Tetris_QuitGame,
			Tetris_ReloadTextures,
			Tetris_ExitMenu,
			NUM_MENUITEMS
		};

		TetrisGame* mGame;
		SDL_Renderer* mScreen;
		TTF_Font* font;
		
		int focusedMenuItem;
		Text menuTexts[NUM_MENUITEMS];
		SDL_Texture* menuImage;

		void TriggerMenu();
		void RedrawMenu();
	};
}

#endif