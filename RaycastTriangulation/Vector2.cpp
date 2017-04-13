#include "stdafx.h"
#include "Vector2.h"
#include <math.h>
#include "TinyMath.h"

Vector2::Vector2(double _x, double _y)
{
	x = _x;
	y = _y;
}

Vector2::Vector2() {}

Vector2::~Vector2()
{
}

double Vector2::angBetweenVecs(Vector2 &_v1, Vector2 &_v2) {

	Vector2 v1Norm = _v1.normalize();
	Vector2 v2Norm = _v2.normalize();

	double angle = acos(v1Norm.x * v2Norm.x + v1Norm.y * v2Norm.y) * 180 / PI;

	// Calculate cross product z component
	double z = TinyMath::crossProductZ(_v1, _v2);
	if (z > 0.0f) {
		angle = 360.0f - angle;
	}

	return angle;
}

bool Vector2::isLineInBetweenVectors(Vector2 &_v1, Vector2 &_v2, Vector2 &_line) {
	//printf("Checking if (%f,%f) is between (%f,%f) and (%f,%f) = ", _line.x, _line.y, _v1.x, _v1.y, _v2.x, _v2.y);

	float a = fmodf(atan2(_line.x, _line.y) - atan2(_v2.x, _v2.y), 360.0f) * 180 / PI;
	float b = fmodf(atan2(_v1.x, _v1.y) - atan2(_v2.x, _v2.y), 360.0f) * 180 / PI;

	float cross1 = TinyMath::crossProductZ(_line, _v2);
	float cross2 = TinyMath::crossProductZ(_v1, _v2);
	if (cross1 > 0) {
		if (cross2 <= 0) {
			a = 360.0f - a;
		}
	}
	if (cross1 < 0) {
		if (cross2 >= 0) {
			b = 360.0f - b;
		}
	}

	//printf("Angle a: %f, angle b: %f. A smaller B? Cross a: %f, cross b: %f\n\n.", a, b, cross1, cross2);
	return a < b;
}

Vector2 Vector2::normalize() {
	double len = this->length();
	return Vector2(this->x / len, this->y / len);
}

double Vector2::length() {
	return sqrtf(this->x * this->x + this->y * this->y);
}

bool Vector2::operator == (const Vector2& v) const {
	return TinyMath::compareDouble(this->x, v.x) && TinyMath::compareDouble(this->y, v.y);
	// return this->x == v.x && this->y == v.y;
}

bool Vector2::operator != (const Vector2& v) const {
	return !(*this == v);
}

Vector2 Vector2::operator - (const Vector2& v) const {
	return Vector2(this->x - v.x, this->y - v.y);
}

Vector2 Vector2::operator + (const Vector2& v) const {
	return Vector2(this->x + v.x, this->y + v.y);
}