#include "mino.h"

namespace Tetris
{
	void Mino::Render(SDL_Renderer* renderer, SDL_Texture** blockImages) const
	{
		SDL_Rect fillRect = { (int)position.x, (int)position.y, width, height };

		if (state == ACTIVE)
		{
			if (blockImages)
			{
				SDL_RenderCopy(renderer, blockImages[piece], NULL, &fillRect);
			}
			else
			{
				SDL_Color color = pieceColors[piece];
				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
				SDL_RenderFillRect(renderer, &fillRect);
			}
		}
	}
}