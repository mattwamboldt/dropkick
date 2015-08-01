#include "tetrion.h"

namespace Tetris
{
	void Tetrion::Init(int w, int h, int x, int y, SDL_Texture** blockTextures, SDL_Texture* frameTexture, SDL_Texture* gridTexture)
	{
		frame = 0;
		frameWidth = 0;
		blockImages = blockTextures;
		frameImage = frameTexture;
		gridImage = gridTexture;

		playfield.Init(w, h);
		
		gridRect.x = x;
		gridRect.y = y;
		gridRect.w = w * MINO_SIZE;
		gridRect.h = h * MINO_SIZE;

		if (frameImage)
		{
			SDL_QueryTexture(frameImage, NULL, NULL, &frameWidth, NULL);
			frameWidth /= 3;
			 
			frameRect.x = x - frameWidth;
			frameRect.y = y - frameWidth;
			frameRect.w = gridRect.w + frameWidth * 2;
			frameRect.h = gridRect.h + frameWidth * 2;
		}
	}

	void Tetrion::Render(SDL_Renderer* screen)
	{
		if (!frame && frameImage)
		{
			// We want to construct a frame backing texture from the source one
			frame = SDL_CreateTexture(screen, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, frameRect.w, frameRect.h);
			if (frame)
			{
				SDL_SetRenderTarget(screen, frame);
				
				//Upper left corner
				SDL_Rect srcRect = { 0, 0, frameWidth, frameWidth };
				SDL_Rect dstRect = { 0, 0, frameWidth, frameWidth };
				SDL_RenderCopy(screen, frameImage, &srcRect, &dstRect);

				//Upper Right corner
				srcRect.x = frameWidth * 2;
				dstRect.x = frameRect.w - frameWidth;
				SDL_RenderCopy(screen, frameImage, &srcRect, &dstRect);

				//Lower Right corner
				srcRect.y = frameWidth * 2;
				dstRect.y = frameRect.h - frameWidth;
				SDL_RenderCopy(screen, frameImage, &srcRect, &dstRect);

				//Lower Left corner
				srcRect.x = 0;
				dstRect.x = 0;
				SDL_RenderCopy(screen, frameImage, &srcRect, &dstRect);

				//Top
				srcRect.x = frameWidth;
				srcRect.y = 0;

				dstRect.x = frameWidth;
				dstRect.w = frameRect.w - frameWidth * 2;
				dstRect.y = 0;
				SDL_RenderCopy(screen, frameImage, &srcRect, &dstRect);

				//bottom
				srcRect.y = frameWidth * 2;
				dstRect.y = frameRect.h - frameWidth;
				SDL_RenderCopy(screen, frameImage, &srcRect, &dstRect);

				//Left
				srcRect.x = 0;
				srcRect.y = frameWidth;

				dstRect.x = 0;
				dstRect.y = frameWidth;
				dstRect.w = frameWidth;
				dstRect.h = frameRect.h - frameWidth * 2;
				SDL_RenderCopy(screen, frameImage, &srcRect, &dstRect);

				//Right
				srcRect.x = frameWidth * 2;
				dstRect.x = frameRect.w - frameWidth;
				SDL_RenderCopy(screen, frameImage, &srcRect, &dstRect);
				
				SDL_SetRenderTarget(screen, NULL);
			}

			frameImage = 0;
		}

		if (frame)
		{
			SDL_RenderCopy(screen, frame, NULL, &frameRect);
		}

		if (gridImage)
		{
			SDL_RenderCopy(screen, gridImage, NULL, &gridRect);
		}

		for (int y = 0; y < playfield.GetHeight(); ++y)
		{
			int screenY = y * MINO_SIZE + gridRect.y;
			for (int x = 0; x < playfield.GetWidth(); ++x)
			{
				Render(screen, playfield.Get(x, y), x * MINO_SIZE + gridRect.x, screenY);
			}
		}
	}

	void Tetrion::Render(SDL_Renderer* renderer, Block* block, int x, int y)
	{
		SDL_Rect fillRect = { x, y, MINO_SIZE, MINO_SIZE };

		if (block->state == ACTIVE)
		{
			if (blockImages)
			{
				SDL_RenderCopy(renderer, blockImages[block->piece], NULL, &fillRect);
			}
			else
			{
				SDL_Color color = pieceColors[block->piece];
				SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
				SDL_RenderFillRect(renderer, &fillRect);
			}
		}
	}
}