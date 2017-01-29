#pragma once

#include <math.h>
#include <algorithm>

const double PI = atan(1.0) * 4;
const double EPSILON = std::numeric_limits<double>::epsilon();

class Vector2
{
public:
	Vector2(float _x, float _y);
	Vector2();
	~Vector2();

	float x, y;

	static double angBetweenVecs(Vector2 &_v1, Vector2 &_v2);
	static bool isLineInBetweenVectors(Vector2 &_v1, Vector2 &_v2, Vector2 &_line);
	
	Vector2 normalize();
	float length();
	bool operator == (const Vector2& v) const;
	bool operator != (const Vector2& v) const;
	Vector2 operator - (const Vector2& v) const;
	Vector2 operator + (const Vector2& v) const;

	struct sortByAngle
	{
		inline bool operator() (const Vector2& _v1, const Vector2& _v2)
		{
			Vector2 v1 = _v1;
			Vector2 v2 = _v2;
			Vector2 v1Norm = v1.normalize();
			Vector2 v2Norm = v2.normalize();

			double arc1 = atan2(0.0f, -1.0f) - atan2(v1Norm.y, v1Norm.x);
			double arc2 = atan2(0.0f, -1.0f) - atan2(v2Norm.y, v2Norm.x);

			return (arc1 > arc2);
		}
	};
};

