#ifndef TETRION_H
#define TETRION_H

#include "mino.h"
#include "SDL/SDL.h"

namespace Tetris
{
	class Tetrion
	{
	public:
		~Tetrion();

		void Init(int w, int h, int x, int y, SDL_Texture** blockTextures = 0, SDL_Texture* frameTexture = 0, SDL_Texture* gridTexture = 0);
		Mino* get(int x, int y);
		void clear();
		int checkLines();
		void removeCleared();

		void Render(SDL_Renderer* screen);

	private:
		SDL_Texture** blockImages;
		SDL_Texture* frameImage;
		SDL_Texture* frame;
		SDL_Texture* gridImage;
		SDL_Rect frameRect;
		SDL_Rect gridRect;
		int frameWidth;

		Mino* blocks;
		int width, height;
	};
}

#endif