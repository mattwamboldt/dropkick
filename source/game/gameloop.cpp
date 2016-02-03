#include "gameloop.h"
#include "gamesettings.h"
#include "SDL/SDL_image.h"
#include <cstdio>
#include "audio/Audio.h"
#include "SDL/SDL_ttf.h"

GameLoop::~GameLoop()
{
	if (initRan)
	{
		if (TTF_WasInit())
		{
			TTF_Quit();
		}

		IMG_Quit();
		DestroyAudio();
		SDL_CloseAudio();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
}

bool GameLoop::Init()
{
	//Start SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		printf("SDL_Init: %s\n", SDL_GetError());
		return false;
	}

	initRan = true;

	// load support for the JPG and PNG image formats
	int flags = IMG_INIT_PNG;
	int initted = IMG_Init(flags);
	if ((initted & flags) != flags) {
		printf("IMG_Init: Failed to init required png support!\n");
		printf("IMG_Init: %s\n", IMG_GetError());
		return false;
	}

	window = SDL_CreateWindow("Hawkeye", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, 0);
	if (window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (renderer == NULL)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}

	SDL_AudioSpec wanted;
	wanted.freq = FREQUENCY;
	wanted.format = AUDIO_S8;
	wanted.channels = 1;    /* 1 = mono, 2 = stereo */
	wanted.samples = SAMPLE_RATE;  /* Good low-latency value for callback */
	wanted.callback = audioCallback;
	wanted.userdata = NULL;

	/* Open the audio device, forcing the desired format */
	if (SDL_OpenAudio(&wanted, NULL) < 0)
	{
		printf("Couldn't open audio: %s\n", SDL_GetError());
		return false;
	}

	InitAudio();
	SDL_PauseAudio(0);

	input.Init();
	TTF_Init();
	return true;
}

void GameLoop::Run(TetrisGame* game)
{
	bool quit = false;
	SDL_Event event;

	Uint32 old_time, current_time;
	float ftime;
	current_time = SDL_GetTicks();

	while (!quit)
	{
		old_time = current_time;
		current_time = SDL_GetTicks();
		ftime = (current_time - old_time) / 1000.0f;

		// Respond to user OS and input events
		while (SDL_PollEvent(&event))
		{
			if (input.HandleEvent(game, event))
			{
				break;
			}

			switch (event.type)
			{
			case SDL_QUIT:
				quit = true;
				break;
			}
		}

		game->Update(ftime);

		//Redraw
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(renderer);

		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		//draw the game
		game->Render();

		//push the screen to the video card and display
		SDL_RenderPresent(renderer);

		SDL_Delay(1);
	}
}

SDL_Renderer* GameLoop::Renderer()
{
	return renderer;
}