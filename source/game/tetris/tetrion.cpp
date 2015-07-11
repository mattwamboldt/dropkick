#include "tetrion.h"

namespace Tetris
{
	Tetrion::~Tetrion()
	{
		if (frame)
		{
			SDL_DestroyTexture(frame);
			frame = 0;
		}

		if (blocks)
		{
			delete[] blocks;
		}
	}

	void Tetrion::Init(int w, int h, int x, int y, SDL_Texture** blockTextures, SDL_Texture* frameTexture, SDL_Texture* gridTexture)
	{
		frame = 0;
		frameWidth = 0;
		blockImages = blockTextures;
		frameImage = frameTexture;
		gridImage = gridTexture;

		if (blocks)
		{
			delete[] blocks;
		}

		width = w;
		height = h;
		blocks = new Mino[width * height];

		if (frameImage)
		{
			SDL_QueryTexture(frameImage, NULL, NULL, &frameWidth, NULL);
			frameWidth /= 3;
			 
			frameRect.x = x - frameWidth;
			frameRect.y = y - frameWidth;
			frameRect.w = width * MINO_SIZE + frameWidth * 2;
			frameRect.h = height * MINO_SIZE + frameWidth * 2;
		}

		gridRect.x = x;
		gridRect.y = y;
		gridRect.w = width * MINO_SIZE;
		gridRect.h = height * MINO_SIZE;

		for (int i = 0; i < width * height; ++i)
		{
			blocks[i].position.x = (float)(x + ((i % width) * MINO_SIZE));
			blocks[i].position.y = (float)(y + ((i / width) * MINO_SIZE));
			blocks[i].width = MINO_SIZE;
			blocks[i].height = MINO_SIZE;
		}
	}

	Mino* Tetrion::get(int x, int y)
	{
		if (x >= 0 && y >= 0 && x < width && y < height)
		{
			return blocks + (x + y * width);
		}

		return 0;
	}

	void Tetrion::clear()
	{
		for (int i = 0; i < width * height; ++i)
		{
			//initialize the field
			blocks[i].state = EMPTY;
		}
	}

	int Tetrion::checkLines()
	{
		int numLines = 0;
		for (int y = 0; y < height; ++y)
		{
			bool lineFilled = true;
			for (int x = 0; x < width; ++x)
			{
				if (get(x, y)->state != ACTIVE)
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
					get(x, y)->state = CLEAR;
				}
			}
		}

		return numLines;
	}

	void Tetrion::removeCleared()
	{
		int y = height - 1;
		while (y >= 0)
		{
			if (get(0, y)->state == CLEAR)
			{
				for (int copyY = y; copyY > 0; --copyY)
				{
					for (int x = 0; x < width; ++x)
					{
						get(x, copyY)->state = get(x, copyY - 1)->state;
						get(x, copyY)->piece = get(x, copyY - 1)->piece;
					}
				}

				for (int x = 0; x < width; ++x)
				{
					get(x, 0)->state = EMPTY;
				}
			}
			else
			{
				--y;
			}
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

		for (int i = 0; i < width * height; ++i)
		{
			blocks[i].Render(screen, blockImages);
		}
	}
}