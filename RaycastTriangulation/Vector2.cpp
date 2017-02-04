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

	//printf("Normalized 1: (%f, %f) 2: (%f,%f).\n", v1Norm.x, v1Norm.y, v2Norm.x, v2Norm.y);
	
	//float angle = (atan2(v2Norm.y, v2Norm.x) - atan2(v1Norm.y, v1Norm.x)) * 180 / PI;
	double angle = acos(v1Norm.x * v2Norm.x + v1Norm.y * v2Norm.y) * 180 / PI;

	// Calculate cross product z component
	double z = TinyMath::crossProductZ(_v1, _v2);
	if (z > 0.0f) {
		angle = 360.0f - angle;
	}

	//printf("Angle between (%f,%f) and (%f,%f) is %f\n", _v1.x, _v1.y, _v2.x, _v2.y, angle);
	return angle;
}

Vector2 Vector2::normalize() {
	double len = this->length();
	return Vector2(this->x / len, this->y / len);
}

double Vector2::length() {
	return sqrtf(this->x * this->x + this->y * this->y);
}


bool Vector2::isLineInBetweenVectors(Vector2 &_v1, Vector2 &_v2, Vector2 &_line) {

	//printf("Checking if (%f,%f) is between (%f,%f) and (%f,%f).\n", _line->x, _line->y, _v1->x, _v1->y, _v2->x, _v2->y);

	double ang1 = Vector2::angBetweenVecs(_v1, _v2);
	double ang2 = Vector2::angBetweenVecs(_v1, _line);

	//printf("Angle 1 %f is and Angle 2 is %f.\n", ang1, ang2);
	return ang2 > 0 && ang2 < ang1;
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