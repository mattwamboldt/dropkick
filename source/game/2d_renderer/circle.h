#include "SDL/SDL.h"
#include <math.h>

class Circle
{
public:
	Circle(int x, int y, int radius, SDL_Color color)
		: x(x), y(y), radius(radius), color(color)
	{}

	void Update(float delta)
	{
		if (totalAnimTime > 0.0f)
		{
			animTime += delta;
			if (animTime >= totalAnimTime)
			{
				totalAnimTime = 0.0f;
				x = destx;
				y = desty;
			}
			else
			{
				x += (destx - x) * (animTime / totalAnimTime);
				y += (desty - y) * (animTime / totalAnimTime);
			}
		}
	}

	void Render(SDL_Surface* screen) const
	{
		Uint32* p_screen = (Uint32*)screen->pixels;

		for (int y1 = y - radius; y1 < y + radius && y1 < screen->h; ++y1)
		{
			for (int x1 = x - radius; x1 < x + radius && x1 < screen->w; ++x1)
			{
				if (pow((float)y - y1, 2) + pow((float)x - x1, 2) <= radius * radius)
				{
					*(p_screen + y1*screen->w + x1) = SDL_MapRGBA(screen->format, color.r, color.g, color.b, color.a);
				}
			}
		}
	}

	void Move(int x, int y, float time)
	{
		destx = x;
		desty = y;
		animTime = 0.0f;
		totalAnimTime = time;
	}

	int x;
	int y;
	int radius;
	SDL_Color color;
private:
	int destx, desty;
	float animTime;
	float totalAnimTime;
};