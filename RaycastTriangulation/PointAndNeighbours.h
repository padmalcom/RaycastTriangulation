#pragma once

#include <vector>
#include "Vector2.h"

class PointAndNeighbours;

class PointAndNeighbours
{
public:
	PointAndNeighbours(Vector2 *_p, Vector2 *_prev, Vector2 *_next, int _index, bool _isHole);
	PointAndNeighbours(const PointAndNeighbours &_pan);
	~PointAndNeighbours();

	Vector2 *p;
	Vector2 *prev;
	Vector2 *next;
	float angle;
	bool isHole = false;
	std::vector<PointAndNeighbours*> neighbours;
	PointAndNeighbours *nextPan;
	int index;

};

