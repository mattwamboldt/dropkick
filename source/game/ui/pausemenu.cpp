#include "pausemenu.h"

#include "..\tetris\tetris.h"
#include "..\gamesettings.h"

namespace Tetris
{
	PauseMenu::PauseMenu(TetrisGame* game, SDL_Renderer* screen)
		:mScreen(screen), mGame(game)
	{
		menuImage = SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
		SDL_SetTextureBlendMode(menuImage, SDL_BLENDMODE_BLEND);
		font = mGame->assets.loadFont("data/fonts/ariblk.ttf", 32);
		menuTexts[MenuItem::Tetris_Reset].Init(screen, "Reset", font, screenWidth / 2, 45, { 0, 0, 0, 255 }, TextAlign::CENTER);
		menuTexts[MenuItem::Tetris_PlayEndless].Init(screen, "Play Endless", font, screenWidth / 2, 115, { 0, 0, 0, 255 }, TextAlign::CENTER);
		menuTexts[MenuItem::Tetris_PlayVersus].Init(screen, "Play Versus", font, screenWidth / 2, 185, { 0, 0, 0, 255 }, TextAlign::CENTER);
		menuTexts[MenuItem::Tetris_PlayCoop].Init(screen, "Play Co-op", font, screenWidth / 2, 255, { 0, 0, 0, 255 }, TextAlign::CENTER);
		menuTexts[MenuItem::Tetris_QuitGame].Init(screen, "Quit Game", font, screenWidth / 2, 325, { 0, 0, 0, 255 }, TextAlign::CENTER);
		menuTexts[MenuItem::Tetris_ReloadTextures].Init(screen, "Reload Assets", font, screenWidth / 2, 395, { 0, 0, 0, 255 }, TextAlign::CENTER);
		menuTexts[MenuItem::Tetris_ExitMenu].Init(screen, "Exit Menu", font, screenWidth / 2, 765, { 0, 0, 0, 255 }, TextAlign::CENTER);
	}

	void PauseMenu::Render()
	{
		SDL_RenderCopy(mScreen, menuImage, NULL, NULL);
		for (int i = 0; i < NUM_MENUITEMS; ++i)
		{
			menuTexts[i].Render(mScreen);
		}
	}

	void PauseMenu::OpenMenu()
	{
		isOpen = true;
		focusedMenuItem = 0;
		RedrawMenu();
	}

	void PauseMenu::TriggerMenu()
	{
		switch (focusedMenuItem)
		{
		case Tetris_Reset:
			mGame->Reset();
			break;

		case Tetris_PlayEndless:
			mGame->SetMode(ENDLESS);
			break;

		case Tetris_PlayVersus:
			mGame->SetMode(BATTLE);
			break;

		case Tetris_PlayCoop:
			mGame->SetMode(COOP);
			break;

		case Tetris_QuitGame:
			SDL_Event ev;
			ev.type = SDL_QUIT;
			SDL_PushEvent(&ev);
			break;
		}
	}

	void PauseMenu::RedrawMenu()
	{
		SDL_SetRenderTarget(mScreen, menuImage);
		//Wipe the texture
		SDL_SetRenderDrawColor(mScreen, 76, 76, 76, 152);
		SDL_RenderClear(mScreen);

		SDL_Rect drawrect = { screenWidth / 2 - 200, 30, 400, 50 };
		SDL_SetRenderDrawColor(mScreen, 255, 255, 255, 255);

		for (int i = 0; i < NUM_MENUITEMS; ++i)
		{
			if (focusedMenuItem == i)
			{
				SDL_SetRenderDrawColor(mScreen, 178, 178, 255, 255);
				SDL_RenderFillRect(mScreen, &drawrect);
				drawrect.y += drawrect.h + 20;
				SDL_SetRenderDrawColor(mScreen, 255, 255, 255, 255);
			}
			else
			{
				SDL_RenderFillRect(mScreen, &drawrect);
				drawrect.y += drawrect.h + 20;
			}
		}

		SDL_SetRenderTarget(mScreen, NULL);
	}

	void PauseMenu::HandleInput(SDL_GameControllerButton button)
	{
		switch (button)
		{
		case SDL_CONTROLLER_BUTTON_DPAD_UP:
			--focusedMenuItem;
			focusedMenuItem = (focusedMenuItem + NUM_MENUITEMS) % NUM_MENUITEMS;
			RedrawMenu();
			break;

		case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
			++focusedMenuItem;
			focusedMenuItem %= NUM_MENUITEMS;
			RedrawMenu();
			break;

		case SDL_CONTROLLER_BUTTON_A:
			TriggerMenu();
		case SDL_CONTROLLER_BUTTON_B:
		case SDL_CONTROLLER_BUTTON_START:
			mGame->CloseMenu();
			break;
		}
	}
}
