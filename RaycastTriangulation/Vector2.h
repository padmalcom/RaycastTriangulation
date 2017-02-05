#pragma once

#include <math.h>
#include <algorithm>

class Vector2
{
public:
	Vector2(double _x, double _y);
	Vector2();
	~Vector2();

	double x, y;

	static double angBetweenVecs(Vector2 &_v1, Vector2 &_v2);
	
	Vector2 normalize();
	double length();
	bool operator == (const Vector2& v) const;
	bool operator != (const Vector2& v) const;
	Vector2 operator - (const Vector2& v) const;
	Vector2 operator + (const Vector2& v) const;
};

