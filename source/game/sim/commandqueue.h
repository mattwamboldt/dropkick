#ifndef SIM_COMMANDQUEUE_H
#define SIM_COMMANDQUEUE_H

#include "..\tetris\types.h"
#include <queue>

namespace Tetris
{
	struct Command
	{
		CommandType type;
		int playerIndex;
		int frame;
	};

	typedef std::queue<Command> CommandQueue;
}

#endif