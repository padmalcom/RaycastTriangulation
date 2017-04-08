#pragma once

#include "Vector2.h"
#include <math.h>

const double PI = atan(1.0) * 4;
const double EPSILON = std::numeric_limits<double>::epsilon();

class TinyMath
{
public:
	static bool compareDouble(double a, double b);
	static double crossProductZ(const Vector2 &a, const Vector2 &b);
	static bool pointInTriangle(Vector2 &point, Vector2 &a, Vector2 &b, Vector2 &c);
	static float cycle(float _x, float _min, float _max);

private:
	static float sign(const Vector2 &p1, const Vector2 &p2, const Vector2 &p3);
};

