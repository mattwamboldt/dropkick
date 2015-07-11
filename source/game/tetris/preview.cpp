#include "preview.h"

namespace Tetris
{
	void Preview::Init(int width, int x, int y)
	{
		for (int i = 0; i < 24; ++i)
		{
			window[i].position.x = (float)(x + ((i % 6) * width / 6));
			window[i].position.y = (float)(y + ((i / width) * width / 6));
			window[i].width = width / 6;
			window[i].height = width / 6;
		}
	}

	void Preview::SetPiece(PieceType piece)
	{

	}

	void Preview::Render(SDL_Renderer* renderer) const
	{
		for (int i = 0; i < 24; ++i)
		{
			window[i].Render(renderer);
		}
	}
}