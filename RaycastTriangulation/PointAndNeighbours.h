#pragma once

#include <vector>
#include "Vector2.h"

class PointAndNeighbours;

class PointAndNeighbours
{
public:
	PointAndNeighbours(Vector2 *_p, Vector2 *_prev, Vector2 *_next, int _index, int _holeId);
	~PointAndNeighbours();

	Vector2 *p;
	Vector2 *prev;
	Vector2 *next;
	double angle;
	int holeId;
	std::vector<PointAndNeighbours*> neighbours;
	PointAndNeighbours *nextPan;
	int index;

};

