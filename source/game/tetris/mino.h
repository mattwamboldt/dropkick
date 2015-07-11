#ifndef MINO_H
#define MINO_H

#include "types.h"
#include "../util/Vector.h"
#include "SDL/SDL.h"

namespace Tetris
{
	//State will let us know whether to render a block or not, This
	//could be done using PieceType however this will allow for powerups
	//in the future
	enum MinoState
	{
		EMPTY,
		ACTIVE,
		CLEAR,
	};

	// Represents an individual block on the playfield
	class Mino
	{
	public:
		Mino() : state(EMPTY), piece(NONE){}

		void Render(SDL_Renderer* renderer, SDL_Texture** blockImages = 0) const;

		MinoState state;
		Vector position;
		int width;
		int height;
		PieceType piece;
	};
}

#endif