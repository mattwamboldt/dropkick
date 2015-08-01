#ifndef SIM_PLAYFIELD_H
#define SIM_PLAYFIELD_H

#include "block.h"

namespace Tetris
{
	class Playfield
	{
	public:
		~Playfield();

		void Init(int w, int h);
		void Destroy();
		void Clear();

		int CheckLines();
		void RemoveCleared();

		Block* Get(int x, int y);
		int GetWidth(){ return width; }
		int GetHeight(){ return height; }

	private:
		Block* blocks;
		int width, height;
	};
}

#endif