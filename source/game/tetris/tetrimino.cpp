#include "tetrimino.h"

namespace Tetris
{
	struct Offset
	{
		int x, y;
	};

	Offset minoOffsets[PieceType::NUM_PIECES][RotationState::NUM_ROTATIONS][4] = {
		// BLOCK
		{
			{ { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },
			{ { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },
			{ { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } },
			{ { 1, 0 }, { 2, 0 }, { 1, 1 }, { 2, 1 } }
		},
		// L
		{
			{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 0, 1 } },
			{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 1, 2 } },
			{ { 0, 2 }, { 1, 2 }, { 2, 2 }, { 2, 1 } },
			{ { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 2 } }
		},
		// J
		{
			{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 2, 1 } },
			{ { 1, 0 }, { 1, 1 }, { 1, 2 }, { 0, 2 } },
			{ { 0, 1 }, { 0, 2 }, { 1, 2 }, { 2, 2 } },
			{ { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 0 } }
		},
		// S
		{
			{ { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },
			{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
			{ { 1, 0 }, { 2, 0 }, { 0, 1 }, { 1, 1 } },
			{ { 0, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } }
		},
		// Z
		{
			{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
			{ { 2, 0 }, { 2, 1 }, { 1, 1 }, { 1, 2 } },
			{ { 0, 0 }, { 1, 0 }, { 1, 1 }, { 2, 1 } },
			{ { 2, 0 }, { 2, 1 }, { 1, 1 }, { 1, 2 } }
		},
		// I
		{
			{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },
			{ { 2, 0 }, { 2, 1 }, { 2, 2 }, { 2, 3 } },
			{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 3, 0 } },
			{ { 2, 0 }, { 2, 1 }, { 2, 2 }, { 2, 3 } }
		},
		// T
		{
			{ { 0, 0 }, { 1, 0 }, { 2, 0 }, { 1, 1 } },
			{ { 1, 0 }, { 0, 1 }, { 1, 1 }, { 1, 2 } },
			{ { 1, 1 }, { 0, 2 }, { 1, 2 }, { 2, 2 } },
			{ { 1, 0 }, { 1, 1 }, { 1, 2 }, { 2, 1 } }
		}
	};

	Tetromino::Tetromino() :
		field(0), x(0), y(0), orientation(SPAWN), type(NONE), locktimer(-1)
	{}

	void Tetromino::setField(Tetrion* tetrion)
	{
		field = tetrion;
	}

	void Tetromino::place()
	{
		if (type == NONE)
		{
			return;
		}

		for (int i = 0; i < 4; ++i)
		{
			Mino* mino = field->get(x + minoOffsets[type][orientation][i].x, y + minoOffsets[type][orientation][i].y);
			mino->state = ACTIVE;
			mino->piece = type;
		}
	}

	bool Tetromino::isBlockout()
	{
		for (int i = 0; i < 4; ++i)
		{
			if (field->get(x + minoOffsets[type][orientation][i].x, y + minoOffsets[type][orientation][i].y)->state == ACTIVE)
			{
				return true;
			}
		}

		return false;
	}

	void Tetromino::lift()
	{
		if (type == NONE)
		{
			return;
		}

		for (int i = 0; i < 4; ++i)
		{
			Mino* mino = field->get(x + minoOffsets[type][orientation][i].x, y + minoOffsets[type][orientation][i].y);
			mino->state = EMPTY;
			mino->piece = NONE;
		}
	}

	bool Tetromino::isValidPlacement(int x, int y)
	{
		for (int i = 0; i < 4; ++i)
		{
			Mino* mino = field->get(x + minoOffsets[type][orientation][i].x, y + minoOffsets[type][orientation][i].y);
			if (!mino || mino->state == ACTIVE)
			{
				return false;
			}
		}

		return true;
	}

	bool Tetromino::shift(int dx)
	{
		bool moved = false;
		lift();
		if (isValidPlacement(x + dx, y))
		{
			x += dx;
			moved = true;
		}
		place();
		return moved;
	}

	bool Tetromino::rotate(int direction)
	{
		lift();

		bool moved = true;
		RotationState lastRotation = orientation;

		orientation = (RotationState)((orientation + direction + NUM_ROTATIONS) % NUM_ROTATIONS);
		if (!isValidPlacement(x, y))
		{
			// Somewhat using the TGM wall kick setup http://tetrisconcept.net/wiki/TGM_rotation
			//TODO: Implement full kick setup
			if (kickEnabled && type != I)
			{
				if (isValidPlacement(x + 1, y))
				{
					++x;
				}
				else if (isValidPlacement(x - 1, y))
				{
					--x;
				}
				else
				{
					orientation = lastRotation;
					moved = false;
				}
			}
			else
			{
				orientation = lastRotation;
				moved = false;
			}
		}

		place();
		return moved;
	}
}