#ifndef TETRIS_PIECEQUEUE_H
#define TETRIS_PIECEQUEUE_H

#include "../2d_renderer/Text.h"
#include "../tetris/tetrion.h"
#include "../sim/randomizer.h"
#include "../sim/tetrimino.h"
#include "../sim/settings.h"

namespace Tetris
{
	class PieceQueue
	{
	public:
		void Init(int x, int y, SDL_Texture** blockTextures, SDL_Texture* frameTexture);
		void Reset();
		void Render(SDL_Renderer* screen);
		PieceType advance();

	private:
		Randomizer randomizer;

		Tetrion sidePreview;
		Tetromino previewPieces[3];
	};
}

#endif