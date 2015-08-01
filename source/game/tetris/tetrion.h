#ifndef TETRION_H
#define TETRION_H

#include "..\sim\block.h"
#include "..\sim\playfield.h"
#include "SDL\SDL.h"

namespace Tetris
{
	class Tetrion
	{
	public:
		void Init(int w, int h, int x, int y, SDL_Texture** blockTextures = 0, SDL_Texture* frameTexture = 0, SDL_Texture* gridTexture = 0);

		void Render(SDL_Renderer* screen);
		void Render(SDL_Renderer* renderer, Block* block, int x, int y);

		Playfield* GetField(){ return &playfield; }
	private:
		SDL_Texture** blockImages;
		SDL_Texture* frameImage;
		SDL_Texture* frame;
		SDL_Texture* gridImage;
		SDL_Rect frameRect;
		SDL_Rect gridRect;
		int frameWidth;

		Playfield playfield;
	};
}

#endif