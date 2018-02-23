#include "stdafx.h"
#include "Triangle.h"


Triangle::Triangle(Vector2 &_a, Vector2 &_b, Vector2 &_c)
{
	this->a = _a;
	this->b = _b;
	this->c = _c;
}


Triangle::~Triangle()
{
}
