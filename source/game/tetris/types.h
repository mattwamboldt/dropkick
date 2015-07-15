#ifndef TETRIS_TYPES_H
#define TETRIS_TYPES_H

#include "SDL/SDL.h"

namespace Tetris
{
	enum PieceType
	{
		BLOCK = 0,
		L,
		J,
		S,
		Z,
		I,
		T,
		NUM_PIECES,
		NONE = -1
	};

	const int FIELD_WIDTH = 10;
	const int FIELD_HEIGHT = 20;
	const int MINO_SIZE = 25;

	static SDL_Color pieceColors[NUM_PIECES] = {
		{ 255, 255, 0, 255 },
		{ 255, 170, 0, 255 },
		{ 0, 0, 255, 255 },
		{ 255, 0, 255, 255 },
		{ 0, 255, 0, 255 },
		{ 255, 0, 0, 255 },
		{ 0, 255, 255, 255 }
	};

	enum RotationState
	{
		SPAWN = 0,
		RIGHT,
		DOWN,
		LEFT,
		NUM_ROTATIONS
	};

	enum CommandType
	{
		// Basic Movement
		MOVE_LEFT,
		MOVE_RIGHT,
		STOP_HORIZONTAL,
		ROTATE_LEFT,
		ROTATE_RIGHT,
		SOFT_DROP_START,
		SOFT_DROP_END,

		// Abilities
		HARD_DROP,
		HOLD_PIECE,
		USE_SPELL,

		NUM_COMMANDS
	};

	enum Mode
	{
		ENDLESS,
		BATTLE,
		PUZZLE,
		COOP,
		SPRINT,
		NUM_MODES
	};
}

#endif
