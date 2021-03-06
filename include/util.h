#ifndef util_H
#define util_H

#include "vex.h"

enum ArmDirections {
  north = 0,
  east = 90,
  south = 180,
  west = -90,
};


int ResetToggle(void *arg)
{
	bool *canChange = (bool *)arg;
	*canChange = false;
	vex::this_thread::sleep_for(500);
	*canChange = true;
  return 1;
}

bool floatWithin(float n, float min, float max)
{
	if (n < min)
	{
		return false;
	}
	if (n > max)
	{
		return false;
	}
	return true;
}

class Vector2
{
public:
	Vector2(float x, float y);
	float x;
	float y;
	bool within(float v);
};
Vector2::Vector2(float _x, float _y)
{
	x = _x;
	y = _y;
}

bool Vector2::within(float v)
{
	if (!floatWithin(x, -v, v))
	{
		return false;
	}
	if (!floatWithin(y, -v, v))
	{
		return false;
	}
	return true;
}

#endif