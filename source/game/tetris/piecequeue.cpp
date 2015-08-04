#include "piecequeue.h"
#include "types.h"
#include <cstdio>

namespace Tetris
{
	void PieceQueue::Init(int x, int y, SDL_Texture** blockTextures, SDL_Texture* frameTexture)
	{
		sidePreview.Init(4, 12, x, y, blockTextures, frameTexture);
		previewPieces[0].setField(sidePreview.GetField());
		previewPieces[1].setField(sidePreview.GetField());
		previewPieces[2].setField(sidePreview.GetField());

		for (int i = 0; i < 3; ++i)
		{
			previewPieces[i].x = 0;
			previewPieces[i].y = i * 4;
		}
	}

	void PieceQueue::Reset()
	{
		for (int i = 0; i < 3; ++i)
		{
			previewPieces[i].lift();
			previewPieces[i].type = randomizer.next();
			previewPieces[i].locktimer = -1;
			previewPieces[i].orientation = SPAWN;
			previewPieces[i].place();
		}
	}

	PieceType PieceQueue::advance()
	{
		PieceType nextPiece = previewPieces[0].type;

		for (int i = 0; i < 3; ++i)
		{
			previewPieces[i].lift();

			if (i == 2)
			{
				previewPieces[i].type = randomizer.next();
			}
			else
			{
				previewPieces[i].type = previewPieces[i + 1].type;
			}

			previewPieces[i].locktimer = -1;
			previewPieces[i].orientation = SPAWN;
			previewPieces[i].place();
		}

		return nextPiece;
	}

	void PieceQueue::Render(SDL_Renderer* screen)
	{
		sidePreview.Render(screen);
	}
}