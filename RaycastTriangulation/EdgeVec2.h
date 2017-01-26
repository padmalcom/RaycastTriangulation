#pragma once

#include "Vector2.h"

class EdgeVec2
{
public:
	EdgeVec2(Vector2 *_a, Vector2 *_b, bool _isOuter);
	~EdgeVec2();

	Vector2 *a;
	Vector2 *b;
	bool isOuter;
};

