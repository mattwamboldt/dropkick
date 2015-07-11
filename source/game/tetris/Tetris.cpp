#include "Tetris.h"
#include "../audio/Audio.h"
#include "../gamesettings.h"
#include <stdlib.h>

TetrisGame::TetrisGame(SDL_Renderer* screen)
	:screen(screen)
{
	font = assets.loadFont("data/fonts/ariblk.ttf", 32);
	blockImages[BLOCK] = assets.loadTexture(screen, "data/images/tetrominos/square.png");
	blockImages[L] = assets.loadTexture(screen, "data/images/tetrominos/l.png");
	blockImages[J] = assets.loadTexture(screen, "data/images/tetrominos/j.png");
	blockImages[S] = assets.loadTexture(screen, "data/images/tetrominos/s.png");
	blockImages[Z] = assets.loadTexture(screen, "data/images/tetrominos/z.png");
	blockImages[I] = assets.loadTexture(screen, "data/images/tetrominos/i.png");
	blockImages[T] = assets.loadTexture(screen, "data/images/tetrominos/t.png");

	menuTexts[MenuItem::Tetris_Reset].Init(screen, "Reset", font, screenWidth / 2, 45, { 0, 0, 0, 255 }, TextAlign::CENTER);
	menuTexts[MenuItem::Tetris_PlayEndless].Init(screen, "Play Endless", font, screenWidth / 2, 115, { 0, 0, 0, 255 }, TextAlign::CENTER);
	menuTexts[MenuItem::Tetris_PlayVersus].Init(screen, "Play Versus", font, screenWidth / 2, 185, { 0, 0, 0, 255 }, TextAlign::CENTER);
	menuTexts[MenuItem::Tetris_QuitGame].Init(screen, "Quit Game", font, screenWidth / 2, 255, { 0, 0, 0, 255 }, TextAlign::CENTER);
	menuTexts[MenuItem::Tetris_ReloadTextures].Init(screen, "Reload Assets", font, screenWidth / 2, 325, { 0, 0, 0, 255 }, TextAlign::CENTER);
	menuTexts[MenuItem::Tetris_ExitMenu].Init(screen, "Exit Menu", font, screenWidth / 2, 395, { 0, 0, 0, 255 }, TextAlign::CENTER);

	frameImage = assets.loadTexture(screen, "data/images/frame.png");
	background = assets.loadTexture(screen, "data/images/background.png");
	gridImage = assets.loadTexture(screen, "data/images/grid.png");
	menuImage = SDL_CreateTexture(screen, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
	SDL_SetTextureBlendMode(menuImage, SDL_BLENDMODE_BLEND);

	int fieldX = (screenWidth / 4) - (MINO_SIZE * FIELD_WIDTH / 2);
	int fieldY = (screenHeight / 2) - (MINO_SIZE * FIELD_HEIGHT / 2);
	
	frameCounter = 0;
	time = 0.0;

	//Nes Settings
	settings.hardDropEnabled = false;
	settings.ghostEnabled = false;
	settings.holdEnabled = true;
	settings.lockDelayFrames = 0;
	//0.5G
	settings.softDropDistance = 1;
	settings.softDropFrames = 2;

	players[0].Init(fieldX, fieldY, &settings, screen, font, blockImages, frameImage, gridImage);
	players[1].Init(fieldX + (screenWidth / 2), fieldY, &settings, screen, font, blockImages, frameImage, gridImage);

	Reset();
	players[0].spawn();
	players[1].spawn();
}

void TetrisGame::Reset()
{
	running = true;
	gameover = false;

	players[0].Reset();
	players[1].Reset();
}

void TetrisGame::Update(float deltaTime)
{
	float frameTime = 1.0f / 60.0f;
	time += deltaTime;
	if (time > frameTime)
	{
		time = time - frameTime;
		frameCounter++;

		if (!gameover && running)
		{
			players[0].Update();
			players[1].Update();
			gameover = players[0].isGameOver() || players[1].isGameOver();
		}
	}
}

void TetrisGame::Render()
{
	//Todo: center vertically and stretch
	SDL_RenderCopy(screen, background, NULL, NULL);
	players[0].Render(screen);
	players[1].Render(screen);
	if (menuopen)
	{
		SDL_RenderCopy(screen, menuImage, NULL, NULL);
		for (int i = 0; i < NUM_MENUITEMS; ++i)
		{
			menuTexts[i].Render(screen);
		}
	}
}

void TetrisGame::OpenMenu()
{
	menuopen = true;
	focusedMenuItem = 0;
	running = false;
	RedrawMenu();
}

void TetrisGame::CloseMenu()
{
	menuopen = false;
	running = true;
}

void TetrisGame::TriggerMenu()
{
	switch (focusedMenuItem)
	{
	case Tetris_Reset:
		Reset();
		break;
	case Tetris_QuitGame:
		SDL_Event ev;
		ev.type = SDL_QUIT;
		SDL_PushEvent(&ev);
		break;
	}
}

void TetrisGame::RedrawMenu()
{
	SDL_SetRenderTarget(screen, menuImage);
	//Wipe the texture
	SDL_SetRenderDrawColor(screen, 76, 76, 76, 152);
	SDL_RenderClear(screen);

	SDL_Rect drawrect = { screenWidth / 2 - 200, 30, 400, 50 };
	SDL_SetRenderDrawColor(screen, 255, 255, 255, 255);

	for (int i = 0; i < NUM_MENUITEMS; ++i)
	{
		if (focusedMenuItem == i)
		{
			SDL_SetRenderDrawColor(screen, 178, 178, 255, 255);
			SDL_RenderFillRect(screen, &drawrect);
			drawrect.y += drawrect.h + 20;
			SDL_SetRenderDrawColor(screen, 255, 255, 255, 255);
		}
		else
		{
			SDL_RenderFillRect(screen, &drawrect);
			drawrect.y += drawrect.h + 20;
		}
	}

	SDL_SetRenderTarget(screen, NULL);
}

void TetrisGame::MenuHandleInput(SDL_GameControllerButton button)
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
		CloseMenu();
		break;
	}
}

void TetrisGame::HandlePress(SDL_GameControllerButton button, int playerIndex)
{
	if (menuopen)
	{
		MenuHandleInput(button);
		return;
	}

	switch (button)
	{
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		players[playerIndex].move(-1);
		break;

	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
		players[playerIndex].move(1);
		break;

	case SDL_CONTROLLER_BUTTON_DPAD_UP:
		if (settings.hardDropEnabled)
		{
			players[playerIndex].hardDrop();
			if (settings.hardDropLocks)
			{
				players[playerIndex].lock();
			}
		}
		break;

	case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		players[playerIndex].startSoftDrop();
		break;

	case SDL_CONTROLLER_BUTTON_A:
	case SDL_CONTROLLER_BUTTON_Y:
		players[playerIndex].rotate(1);
		break;

	case SDL_CONTROLLER_BUTTON_B:
	case SDL_CONTROLLER_BUTTON_X:
		players[playerIndex].rotate(-1);
		break;

	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		if (settings.holdEnabled)
		{
			players[playerIndex].hold();
			gameover = players[playerIndex].isGameOver() || gameover;
		}
		break;

	case SDL_CONTROLLER_BUTTON_BACK:
		Reset();
		players[0].spawn();
		players[1].spawn();
		break;

	case SDL_CONTROLLER_BUTTON_START:
		OpenMenu();
		break;
	}
}

void TetrisGame::HandleRelease(SDL_GameControllerButton button, int playerIndex)
{
	if (button == SDL_CONTROLLER_BUTTON_DPAD_DOWN)
	{
		players[playerIndex].stopSoftDrop();
	}
}
