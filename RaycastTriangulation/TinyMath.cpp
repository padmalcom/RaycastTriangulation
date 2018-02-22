#include "stdafx.h"
#include "TinyMath.h"

bool TinyMath::compareDouble(double a, double b) {
	return fabs(a - b) < 0.001f;
}

double TinyMath::crossProductZ(const Vector2 &a, const Vector2 &b) {
	return a.x * b.y - a.y * b.x;
}

float TinyMath::sign(const Vector2 &p1, const Vector2 &p2, const Vector2 &p3)
{
	return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
}

float TinyMath::cycle(float _x, float _min, float _max) {
	while (_x < _min) {
		_x += _max - _min;
	}
	while (_x > _max) {
		_x -= _max + _min;
	}
	return _x;
}

bool TinyMath::pointInTriangle(Vector2 &point, Vector2 &a, Vector2 &b, Vector2 &c) {
	bool b1, b2, b3;

	b1 = TinyMath::sign(point, a, b) < 0.0f;
	b2 = TinyMath::sign(point, b, c) < 0.0f;
	b3 = TinyMath::sign(point, c, a) < 0.0f;

	return ((b1 == b2) && (b2 == b3));
}

int TinyMath::map(int x, int in_min, int in_max, int out_min, int out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
