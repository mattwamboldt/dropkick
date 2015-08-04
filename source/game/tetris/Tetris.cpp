#include "Tetris.h"
#include "../audio/Audio.h"
#include "../gamesettings.h"
#include <stdlib.h>

TetrisGame::TetrisGame(SDL_Renderer* screen)
	:screen(screen), currentMode(NUM_MODES), menu(this, screen)
{
	font = assets.loadFont("data/fonts/ariblk.ttf", 32);
	blockImages[BLOCK] = assets.loadTexture(screen, "data/images/tetrominos/square.png");
	blockImages[L] = assets.loadTexture(screen, "data/images/tetrominos/l.png");
	blockImages[J] = assets.loadTexture(screen, "data/images/tetrominos/j.png");
	blockImages[S] = assets.loadTexture(screen, "data/images/tetrominos/s.png");
	blockImages[Z] = assets.loadTexture(screen, "data/images/tetrominos/z.png");
	blockImages[I] = assets.loadTexture(screen, "data/images/tetrominos/i.png");
	blockImages[T] = assets.loadTexture(screen, "data/images/tetrominos/t.png");

	frameImage = assets.loadTexture(screen, "data/images/frame.png");
	background = assets.loadTexture(screen, "data/images/background.png");
	gridImage = assets.loadTexture(screen, "data/images/grid.png");
	
	frameCounter = 0;
	time = 0.0;

	//Nes Settings
	settings.hardDropEnabled = true;
	settings.ghostEnabled = true;
	settings.holdEnabled = true;
	settings.lockDelayFrames = 10;
	//0.5G
	settings.softDropDistance = 1;
	settings.softDropFrames = 2;

	SetMode(COOP);
}

void TetrisGame::SetMode(Mode mode)
{
	if (mode == currentMode)
	{
		Reset();
	}
	else
	{
		for (unsigned int i = 0; i < playfields.size(); ++i)
		{
			delete playfields[i];
		}

		playfields.clear();

		if (mode == Mode::BATTLE)
		{
			int fieldWidth = MINO_SIZE * FIELD_WIDTH;

			int fieldX = (screenWidth / 4) - (fieldWidth / 2);
			int fieldY = (screenHeight / 2) - (MINO_SIZE * FIELD_HEIGHT / 2);

			Tetrion* leftGrid = new Tetrion();
			leftGrid->Init(FIELD_WIDTH, FIELD_HEIGHT, fieldX, fieldY, blockImages, frameImage, gridImage);
			playfields.push_back(leftGrid);

			Tetrion* rightGrid = new Tetrion();
			rightGrid->Init(FIELD_WIDTH, FIELD_HEIGHT, fieldX + (screenWidth / 2), fieldY, blockImages, frameImage, gridImage);
			playfields.push_back(rightGrid);

			queues[0].Init(fieldX + fieldWidth + MINO_SIZE, fieldY, blockImages, frameImage);
			queues[1].Init(fieldX + (screenWidth / 2) + fieldWidth + MINO_SIZE, fieldY, blockImages, frameImage);

			players[0].Init(leftGrid, fieldX, fieldY, &settings, screen, font, blockImages, frameImage, gridImage);
			players[1].Init(rightGrid, fieldX + (screenWidth / 2), fieldY, &settings, screen, font, blockImages, frameImage, gridImage);

			players[0].SetQueue(queues);
			players[1].SetQueue(queues + 1);

			Reset();

			players[0].spawn();
			players[1].spawn();
		}
		else if (mode == Mode::COOP)
		{
			int fieldWidth = MINO_SIZE * FIELD_WIDTH * 2;
			int fieldX = (screenWidth / 2) - (MINO_SIZE * FIELD_WIDTH);
			int fieldY = (screenHeight / 2) - (MINO_SIZE * FIELD_HEIGHT / 2);

			Tetrion* coopGrid = new Tetrion();
			coopGrid->Init(FIELD_WIDTH * 2, FIELD_HEIGHT, fieldX, fieldY, blockImages, frameImage, gridImage);
			playfields.push_back(coopGrid);

			queues[0].Init(fieldX - MINO_SIZE * 5, fieldY, blockImages, frameImage);
			queues[1].Init(fieldX + fieldWidth + MINO_SIZE, fieldY, blockImages, frameImage);

			players[0].Init(coopGrid, fieldX, fieldY, &settings, screen, font, blockImages, frameImage, gridImage);
			players[1].Init(coopGrid, fieldX + (screenWidth / 2), fieldY, &settings, screen, font, blockImages, frameImage, gridImage);

			players[0].SetQueue(queues);
			players[1].SetQueue(queues + 1);

			Reset();

			players[0].spawn();
			players[1].spawn();
		}
		else if (mode == ENDLESS)
		{
			int fieldWidth = MINO_SIZE * FIELD_WIDTH;
			int fieldX = (screenWidth / 2) - (fieldWidth / 2);
			int fieldY = (screenHeight / 2) - (MINO_SIZE * FIELD_HEIGHT / 2);

			Tetrion* mainGrid = new Tetrion();
			mainGrid->Init(FIELD_WIDTH, FIELD_HEIGHT, fieldX, fieldY, blockImages, frameImage, gridImage);
			playfields.push_back(mainGrid);

			queues[0].Init(fieldX + fieldWidth + MINO_SIZE, fieldY, blockImages, frameImage);
			players[0].Init(mainGrid, fieldX, fieldY, &settings, screen, font, blockImages, frameImage, gridImage);

			players[0].SetQueue(queues);

			running = true;
			gameover = false;
			queues[0].Reset();

			players[0].Reset();
			players[0].spawn();
		}

		currentMode = mode;
	}
}

void TetrisGame::Reset()
{
	running = true;
	gameover = false;

	players[0].Reset();
	players[1].Reset();
	queues[0].Reset();
	queues[1].Reset();
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
			if (currentMode == Mode::BATTLE || currentMode == COOP)
			{
				players[0].Update();
				players[1].Update();
				gameover = players[0].isGameOver() || players[1].isGameOver();
			}
			else
			{
				players[0].Update();
				gameover = players[0].isGameOver();
			}
		}
	}
}

void TetrisGame::Render()
{
	//Todo: center vertically and stretch
	SDL_RenderCopy(screen, background, NULL, NULL);
	for (unsigned int i = 0; i < playfields.size(); ++i)
	{
		playfields[i]->Render(screen);
	}

	queues[0].Render(screen);
	players[0].Render(screen);

	if (currentMode == BATTLE || currentMode == COOP)
	{
		players[1].Render(screen);
		queues[1].Render(screen);
	}
	
	if (menu.isOpen)
	{
		menu.Render();
	}
}

void TetrisGame::CloseMenu()
{
	menu.isOpen = false;
	running = true;
}

void TetrisGame::HandlePress(SDL_GameControllerButton button, int playerIndex)
{
	if (menu.isOpen)
	{
		menu.HandleInput(button);
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
		running = false;
		menu.OpenMenu();
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
