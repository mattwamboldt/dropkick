#ifndef SIM_BLOCK_H
#define SIM_BLOCK_H

#include "../tetris/types.h"
#include "../util/Vector.h"
#include "SDL/SDL.h"

namespace Tetris
{
	//State will let us know whether to render a block or not, This
	//could be done using PieceType however this will allow for powerups
	//in the future
	enum BlockState
	{
		EMPTY,
		ACTIVE,
		CLEAR,
	};

	struct Block
	{
		Block() : state(EMPTY), piece(NONE){}

		BlockState state;
		PieceType piece;
	};
}

#endif
