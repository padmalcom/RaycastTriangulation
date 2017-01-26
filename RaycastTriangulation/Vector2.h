#pragma once

#include <math.h>

const double PI = atan(1.0) * 4;

class Vector2
{
public:
	Vector2(float _x, float _y);
	Vector2();
	~Vector2();

	float x, y;

	static float angBetweenVecs(Vector2 *_v1, Vector2 *_v2);
	static bool isLineInBetweenVectors(Vector2 *_v1, Vector2 *_v2, Vector2 *_line);
	static Vector2* sub(Vector2 *_v1, Vector2 *_v2);
	static Vector2* normalize(Vector2 *_v);
	static float length(Vector2 *_v);

	bool operator == (const Vector2& v) const;

	struct sortByAngle
	{
		inline bool operator() (const Vector2& _v1, const Vector2& _v2)
		{
			Vector2 *v1 = new Vector2(_v1);
			Vector2 *v2 = new Vector2(_v2);
			Vector2 *v1Norm = Vector2::normalize(v1);
			Vector2 *v2Norm = Vector2::normalize(v2);

			float arc1 = atan2(0.0f, -1.0f) - atan2(v1Norm->y, v1Norm->x);
			float arc2 = atan2(0.0f, -1.0f) - atan2(v2Norm->y, v2Norm->x);

			return (arc1 > arc2);
		}
	};
};

