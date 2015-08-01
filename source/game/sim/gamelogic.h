#ifndef TETRIS_GAMELOGIC_H
#define TETRIS_GAMELOGIC_H

#include "commandqueue.h"
#include "..\tetris\types.h"
#include "settings.h"
#include "playfield.h"

namespace Tetris
{
	class GameLogic
	{
	public:
		GameLogic(){}

		// Moves the simulation forward one frame, does not use delta time to be independant of that
		void Tick();
		void AddCommand(CommandType command, int playerIndex);

	private:

		CommandQueue mCommandQueue;
		Mode currentMode;
		Settings settings;

		std::vector<Playfield*> playfields;
	};
}

#endif