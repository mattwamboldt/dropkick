#ifndef TETRIMINO_H
#define TETRIMINO_H

#include "tetrion.h"

namespace Tetris
{
	class Tetromino
	{
	public:
		Tetromino();

		void setField(Tetrion* tetrion);
		void place();
		void lift();
		bool shift(int dx);
		bool rotate(int direction);

		// Blockout is when a piece spawns in a position overlapping a locked mino
		bool isBlockout();
		bool isValidPlacement(int x, int y);

		int x, y;
		RotationState orientation;
		PieceType type;

		// http://tetrisconcept.net/wiki/Wall_kick
		// Some games don't support wall kicks including nes tetris
		// At some point this will be a more fleshed out setting
		bool kickEnabled;
		int locktimer;

	private:
		Tetrion* field;
	};
}

#endif