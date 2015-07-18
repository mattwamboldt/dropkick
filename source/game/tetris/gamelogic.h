#ifndef TETRIS_GAMELOGIC_H
#define TETRIS_GAMELOGIC_H

#include "commandqueue.h"

namespace Tetris
{
	class GameLogic
	{
	public:
		GameLogic(){}

		// Moves the simulation forward one frame, does not use delta time to be independant of that
		void Tick();

	private:
		CommandQueue mCommandQueue;

		enum BlockStates
		{
			EMPTY,
			ACTIVE,
			CLEAR,
		};

		BlockStates* blocks;
	};
}

#endif