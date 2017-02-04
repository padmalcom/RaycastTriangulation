#pragma once

#include "Vector2.h"

class Intersection
{
public:
	static bool line_intersection(Vector2 &_p1, Vector2 &_p2, Vector2 &_p3, Vector2 &_p4, Vector2 *&_intersection);
};

