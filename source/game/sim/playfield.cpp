#include "playfield.h"

namespace Tetris
{
	Playfield::~Playfield()
	{
		Destroy();
	}

	void Playfield::Init(int w, int h)
	{
		Destroy();

		width = w;
		height = h;
		blocks = new Block[width * height];
	}

	void Playfield::Destroy()
	{
		if (blocks)
		{
			delete[] blocks;
		}
	}

	Block* Playfield::Get(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < width && y < height)
		{
			return blocks + (x + y * width);
		}

		return 0;
	}

	void Playfield::Clear()
	{
		for (int i = 0; i < width * height; ++i)
		{
			blocks[i].state = EMPTY;
		}
	}

	int Playfield::CheckLines()
	{
		int numLines = 0;
		for (int y = 0; y < height; ++y)
		{
			bool lineFilled = true;
			for (int x = 0; x < width; ++x)
			{
				if (Get(x, y)->state != ACTIVE)
				{
					lineFilled = false;
					break;
				}
			}

			if (lineFilled)
			{
				++numLines;
				for (int x = 0; x < width; ++x)
				{
					Get(x, y)->state = CLEAR;
				}
			}
		}

		return numLines;
	}

	void Playfield::RemoveCleared()
	{
		int y = height - 1;
		while (y >= 0)
		{
			if (Get(0, y)->state == CLEAR)
			{
				for (int copyY = y; copyY > 0; --copyY)
				{
					for (int x = 0; x < width; ++x)
					{
						Get(x, copyY)->state = Get(x, copyY - 1)->state;
						Get(x, copyY)->piece = Get(x, copyY - 1)->piece;
					}
				}

				for (int x = 0; x < width; ++x)
				{
					Get(x, 0)->state = EMPTY;
				}
			}
			else
			{
				--y;
			}
		}
	}
}
