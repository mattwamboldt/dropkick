#ifndef PREVIEW_H
#define PREVIEW_H

#include "mino.h"
#include "types.h"

namespace Tetris
{
	class Preview
	{
		void Init(int width, int x, int y);
		void SetPiece(PieceType piece);
		void Render(SDL_Renderer* renderer) const;
	private:
		Mino window[6 * 4];
	};
}

#endif
