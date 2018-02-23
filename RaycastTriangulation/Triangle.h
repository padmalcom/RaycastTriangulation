#pragma once

#include "Vector2.h"

class Triangle
{
public:
	Triangle(Vector2 &_a, Vector2 &_b, Vector2 &_c);
	~Triangle();

	Vector2 a;
	Vector2 b;
	Vector2 c;
};

