#ifndef BOX_H
#define BOX_H

#include "SDL/SDL.h"
#include "../util/Vector.h"

class Box
{
public:
	Box() : width(0), height(0), enabled(true)
	{
		color.a = color.r = color.g = color.b = 255;
	}

	Box(int x, int y, int width, int height, SDL_Color color)
		: position((float)x, (float)y), width(width), height(height), color(color)
	{}

	void Update(float delta)
	{
		if (totalAnimTime > 0.0f)
		{
			animTime += delta;
			if (animTime >= totalAnimTime)
			{
				totalAnimTime = 0.0f;
				position.x = (float)destx;
				position.y = (float)desty;
			}
			else
			{
				position.x += (destx - position.x) * (animTime / totalAnimTime);
				position.y += (desty - position.y) * (animTime / totalAnimTime);
			}
		}

		position.x += (velocity.x * delta);
		position.y += (velocity.y * delta);
	}

	void Render(SDL_Renderer* renderer) const
	{
		SDL_Rect fillRect = { (int)position.x, (int)position.y, width, height };
		SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
		SDL_RenderFillRect(renderer, &fillRect);
	}

	void Move(int x, int y, float time)
	{
		destx = x;
		desty = y;
		animTime = 0.0f;
		totalAnimTime = time;
	}

	bool Intersects(const Box& box)
	{
		return !(position.x > box.position.x + box.width
			|| position.x + width < box.position.x
			|| position.y > box.position.y + box.height
			|| position.y + height < box.position.y);
	}

	void Speed(float val)
	{
		speed = val;
		velocity = velocity.Normalize() * speed;
	}

	float Speed()
	{
		return speed;
	}

	Vector position;
	Vector velocity;
	int width;
	int height;
	SDL_Color color;
	bool enabled;
private:
	int destx, desty;
	float animTime;
	float totalAnimTime;
	float speed;
};

#endif