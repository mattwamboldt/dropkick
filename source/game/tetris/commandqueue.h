#ifndef SIM_COMMANDQUEUE_H
#define SIM_COMMANDQUEUE_H

#include "types.h"
#include <queue>

namespace Tetris
{
	struct Command
	{
		CommandType type;
		int playerIndex;
	};

	typedef std::queue<Command> CommandQueue;
}

#endif