#ifndef VECTOR_H
#define VECTOR_H
#include <math.h>
#include "SDL/SDL.h"

class Vector
{
public:
	static Vector unit(float degrees)
	{
		float radians = (float)(degrees * M_PI / 180.0);
		return Vector(cosf(radians), sinf(radians));
	}

	Vector() : x(0.0f), y(0.0f) {}
	Vector(float x, float y) : x(x), y(y) {}

	float Magnitude()
	{
		return sqrtf(x * x + y * y);
	}

	Vector Normalize()
	{
		float mag = Magnitude();
		return Vector(x / mag, y / mag);
	}

	Vector operator * (float maginitude)
	{
		return Vector(x*maginitude, y* maginitude);
	}

	float x, y;
};
#endif