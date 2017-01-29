#include "stdafx.h"
#include "Vector2.h"
#include <math.h>
#include "TinyMath.h"

Vector2::Vector2(float _x, float _y)
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
	float angle = acos(v1Norm.x * v2Norm.x + v1Norm.y * v2Norm.y) * 180 / PI;

	// Calculate cross product z component
	float z = _v1.x * _v2.y - _v1.y * _v2.x;
	if (z > 0.0f) {
		angle = 360.0f - angle;
	}

	//printf("Angle between (%f,%f) and (%f,%f) is %f\n", _v1.x, _v1.y, _v2.x, _v2.y, angle);
	return angle;
}

Vector2 Vector2::normalize() {
	float len = this->length();
	return Vector2(this->x / len, this->y / len);
}

float Vector2::length() {
	return sqrtf(this->x * this->x + this->y * this->y);
}


bool Vector2::isLineInBetweenVectors(Vector2 &_v1, Vector2 &_v2, Vector2 &_line) {

	//printf("Checking if (%f,%f) is between (%f,%f) and (%f,%f).\n", _line->x, _line->y, _v1->x, _v1->y, _v2->x, _v2->y);

	float ang1 = Vector2::angBetweenVecs(_v1, _v2);
	float ang2 = Vector2::angBetweenVecs(_v1, _line);

	//printf("Angle 1 %f is and Angle 2 is %f.\n", ang1, ang2);
	return ang2 > 0 && ang2 < ang1;
}

bool Vector2::operator == (const Vector2& v) const {
	return TinyMath::compareFloat(this->x, v.x) && TinyMath::compareFloat(this->y, v.y);
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


