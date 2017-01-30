#include "stdafx.h"
#include "EdgeVec2.h"


EdgeVec2::EdgeVec2(Vector2 &_a, Vector2 &_b, bool _isOuter)
{
	this->a = _a;
	this->b = _b;
	this->isOuter = _isOuter;
}


EdgeVec2::~EdgeVec2()
{
}
