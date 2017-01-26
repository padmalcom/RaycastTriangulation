#include "stdafx.h"
#include "Intersection.h"


Intersection::Intersection()
{
}


Intersection::~Intersection()
{
}

bool Intersection::line_intersection(Vector2 *_p1, Vector2 *_p2, Vector2 *_p3, Vector2 *_p4, Vector2 *_intersection) {
	float Ax, Bx, Cx, Ay, By, Cy, d, e, f, num;
	float x1lo, x1hi, y1lo, y1hi;

	Ax = _p2->x - _p1->x;
	Bx = _p3->x - _p4->x;

	// X bound box test/
	if (Ax < 0)
	{
		x1lo = _p2->x; x1hi = _p1->x;
	}
	else
	{
		x1hi = _p2->x; x1lo = _p1->x;
	}

	if (Bx > 0)
	{
		if (x1hi < _p4->x || _p3->x < x1lo) return false;
	}
	else
	{
		if (x1hi < _p3->x || _p4->x < x1lo) return false;
	}

	Ay = _p2->y - _p1->y;
	By = _p3->y - _p4->y;

	// Y bound box test//
	if (Ay < 0)
	{
		y1lo = _p2->y; y1hi = _p1->y;
	}
	else
	{
		y1hi = _p2->y; y1lo = _p1->y;
	}

	if (By > 0)
	{
		if (y1hi < _p4->y || _p3->y < y1lo) return false;
	}
	else
	{
		if (y1hi < _p3->y || _p4->y < y1lo) return false;
	}

	Cx = _p1->x - _p3->x;
	Cy = _p1->y - _p3->y;
	d = By * Cx - Bx * Cy;  // alpha numerator//
	f = Ay * Bx - Ax * By;  // both denominator//

	// alpha tests//
	if (f > 0)
	{
		if (d < 0 || d > f) return false;
	}
	else
	{
		if (d > 0 || d < f) return false;
	}

	e = Ax * Cy - Ay * Cx;  // beta numerator//

	// beta tests //
	if (f > 0)
	{
		if (e < 0 || e > f) return false;
	}
	else
	{
		if (e > 0 || e < f) return false;
	}

	// check if they are parallel
	if (f == 0) return false;

	// compute intersection coordinates //
	num = d * Ax; // numerator //

	_intersection->x = _p1->x + num / f;

	num = d * Ay;
	_intersection->y = _p1->y + num / f;
	return true;
}
